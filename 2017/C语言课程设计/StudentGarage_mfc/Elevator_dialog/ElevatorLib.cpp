#include "stdafx.h"
#include <math.h>

#include "ElevatorLib.h"
#include "resource.h"
#include "Elevator_dialogDlg.h"

/*******************************************************************************
* 全局变量，作为Lib与mfc之间的共享的变量，进行信息传递。
 *******************************************************************************/
bool Lib_Running = false; // 正在运行仿真: true; 否则false
// 电梯箱体下边沿距离地面的相对高度,初始值0
double Lib_CurrentCarPosition = 0;
// 电梯箱体速度，Up>0,Down<0
double Lib_CurrentCarVelocity = 0;
// 电机功率，-1.0到1.0之间，设为1.0,电梯箱体以最大速度上升;设为-1.0,电梯箱体以最大速度下降;初始值0.0。
double Lib_Power = 0.0;
// 门内: 开、关门按钮灯
bool Lib_OpenDoorLight = false; 
bool Lib_CloseDoorLight = false;
// 门内: 楼层按钮灯(PanelFloorLight),数组元素对应门内楼层数字按钮的状态,注意下标0 -- Lib_FloorNum-1
bool Lib_PanelFloorLight[Lib_FloorNum] = {false,false,false};
// 门外: Up/Down按钮灯(Call Light),数组元素对应各楼层门外Up/Down按钮的状态,注意下标0 -- Lib_FloorNum-1
bool Lib_CallLightUp[Lib_FloorNum] = {false,false,false};
bool Lib_CallLightDown[Lib_FloorNum] = {false,false,false};

// 空闲状态，此时电梯是静止的，电梯当前运动趋势是上升(true)还是下降(false)
// IsgongUp()函数中，如果在1楼，Lib_gongUp=true; 如果在最高楼(Lib_FloorNum)，Lib_gongUp=false；
// IdleWhatFloorToGoTo()中维护该值
// 在elevator.cpp中读取该值，函数IsgoingUp()返回该值。
bool Lib_goingUp = true;
bool IsgoingUp() {
	if(GetNearestFloor() == 1) Lib_goingUp = true;
	else if (GetNearestFloor() == Lib_FloorNum) Lib_goingUp = false; 
	return Lib_goingUp;
}

// 目标楼层，即下一步将要到的楼层，初始值-1
int Lib_WillToFloor = -1;

// 开关门定时器已经启动，表示正在开关门,SetDoor()读取，mfc维护
bool Lib_DoorTimerStarted = false;
// 开门结束,初始化为false,在mfc中维护
bool Lib_DoorOpened = false;
// 关门结束,初始化为true,在mfc中维护
bool Lib_DoorClosed = true;

// 在Idle状态，一定时间（AutoTimerDuration）无动作，自动下降到1楼,该变量确定定时器是否启动
bool Lib_AutoTimerStarted = false;
int AutoTimerDuration = 10000;  // ms，时长，缺省10000ms,即10s
// 在Idle状态，当楼层>1时，调用此函数，一定时间无动作，自动转为MovingDown状态，自动下降到1楼
extern void AutoTo1Floor() 
{
	if (!Lib_AutoTimerStarted)
	{
		SetTimer(CElevator_dialogDlg::MAIN_WIN,ID_AUTO_TIMER,AutoTimerDuration,NULL);//10s,10000ms
		Lib_AutoTimerStarted = true;
	}
}
// 其它状态，取消自动到1楼
extern void CancelTo1Floor() 
{
	if (Lib_AutoTimerStarted)
	{
		KillTimer(CElevator_dialogDlg::MAIN_WIN,ID_AUTO_TIMER);
		Lib_AutoTimerStarted = false;
	}
}
// AutoTimerDuration时间到，自动执行到1楼的动作，在mfc定时器回调函数中被调用
extern void To1Floor(int *state) 
{
	printf("%d(s)时间到，自动下降到1楼...\n",AutoTimerDuration/1000);
	// 此时可能有电梯外的Up/Down Call Light按钮灯是亮的，索性关闭所有的；
	// 电梯内的楼层按钮也有可能是亮的，关闭之。
	for(int floor = 1; floor <= Lib_FloorNum; floor++) 
	{
		SetCallLight(floor,true,false);
		SetCallLight(floor,false,false);
		SetPanelFloorLight(floor,false);
	}
	SetMotorPower(-1);
	*state = MovingDown;
}

/** 
 * 仿真是否在运行，返回true，运行，否则没有运行
 */
bool IsElevatorRunning()
{
	return Lib_Running;
}

/** 
 * 启动仿真，在初始化时启动
 */
void ElevatorStartup()
{
	Lib_Running = true;
}

/** 
 * 结束仿真
 */
void ElevatorShutdown()
{
	Lib_Running = false;
}

/******************************************************************
 * 按钮灯(Lights): 
 * （1）门内开关门按钮灯；（2）门内楼层按钮灯（Panel Floor Light）；
 * （3）门外Up/Down按钮（Call Light）
 ******************************************************************/

/** 
 * 设置门内关门按钮灯的状态
 * 参数s: true,按钮灯亮，false，按钮灯灭
 */
void SetCloseDoorLight(bool s)
{
	Lib_CloseDoorLight = s;
	// 向mfc发送消息，更新电梯内外按钮灯状态，参数 2: 表示电梯内开关门按钮灯
	postToMfc(2,0,s,0);
}

/** 
 * 返回门内关门按钮灯的状态
 * true,按钮灯亮，false，按钮灯灭
 */
bool GetCloseDoorLight()
{
	return Lib_CloseDoorLight;
}

/** 
 * 设置门内开门按钮灯的状态
 * 参数s: true,按钮灯亮，false，按钮灯灭
 */
void SetOpenDoorLight(bool s)
{
	Lib_OpenDoorLight = s;
	// 向mfc发送消息，更新电梯内外按钮灯状态，参数 2: 表示电梯内开关门按钮灯
	postToMfc(2,0,s,0);
}

/** 
 * 返回门内开门按钮灯的状态
 * true,按钮灯亮，false，按钮灯灭
 */
bool GetOpenDoorLight()
{
	return Lib_OpenDoorLight;
}

/** 
 * 设置门内楼层按钮灯（Panel Floor Light）的状态
 * 参数floor: 表示楼层
 * 参数s: true,按钮灯亮，false，按钮灯灭
 */
void SetPanelFloorLight(int floor, bool s)
{
	Lib_PanelFloorLight[floor-1] = s;
	// 向mfc发送消息，更新电梯内外按钮灯状态，参数 1: 表示电梯内楼层按钮灯(Panel Floor Light)
	postToMfc(1,floor,s,0);
}

/** 
 * 返回门内楼层按钮灯（Panel Floor Light）的状态
 * 参数floor: 表示楼层
 * 返回：true,按钮灯亮，false，按钮灯灭
 */
bool GetPanelFloorLight(int floor)
{
	return Lib_PanelFloorLight[floor-1];
}

/** 
 * 设置门外Up/Down灯（Call Light）的状态
 * 参数floor: 表示楼层
 * 参数up: true,表示设置Up(上升)按钮灯;false,表示设置Down(下降)按钮灯
 * 参数s: true,按钮灯亮，false，按钮灯灭
 */
void SetCallLight(int floor, bool up, bool s)
{
	if(up) Lib_CallLightUp[floor-1] = s;
	else Lib_CallLightDown[floor-1] = s;
	// 向mfc发送消息，更新电梯内外按钮灯状态，参数 2: 表示电梯外Up/Down按钮灯
	postToMfc(3,floor,s,up);
}

/** 
 * 返回门外Up/Down灯（Call Light）的状态
 * 参数floor: 表示楼层
 * 参数up: true,表示返回Up(上升)按钮灯;false,表示返回Down(下降)按钮灯
 * 返回: true,按钮灯亮，false，按钮灯灭
 */
bool GetCallLight(int floor, bool up)
{
	if(up) return Lib_CallLightUp[floor-1];
	else return Lib_CallLightDown[floor-1];
}

/********************************************
 * 电梯箱体门(Doors)
 ********************************************/

/**
 * 开启/关闭电梯箱体门
 * 参数: floor, 表示所在楼层
 *       open, true：表示开门，false：表示关门
 */
void SetDoor(int floor, bool open)
{
	// 不应在此判断，应该在状态函数中判断
	// 如果门是打开的，就不用再打开了; 如果门是关闭的，就不用再关闭了
    //if ((IsDoorOpen(floor) && open) || (IsDoorClosed(floor) && !open)) return;

	// 发送开关门消息
	OpenCloseDoor(floor, open);
}

/** 
 * 返回门是否是打开的，true表示门是打开的，否则门是关闭的或者正在打开/关闭
 * 参数floor: 表示所在的楼层
 */
bool IsDoorOpen(int floor)
{
	// 表示开门过程结束，门是开的
	return Lib_DoorOpened;
}

/** 
 * 返回门是否是关闭的，true关闭的，否则门是打开的或者正在打开/关闭
 * 参数floor: 表示所在的楼层
 */
bool IsDoorClosed(int floor)
{
	return Lib_DoorClosed;
}


/**************************************************
 * 电机控制（Motor control）
 **************************************************/

/** 
 * 刹车，暂时未用
 * 参数b，true：表示使能刹车，否则，false：表示不刹车
 */
void SetBrake(bool b)
{

}

/**
 * 设置电机功率
 * 参数power在-1和1之间。-1：最大速度下降; 1：最大速度上升; 0： 停止
 */
void SetMotorPower(double power)
{
	Lib_Power = power;
}

/** 
 * 获取电梯位置(m),即电梯下边沿距离地面的距离。
 * 电梯停留在一层，返回0;
 * 电梯上升/下降在1、2楼中间,返回0.5;
 * 电梯停留在二层，返回1*Lib_FloorSpacing;
 * 电梯上升/下降在2、3楼中间,返回1.5*Lib_FloorSpacing;
 * 电梯停留在三层，返回2*Lib_FloorSpacing; 
 * 调用举例：
 if(GetPosition() > FloorSpacing) {
       printf("The elevator is above the second floor\n");
   }
 */
double GetPosition()
{
	return Lib_CurrentCarPosition;
}

/** 
 * 获取电梯的速度，>0上升，<0下降，暂时未用 
 */
double GetVelocity()
{
	return Lib_CurrentCarVelocity;
}

/**
 * 返回当前电梯所在层数，是浮点数，因此可以表示楼层之间。
 * 例如，1.5，表示电梯箱体处在1层到2层的中间
 * 等效于: GetPosition() / FloorSpacing + 1
 */
double GetFloor()
{
	return (GetPosition() / Lib_FloorSpacing + 1);
}

/** 
 * 获取当前楼层或最近的楼层，即GetFloor()的最近整数，即它的四舍五入值
 */
int GetNearestFloor()
{
	//printf("%f,%f,%f,%f\n",floor(1.2),ceil(1.2),floor(1.5),ceil(1.5));     // 1,2,1,2
	//printf("%f,%f,%f,%f\n",floor(-1.2),ceil(-1.2),floor(-1.5),ceil(-1.5)); // -2,-1,-2,-1
	return (int)floor(GetFloor()+0.5); // 四舍五入
	//return (int)GetFloor();
}

/**
 * 改变乘客(passenger)数量
 * 当每次开门后，是改变乘客数量的机会。测试时，产生一个随机数，代表乘客数量。
 * 暂时没用
 */
void ChangeLoading()
{

}

/** 
 * 设置乘客数量，暂时没用
 *
 */
void SetLoading(int r)
{

}

 /************************************************************************
 * 静态监测
 * 电梯处于空闲状态, 根据电梯门内楼层按钮和门外各个up/down呼叫按钮,
 * 确定下一步的运动方向和所到楼层(目标楼层)
 *  参数：up 当返回值>0时，下一步电梯的运动方向，true表示向上，false表示向下
 *  返回目标楼层，否则返回-1,表示没有检测到目标楼层
 * 调用举例:
 * int floor; bool up;
 * floor = IdleWhatFloorToGoTo(&up);
 ************************************************************************/
int IdleWhatFloorToGoTo(bool *up)
{
	int ret;
	bool goingUp = IsgoingUp();

	*up = goingUp;
	if(goingUp) {  // 向上
		ret = IdleGoingUpToFloor();
		if(ret < 0) { // 只能改变方向转而向下
			ret = IdleGoingDownToFloor(); 
			if (ret > 0){
				*up = !goingUp;
				Lib_goingUp = *up;
			}
		}
	}
	else {  //向下
		ret = IdleGoingDownToFloor();
		if(ret < 0) { // 只能改变方向转而向上
			ret = IdleGoingUpToFloor(); 
			if (ret > 0) {
				*up = !goingUp;
				Lib_goingUp = *up;
			}
		}
	}

	if(ret > 0) ASSERT(ret>=1 && ret<=Lib_FloorNum);  // 确保返回合法的楼层或-1

	Lib_WillToFloor = ret;
	return ret;
}

/************************************************************************
 * 静态检测
 * 电梯处于空闲状态,检查：上行要到的楼层(目标楼层)，否则返回-1
 ***********************************************************************/
int IdleGoingUpToFloor()
{
	int floor,ret1, ret2,ret;

	// 已经是最高楼了, 不用再上了
	if (GetNearestFloor() == Lib_FloorNum)
	{
		return -1;
	}

	ret1 = -1, ret2 = -1,ret = -1;
	// 检查门内楼层按钮，当前楼层以【上】是否有要到的楼层
	for(floor = GetNearestFloor() + 1; floor <= Lib_FloorNum; floor++) {
		if(GetPanelFloorLight(floor)) { ret1 = floor; break; }
	}
	// 检查门外Up按钮（Call Light）,当前楼层以【上】是否有请求
	for(floor = GetNearestFloor() + 1; floor <= Lib_FloorNum; floor++) {
		if(GetCallLight(floor,true)) { ret2 = floor; break; }
	}
	// 如果以上满足，取最近者（小者）
	if (ret1 > 0 && ret2 > 0) ret = ret1 < ret2 ? ret1 : ret2;
	else if(ret1 > 0 && ret2 < 0) ret = ret1;
	else if(ret1 < 0 && ret2 > 0) ret = ret2;
	else ret = -1; // 都不满足
	if(ret > 0) return ret;

	// 检查门外Down按钮（Call Light）,当前楼层以【上】是否有请求
	for(floor = GetNearestFloor() + 1; floor <= Lib_FloorNum; floor++) {
		if(GetCallLight(floor,false)) { ret = floor; break; }
	}

	if(ret > 0) ASSERT(ret>=1 && ret<=Lib_FloorNum);  // 确保返回合法的楼层或-1

	return ret;
}

/************************************************************************
 * 静态监测
 * 电梯处于空闲状态,检查：下行要到的楼层(目标楼层)，否则返回-1
 ***********************************************************************/
int IdleGoingDownToFloor()
{
	int floor,ret1, ret2, ret;

	// 已经是1楼了, 不用再下了
	if (GetNearestFloor() == 1)
	{
		return -1;
	}

	ret1 = -1, ret2 = -1; ret = -1;
	// 检查门内按钮，当前楼层以【下】是否有要到的楼层
	for(floor = GetNearestFloor() - 1; floor >= 1; floor--) {
		if(GetPanelFloorLight(floor)) { ret1 = floor; break; }
	}
	// 检查门外Down按钮（Call Light）,当前楼层以【下】是否有请求
	for(floor = GetNearestFloor() - 1; floor >= 1; floor--) {
		if(GetCallLight(floor,false)) { ret2 = floor; break; }
	}

	// 如果以上满足，取最近者(大者)
	if (ret1 < 0 && ret2 < 0) ret = -1;
	else return (ret1 > ret2 ? ret1 : ret2);

	// 检查门外Up按钮（Call Light）,当前楼层以【下】是否有请求
	for(floor = GetNearestFloor() -1; floor >= 1; floor--) {
		if(GetCallLight(floor,true)) { ret = floor; break; }
	}

	if(ret > 0) ASSERT(ret>=1 && ret<=Lib_FloorNum);  // 确保返回合法的楼层或-1

	return ret;
}

/************************************************************************
 * 动态监测, 电梯正在上升时，检测将要到达停止的最近楼层(目标楼层)
 * 返回值：最近的目标楼层; 或者-1，表示没有检测到最近的目标楼层。
 * 电梯正在上行,在当前楼层和上一层之间的一半高度以下，检查是否上一楼层是要到的楼层
 * 如果过了一半，就不检查啦，返回原来存储的值。因为过了一半，就没有时间让直流电机停止啦。
 * 这里的当前楼层指，刚刚上行经过的楼层，即(int)GetFloor()返回的楼层
 * ------- 3Floor
 * --- GetFloor(),2.5
 * ------- 2Floor, (int)GetFloor()  当前层
***********************************************************************/
int GoingUpToFloor()
{
	// int floor =  GetNearestFloor(); // 使用四舍五入形式的GetNearestFloor()，判断复杂
	int floor =  (int)GetFloor(); // 当前楼层，即刚刚经过的楼层

	// 显示上行经过的楼层
	if(fabs(GetFloor() - floor) < Lib_FloorTolerance) {
		CString status;
		status.Format(_T("向上,[%d]楼"),floor);
		ViewStatus(status);
		// 如果当前层就是要到的楼层赶快返回
		if(Lib_WillToFloor == floor) return Lib_WillToFloor;
	}

	// 如果上一层就是要到的楼层,就不用判断了。
	if(Lib_WillToFloor == floor + 1) return Lib_WillToFloor;

	// 如果过了一半，就不检查啦，返回原来存储的值。因为过了一半，就没有时间让直流电机停止啦。
	if(GetFloor() - floor > 0.5) return Lib_WillToFloor;

	// 检查门内楼层按钮，当前楼层的上一层是否是要到的楼层
	floor++; 
	ASSERT(floor>=1 && floor<=Lib_FloorNum); // 断言此时绝对没有越界的情况发生

	if(GetPanelFloorLight(floor))  { Lib_WillToFloor = floor; }
	// 检查门外Up按钮（Call Light）,当前楼层的上一层是否有请求
	else if(GetCallLight(floor,true)) { Lib_WillToFloor = floor; }
	// 当前楼层以上没有要到的楼层，只能检查当前楼层的上一层的down按钮
	// 检查门外Down按钮（Call Light）,当前楼层的上一层是否有请求
	else if(Lib_WillToFloor < floor && GetCallLight(floor,false)) { Lib_WillToFloor = floor; }

	if(Lib_WillToFloor > 0) ASSERT(Lib_WillToFloor>=1 && Lib_WillToFloor<=Lib_FloorNum);  // 确保返回合法的楼层或-1

	// 如果没有改变，返回原来存储的值。
	return Lib_WillToFloor;
}

/************************************************************************
 * 动态监测, 电梯正在下降时，检测将要到达停止的最近楼层(目标楼层),否则返回-1
 * 返回值：最近的目标楼层; 或者-1，表示没有检测到最近的目标楼层。
 * 特别提示：自动下降到1楼的情况：
 * AutoTimerDuration(10s)后无动作，自动下降到1楼，最终返回1。
 * 无动作，自动回到MovingDown状态，开始时，此函数返回-1，随后周期调用，由于电机是向下运动，最终就返回1。
 *
 * 电梯正在下行,在当前楼层和下一层之间的一半高度以上，检查是否下一楼层是要到的楼层
 * 如果过了一半，就不检查啦，返回原来存储的值。因为过了一半，就没有时间让直流电机停止啦。
 * 这里的当前楼层指，刚刚下行经过的楼层，即(int)GetFloor()返回的楼层 + 1
 * ------- 3Floor 当前层
 * --- GetFloor(), 2.5
 * ------- 2Floor, (int)GetFloor()
 ***********************************************************************/
int GoingDownToFloor()
{
	//int floor =  GetNearestFloor(); // 使用四舍五入形式的GetNearestFloor()，判断复杂
	int floor =  (int)GetFloor(); // 当前楼层的下一层，即刚刚经过的楼层是floor+1
	
	// 显示下行经过的楼层
	if(fabs(GetFloor() - floor) < Lib_FloorTolerance) {
		CString status;
		status.Format(_T("向下,[%d]楼"),floor);
		ViewStatus(status);
		// 如果到1楼，立即返回1. 适用于10s后无动作，自动下降到1楼的情况
		if(floor == 1) { Lib_WillToFloor = floor; return floor; }
	}

	// 如果floor就是要到的楼层,就不用判断了。
	if(Lib_WillToFloor == floor) return Lib_WillToFloor;

	// 如果过了一半，就不检查啦，返回原来存储的值。因为过了一半，就没有时间让直流电机停止啦。
	if(GetFloor() - floor < 0.5) return Lib_WillToFloor;

	// 检查门内楼层按钮，当前楼层的下一层是否是要到的楼层
	if(GetPanelFloorLight(floor))  { Lib_WillToFloor = floor; }
	// 检查门外Down按钮（Call Light）,当前楼层的下一层是否有请求
	else if(GetCallLight(floor,false)) { Lib_WillToFloor = floor; }
	// 当前楼层以下没有要到的楼层，只能检查当前楼层的下一层的up按钮
	// 检查门外up按钮（Call Light）,当前楼层的下一层是否有请求
	else if(Lib_WillToFloor >= floor && GetCallLight(floor,true)) { Lib_WillToFloor = floor; }

	if(Lib_WillToFloor > 0) ASSERT(Lib_WillToFloor>=1 && Lib_WillToFloor<=Lib_FloorNum);  // 确保返回合法的楼层或-1

	// 如果没有改变，返回原来存储的值。
	return Lib_WillToFloor;
}

/**
* 向mfc发送消息,更新电梯内外按钮灯的状态
* int type;  // 1: 表示电梯内楼层按钮灯(Panel Floor Light), 2: 表示电梯内开关门按钮灯, 3: 表示电梯外Up/Down按钮灯(Call Light)
* int floor; // 楼层号, type=2时,floor无意义
* bool LightOn; // true: 按钮灯On， false: 按钮能Off
* bool up;      // true: Up按钮灯，false: Down按钮灯; 电梯外Up/Down有此项                                                                 
*/
void postToMfc(int type,int floor,bool LightOn,bool up) 
{
	Light_Msg *msg = new Light_Msg();
	msg->type = type;
	msg->floor = floor;
	msg->up = up;
	msg->LightOn = LightOn;
	::PostMessage(CElevator_dialogDlg::MAIN_WIN,WM_LIGHT_MESSAGE,0,(LPARAM)msg);
}

// 向mfc发送消息,显示当前状态
void ViewStatus(CString status)
{
	CString *msg =new CString(status);
	::PostMessage(CElevator_dialogDlg::MAIN_WIN,WM_Status_MESSAGE,0,(LPARAM)msg);
}

// 向mfc发送消息,打开/关闭电梯门
// floor: 表示所在楼层
// Open = true;开门，否则关门
void OpenCloseDoor(int floor,bool Open)
{
	::PostMessage(CElevator_dialogDlg::MAIN_WIN,WM_Door_MESSAGE,(WPARAM) floor,(LPARAM)Open);
}
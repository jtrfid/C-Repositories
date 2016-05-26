/*****************************************************************
 * Elevator Simulator 电梯状态机仿真程序
 * 以MFC为工具，图形动画展示三层电梯仿真运行, 提供电机控制，位置感知，按钮点击功能
 * 本文件是电梯状态机仿真程序头文件
 * (1) 定义状态机状态常量
 * (2) 库函数说明
 ******************************************************************/
#pragma once
#ifndef ELEVATORLIB_H_
#define ELEVATORLIB_H_

// c++程序调用c语言成分的标志，.c文件不能有此标志，.cpp文件中此标志包含的部分表示的是C语言成分
#ifdef __cplusplus
extern "C" {
#endif

/*
 * 常量(Constants)
 */

/**
 * 楼间距(m)，即楼层间的高度
 */
#define Lib_FloorSpacing 3.28

/** 
 * 判断是否到目标楼层的容许误差
 * 判断是否下降到目标楼层：
 * fabs(GetFloor() - GoingDownToFloor() < Lib_FloorTolerance
 * 判断是否上升到目标楼层：
 * fabs(GetFloor() - GoingUpToFloor() < Lib_FloorTolerance
 */
//#define Lib_FloorTolerance 0.16
#define Lib_FloorTolerance 0.01

// 楼层数
#define Lib_FloorNum 3

// 电梯箱体下边沿距离地面的最大相对高度，即到达最顶层时，电梯箱体下边沿距离地面的相对高度
#define Lib_MaxCarPosition (Lib_FloorNum-1)*Lib_FloorSpacing

// 向mfc发送消息,更新电梯内外按钮灯的状态
#define WM_LIGHT_MESSAGE (WM_USER+100)
typedef struct  
{
	int type;     // 1: 表示电梯内楼层按钮灯(Panel Floor Light), 2: 表示电梯内开关门按钮灯, 3: 表示电梯外Up/Down按钮灯(Call Light)
	int floor;    // 楼层号, type=2时,floor无意义
	bool LightOn; // true: 按钮灯On， false: 按钮能Off
	bool up;      // true: Up按钮灯，false: Down按钮灯; 电梯外Up/Down有此项，type=1,2时,此项无意义
}Light_Msg;

// 向mfc发送消息,更新电梯内外按钮灯的状态
extern void postToMfc(int type,int floor,bool LightOn,bool up);

// 向mfc发送消息,显示当前状态
#define WM_Status_MESSAGE (WM_USER+200)
extern void ViewStatus(CString status);

// 向mfc发送消息,打开/关闭电梯门
#define WM_Door_MESSAGE (WM_USER+300)

// floor: 表示所在楼层
// Open = true;开门，否则关门
extern void OpenCloseDoor(int floor,bool Open);

/************************************************************************/
/* 电梯箱体状态                                                          */
/************************************************************************/
#define Idle        1
#define MovingUp    2
#define MovingDown  3
#define DoorOpen    4
#define DoorClosing 5

/*******************************************************************************
 * 说明在ElevatorLib.cpp中定义的全局变量，作为Lib与mfc之间的共享的变量，进行信息传递。
 *******************************************************************************/
extern bool Lib_Running;       // 正在运行仿真: true; 否则false
// 电梯箱体下边沿距离地面的相对高度,初始值0
extern double Lib_CurrentCarPosition;
// 电梯箱体速度，Up>0,Down<0
extern double Lib_CurrentCarVelocity;
// 电机功率，-1.0到1.0之间，设为1.0,电梯箱体以最大速度上升;设为-1.0,电梯箱体以最大速度下降;初始值0.0，表示停止。
extern double Lib_Power;
// 门内: 开、关门按钮灯
extern bool Lib_OpenDoorLight; 
extern bool Lib_CloseDoorLight;
// 门内: 楼层按钮灯(Panel Floor Light),数组元素对应门内楼层数字按钮的状态,注意下标0 -- Lib_FloorNum-1
extern bool Lib_PanelFloorLight[Lib_FloorNum];
// 门外: Up/Down按钮灯(Call Light),数组元素对应各楼层门外Up/Down按钮的状态,注意下标0 -- Lib_FloorNum-1
extern bool Lib_CallLightUp[Lib_FloorNum];
extern bool Lib_CallLightDown[Lib_FloorNum];

// 空闲状态，此时电梯是静止的，电梯当前运动趋势是上升(true)还是下降(false)
// IsgongUp()函数中，如果在1楼，Lib_gongUp=true; 如果在最高楼(Lib_FloorNum)，Lib_gongUp=false；
// IdleWhatFloorToGoTo()中维护该值
// 在elevator.cpp中读取该值，函数IsgoingUp()返回该值。
extern bool Lib_goingUp;
extern bool IsgoingUp();

// 目标楼层，即下一步将要到的楼层，初始值-1
extern int Lib_WillToFloor;

// 开关门定时器已经启动，表示正在开关门,SetDoor()读取，mfc维护
extern bool Lib_DoorTimerStarted;
// 开门结束,初始化为false,在mfc中维护
extern bool Lib_DoorOpened;
// 关门结束,初始化为true,在mfc中维护
extern bool Lib_DoorClosed;

// 一定时间（AutoTimerDuration）无动作，自动下降到1楼,该变量确定定时器是否启动
extern bool Lib_AutoTimerStarted;
extern int AutoTimerDuration;  // ms，时长，缺省10000ms,即10s
// 一定时间无动作，自动到1楼
extern void AutoTo1Floor();
// 取消自动到1楼
extern void CancelTo1Floor();
// AutoTimerDuration时间到，自动执行到1楼的动作，在mfc定时器回调函数中被调用
extern void To1Floor(int *state);

/************************************************************************
 * 在elevator.cpp中定义，实现状态机代码。mfc中调用此函数。
 * 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
 ************************************************************************/
extern void main_control(int *state);

/** 
 * 仿真是否在运行，返回true，运行，否则没有运行
 */
extern bool IsElevatorRunning();

/** 
 * 启动仿真，在初始化时启动
 */
extern void ElevatorStartup();

/** 
 * 结束仿真
 */
extern void ElevatorShutdown();

/******************************************************************
 * 按钮灯(Lights): 
 * （1）门内开关门按钮灯；（2）门内楼层按钮灯（Panel Floor Light）；
 * （3）门外Up/Down按钮（Call Light）
 ******************************************************************/

/** 
 * 设置门内关门按钮灯的状态
 * 参数s: true,按钮灯亮，false，按钮灯灭
 */
extern void SetCloseDoorLight(bool s);

/** 
 * 返回门内关门按钮灯的状态
 * true,按钮灯亮，false，按钮灯灭
 */
extern bool GetCloseDoorLight();

/** 
 * 设置门内开门按钮灯的状态
 * 参数s: true,按钮灯亮，false，按钮灯灭
 */
extern void SetOpenDoorLight(bool s);

/** 
 * 返回门内开门按钮灯的状态
 * true,按钮灯亮，false，按钮灯灭
 */
extern bool GetOpenDoorLight();

/** 
 * 设置门内楼层按钮灯（Panel Floor Light）的状态
 * 参数floor: 表示楼层
 * 参数s: true,按钮灯亮，false，按钮灯灭
 */
extern void SetPanelFloorLight(int floor, bool s);

/** 
 * 返回门内楼层按钮灯（Panel Floor Light）的状态
 * 参数floor: 表示楼层
 * 返回：true,按钮灯亮，false，按钮灯灭
 */
extern bool GetPanelFloorLight(int floor);

/** 
 * 设置门外Up/Down灯（Call Light）的状态
 * 参数floor: 表示楼层
 * 参数up: 表示设置Up(上升)按钮灯
 * 参数s: true,按钮灯亮，false，按钮灯灭
 */
extern void SetCallLight(int floor, bool up, bool s);

/** 
 * 返回门外Up/Down灯（Call Light）的状态
 * 参数floor: 表示楼层
 * 参数up: 表示返回Up(上升)按钮灯
 * 返回: true,按钮灯亮，false，按钮灯灭
 */
extern bool GetCallLight(int floor, bool up);

/********************************************
 * 电梯箱体门(Doors)
 ********************************************/

/**
 * 开启/关闭电梯箱体门
 * 参数: floor, 表示所在楼层
 *       open, true：表示开门，false：表示关门
 */
extern void SetDoor(int floor, bool open);

/** 
 * 返回门是否是打开的，true表示门是打开的，否则门是关闭的或者正在打开/关闭
 * 参数floor: 表示所在的楼层
 */
extern bool IsDoorOpen(int floor);

/** 
 * 返回门是否是关闭的，true关闭的，否则门是打开的或者正在打开/关闭
 * 参数floor: 表示所在的楼层
 */
extern bool IsDoorClosed(int floor);

/**************************************************
 * 电机控制（Motor control）
 **************************************************/

/** 
 * 刹车，暂时未用
 * 参数b，true表示使能刹车，否则不刹车
 */
extern void SetBrake(bool b);

/**
 * 设置电机功率
 * 参数power在-1和1之间。-1：最大速度下降; 1：最大速度上升; 0： 停止
 */
extern void SetMotorPower(double power);

/** 
 * 获取电梯位置(m),即电梯下边沿距离地面的距离。
 * 电梯停留在一层，返回0;
 * 电梯上升/下降在1、2楼中间,返回0.5;
 * 电梯停留在二层，返回1*Lib_FloorSpacing;
 * 电梯上升/下降在2、3楼中间,返回1.5*Lib_FloorSpacing;
 * 电梯停留在三层，返回2*Lib_FloorSpacing; 
 * 电梯停留在三层，返回2*Lib_FloorSpacing; 
 * 调用举例：
 if(GetPosition() > FloorSpacing) {
       printf("The elevator is above the second floor\n");
   }
 */
extern double GetPosition();

/** 
 * 获取电梯的速度，>0上升，<0下降，暂时未用 
 */
extern double GetVelocity();

/**
 * 返回当前电梯所在层数，是浮点数，因此可以表示楼层之间。
 * 例如，1.5，表示电梯箱体处在1层到2层的中间
 * 等效于: GetPosition() / FloorSpacing + 1
 */
extern double GetFloor();

/** 
 * 获取GetFloor()的最近整数，即它的四舍五入值
 */
extern int GetNearestFloor();

/**
 * 改变乘客(passenger)数量
 * 当每次开门后，是改变乘客数量的机会。测试时，产生一个随机数，代表乘客数量。
 * 暂时没用
 */
extern void ChangeLoading();

/** 
 * 设置乘客数量，暂时没用
 *
 */
extern void SetLoading(int r);

/************************************************************************
 * 静态监测
 * 电梯处于空闲状态, 确定下一步的运动方向和所到楼层(目标楼层)
 *  参数：up 当返回值>0时，下一步电梯的运动方向，true表示向上，false表示向下
 *  返回目标楼层，否则返回-1
 * 调用举例:
 * int floor; bool up;
 * floor = IdleWhatFloorToGoTo(&up);
 ************************************************************************/
extern int IdleWhatFloorToGoTo(bool *up);

/************************************************************************
 * 静态检测
 * 电梯处于空闲状态,检查：上行要到的楼层(目标楼层)，否则返回-1
 ***********************************************************************/
extern int IdleGoingUpToFloor();

/************************************************************************
 * 静态监测
 * 电梯处于空闲状态,检查：下行要到的楼层(目标楼层)，否则返回-1
 ***********************************************************************/
extern int IdleGoingDownToFloor();

/************************************************************************
 * 动态监测, 电梯正在上升时，检测将要到达停止的最近楼层(目标楼层)
 * 电梯正在上行,在当前楼层和上一层之间的一半高度以下，检查是否上一楼层是要到的楼层
 * 如果过了一半，就不检查啦，返回原来存储的值。因为过了一半，就没有时间让直流电机停止啦。
 * 这里的当前楼层指，刚刚上行经过的楼层，即(int)GetFloor()返回的楼层
 * ------- 3Floor
 * --- GetFloor()
 * ------- 2Floor, (int)GetFloor()  当前层
 ***********************************************************************/
extern int GoingUpToFloor();

/************************************************************************
* 动态监测, 电梯正在下降时，检测将要到达停止的最近楼层(目标楼层),或返回-1
*
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
extern int GoingDownToFloor();

#ifdef __cplusplus
}
#endif

#endif /* ELEVATORLIB_H_ */

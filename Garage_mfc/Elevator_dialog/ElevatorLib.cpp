#include "stdafx.h"
#include <math.h>

#include "ElevatorLib.h"
#include "resource.h"
#include "Elevator_dialogDlg.h"

bool Lib_Running = false; // 正在运行仿真: true; 否则false
// 电梯箱体下边沿距离地面的相对高度,初始值0
double Lib_CurrentCarPosition = 0;
// 电梯箱体速度，Up>0,Down<0
double Lib_CurrentCarVelocity = 0;
// 电机功率，-1.0到1.0之间，设为1.0,电梯箱体以最大速度上升;设为-1.0,电梯箱体以最大速度下降。
double Lib_Power = 0.0;
// 门内: 开、关门按钮灯
bool Lib_OpenDoorLight = false; 
bool Lib_CloseDoorLight = false;
// 门内: 楼层按钮灯,数组元素对应门内楼层数字按钮的状态,注意下标0 -- Lib_FloorNum-1
bool Lib_PanelFloorLight[Lib_FloorNum] = {false,false,false};
// 门外: Up/Down按钮灯(Call Light),数组元素对应各楼层门外Up/Down按钮的状态,注意下标0 -- Lib_FloorNum-1
bool Lib_CallLightUp[Lib_FloorNum] = {false,false,false};
bool Lib_CallLightDown[Lib_FloorNum] = {false,false,false};

// 电梯当前运动状态是上升(true)还是下降(false)，
// IsgongUp()函数中，如果在1楼，Lib_gongUp=true; 如果在最高楼，Lib_gongUp=false；
// IdleWhatFloorToGoTo()中维护该值
// 在elevator.cpp中读取该值，函数IsgoingUp()返回该值。
bool Lib_goingUp = true;
bool IsgoingUp() {
	if(GetNearestFloor() == 1) Lib_goingUp = true;
	else if (GetNearestFloor() == Lib_FloorNum) Lib_goingUp = false; 
	return Lib_goingUp;
}

// 将要到的楼层
int Lib_WillToFloor = -1;

// 开关门定时器已经启动，表示正在开关门SetDoor()读取，mfc维护
bool Lib_DoorTimerStarted = false;
// 开门结束,在mfc中维护
bool Lib_DoorOpened = false;
// 关门结束,在mfc中维护
bool Lib_DoorClosed = false;

// 一定时间无动作，自动到1楼,该变量确定定时器是否启动
bool Lib_AutoTimerStarted = false;
// 一定时间无动作，自动到1楼
extern void AutoTo1Floor() 
{
	if (!Lib_AutoTimerStarted)
	{
		SetTimer(CElevator_dialogDlg::MAIN_WIN,ID_AUTO_TIMER,10000,NULL);//10s,10000ms
		Lib_AutoTimerStarted = true;
	}
}
// 取消自动到1楼
extern void CancelTo1Floor() 
{
	if (Lib_AutoTimerStarted)
	{
		KillTimer(CElevator_dialogDlg::MAIN_WIN,ID_AUTO_TIMER);
		Lib_AutoTimerStarted = false;
	}
}
// 10s时间到，自动执行到1楼的动作，在mfc定时器回调函数中被调用
extern void To1Floor(int *state) 
{
	printf("10s时间到，自动下降到1楼...\n");
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

/** \brief Function to determine if the elevator simulator is currently running. */
/**
* This function should be called in a loop that exists when the elevator is
* no longer running. If the elevator window is closed by the user, the calls
* are not longer valid and should not be used (other than ElevatorShutdown).
* \return true if the elevator simulation is currently running.
* \code
while(IsElevatorRunning())
{
ElevatorButtonProcessing();

usleep(1000);
}
\endcode
*/
bool IsElevatorRunning()
{
	return Lib_Running;
}

/** \brief Start the Elevator Simulation.
*
* This function must be called before any other call can be made
* to any Elevator Simulator functions. It starts the elevator simulator
* in a new window.
*/
void ElevatorStartup()
{
	Lib_Running = true;
}

/** \brief Shut down the Elevator Simulation.
*
* This function should be called last in the program. If the simulator is
* currently running, it shuts down the simulator and closes the window. Then
* it releases any resources allocated to this program.
*/
void ElevatorShutdown()
{
	Lib_Running = false;
}

/*
* Lights
*/


/** \brief Set the status of the close door light on the panel in the elevator car.
*
* This function sets the lighted status of the close door button inside the
* elevator car. If set to true, the light is turned on.
* \param s Set to true to run the light on
* \code
SetCloseDoorLight(true);
\endcode
*/
void SetCloseDoorLight(bool s)
{
	Lib_CloseDoorLight = s;
	// 向mfc发送消息，更新电梯内外按钮灯状态，参数 2: 表示电梯内开关门按钮灯
	postToMfc(2,0,s,0);
}

/** \brief Get the status of the close door light on the panel in the elevator car.
*
* This functions checks to see if the close door light on the panel in the elevator
* car is on.
* \return true if the light is on.
* \code
if(GetCloseDoorLight())
{
do something here...
}
\endcode
*
*/
bool GetCloseDoorLight()
{
	return Lib_CloseDoorLight;
}

/** \brief Set the status of the open door light on the panel in the elevator car.
*
* This function sets the lighted status of the open door button inside the
* elevator car. If set to true, the light is turned on.
* \param s Set to true to run the light on
* \code
SetOpenDoorLight(true);
\endcode
*/
void SetOpenDoorLight(bool s)
{
	Lib_OpenDoorLight = s;
	// 向mfc发送消息，更新电梯内外按钮灯状态，参数 2: 表示电梯内开关门按钮灯
	postToMfc(2,0,s,0);
}

/** \brief Get the status of the open door light on the panel in the elevator car.
*
* This functions checks to see if the open door light on the panel in the elevator
* car is on.
* \return true if the light is on.
* \code
if(GetOpenDoorLight())
{
do something here...
}
\endcode
*
*/
bool GetOpenDoorLight()
{
	return Lib_OpenDoorLight;
}


/** \brief Set the status of the floor select light on the panel in the elevator car.
*
* This function sets the lighted status of the floor select button inside the
* elevator car. If set to true, the light is turned on.
* \param floor The floor number. 1, 2, or 3.
* \param s Set to true to run the light on
* \code
SetPanelFloorLight(1, true);
\endcode
*/
void SetPanelFloorLight(int floor, bool s)
{
	Lib_PanelFloorLight[floor-1] = s;
	// 向mfc发送消息，更新电梯内外按钮灯状态，参数 1: 表示电梯内楼层按钮灯(Panel Floor Light)
	postToMfc(1,floor,s,0);
}

/** \brief Get the status of the floor select light on the panel in the elevator car.
*
* This functions checks to see if the floor select button on the panel in the elevator
* car is currently lit.
* \param floor The floor number. 1, 2, or 3.
* \return true if the light is on.
* \code
if(GetPanelFloorLight(1))
{
do something here...
}
\endcode
*
*/
bool GetPanelFloorLight(int floor)
{
	return Lib_PanelFloorLight[floor-1];
}

/** \brief Set the status of a call button light for a floor.
*
* This function sets the lighted status of a call button on a floor.
* Each floor has an up and down call button. This sets the lighting of
* that button on or off.
* \param floor The floor number. 1, 2, or 3.
* \param up If true, the up button is set. If false, the down button is set.
* \param s Set to true to turn the light on
* \code
SetCallLight(2, true, true);
\endcode
*/
void SetCallLight(int floor, bool up, bool s)
{
	if(up) Lib_CallLightUp[floor-1] = s;
	else Lib_CallLightDown[floor-1] = s;
	// 向mfc发送消息，更新电梯内外按钮灯状态，参数 2: 表示电梯外Up/Down按钮灯
	postToMfc(3,floor,s,up);
}

/** \brief Get the status of a call button light for a floor.
*
* This functions checks to see if an elevator call button for a
* floor is currently lit. Each floor has an up and down call button.
* This function controls the lighting of those buttons.
* \param floor The floor number. 1, 2, or 3.
* \param up If true, the up button is read. If false, the down button is read.
* \return true if the light is on.
* \code
if(GetCallLight(1, false))
{
do something here...
}
\endcode
*
*/
bool GetCallLight(int floor, bool up)
{
	if(up) return Lib_CallLightUp[floor-1];
	else return Lib_CallLightDown[floor-1];
}

/** \brief Set the status of a door indicator for a floor.
*
* This function sets the lighted status of a door indicator on a floor.
* Each floor has an up and down arrow above the elevator door. This light
* is normally used to indicate when the elevator has arrived and which
* direction it is going. This function turns those indicators on and off.
* \param floor The floor number. 1, 2, or 3.
* \param up If true, the up arrow is set. If false, the down arrow is set.
* \param s Set to true to turn the light on
* \code
SetCallLight(2, true, true);
\endcode
*/
void SetDoorIndicator(int floor, bool up, bool s)
{

}

/** \brief Get the status of a door indicator light for a floor.
*
* This functions checks to see if a door indicator for a
* floor is currently lit. Each floor has an up and down arrow above the elevator door. This light
* is normally used to indicate when the elevator has arrived and which
* direction it is going. This function checks to see if those indicators are on or off.
* \param floor The floor number. 1, 2, or 3.
* \param up If true, the up indicator is read. If false, the down indicator is read.
* \return true if the light is on.
* \code
if(GetDoorIndicator((3, false))
{
do something here...
}
\endcode
*
*/
bool GetDoorIndicator(int floor, bool up)
{
	return 0;
}

/*
* Doors
*/

/** \brief Set the opening/closing status of the elevator doors.
*
* The elevator doors have automatic control hardware that opens
* and closes then based on this call. If you call this function with
* open set true, the door will begin to open. That opening takes
* a finite amount of time, so it is not immediately open. If the door
* is closing and this call requests an open, it will reverse and
* begin opening again. The same is true if opening.
*
* \par It is the control system's responsibility to only open doors
* when the elevator is at the floor and not to move the elevator
* while the doors are open.
* \param floor The floor number. 1, 2, or 3.
* \param open true tells the doors to open on that floor.
*/
void SetDoor(int floor, bool open)
{
	if(!Lib_DoorTimerStarted)  // 防止开关门未结束，再次执行
	{
		// 如果门是打开的，就不用再打开了; 如果门是关闭的，就不用再关闭了
		if((IsDoorOpen(floor) && open) || (IsDoorClosed(floor) && !open)) return;
		// 发送开关门消息，mfc开启开关门定时器
		OpenCloseDoor(floor,open);
	}
}

/** \brief Determines if the elevator door is open.
*
* This function returns true if the elevator door is completely open.
* When a door is moving from open to closed or vice versa, this function
* and IsDoorClosed will both return false. Use this function to determine
* when the opening process has completed.
* \param floor The floor number. 1, 2, or 3.
*/
bool IsDoorOpen(int floor)
{
	// 表示开门过程结束，门是开的
	return Lib_DoorOpened;
}

/** \brief Determines if the elevator door is closed.
*
* This function returns true if the elevator door is completely closed.
* When a door is moving from open to closed or vice versa, this function
* and IsDoorOpen will both return false. Use this function to determine
* when the closing process has completed.
* \param floor The floor number. 1, 2, or 3.
*/
bool IsDoorClosed(int floor)
{
	return Lib_DoorClosed;
}

/*
* Button callback management
*/

/** \brief Set a function to be called when the door open button is pressed.
*
* This function installs a handler function that will be called when the
* open door button on the panel inside the elevator is pressed.
*
* There is no default behavior for this button.
* \param handler A C function that will be called when the button is pressed.
* \par Example of how to set the handler in code:
* \code
SetOpenDoorHandler(OnOpenDoor);
\endcode
* \par Example handler function:
* \code
void OnOpenDoor()
{
printf("Open door pressed\n");
}
\endcode
*/
void SetOpenDoorHandler(void (*handler)())
{

}

/** \brief Set a function to be called when the door close button is pressed.
*
* This function installs a handler function that will be called when the
* close door button on the panel inside the elevator is pressed.
*
* There is no default behavior for this button.
* \param handler A C function that will be called when the button is pressed.
* \par Example of how to set the handler in code:
* \code
SetCloseDoorHandler(OnOpenDoor);
\endcode
* \par Example handler function:
* \code
void OnCloseDoor()
{
printf("Close door pressed\n");
}
\endcode
*/
void SetCloseDoorHandler(void (*handler)())
{

}

/** \brief Set a function to be called when a call button on a floor is pressed.
*
* This function installs a handler function that will be called when a elevator
* call button on any floor is pressed.
*
* If no handler is installed, the default behavior for a call button press is
* to light the call button on that floor, with the exception that the up button
* on the top floor and the down button on the bottom floor do nothing. If a
* handler is installed, this default behavior does not occur.
* \param handler A C function that will be called when the button is pressed.
* \par Example of how to set the handler in code:
* \code
SetCallPressedHandler(OnCallPressed);
\endcode
* \par Example handler function:
* \code
void OnCallPressed(int floor, bool up)
{
if(up
{
printf("The up call button has been pressed on floor %d\n", floor);
}
else
{
printf("The down call button has been pressed on floor %d\n", floor);
}
\endcode
*/
void SetCallPressedHandler(void (*handler)(int floor, bool up))
{

}

/** \brief Set a function to be called when a floor button on the elevator car panel is pressed.
*
* This function installs a handler function that will be called when a floor select button
* is pressed on the elevator panel.
*
* If no handler is installed, the default behavior for a floor button press is
* to light the button for that floor on the elevator panel. If a
* handler is installed, this default behavior does not occur.
* \param handler A C function that will be called when the button is pressed.
* \par Example of how to set the handler in code:
* \code
SetFloorPressedHandler(OnFloorPressed);
\endcode
* \par Example handler function:
* \code
void OnFloorPressed(int floor)
{
printf("The elevator panel button has been pressed for floor %d\n", floor);
\endcode
*/
void SetFloorPressedHandler(void (*handler)(int floor))
{

}


/*
* Motor control
*/

/** \brief Set the status of the elevator brake.
*
* This function controls the elevator brake. It called with true, the brake is
* set on. If called with false, the brake is set off. The brake will immediately
* stop the movement of the car unless the car is going faster than 0.33 meters
* per second. If the car is going faster than 0.33 meters per second, indicated
* by the velocity outside the green on the speed indicator, the brake will be
* destroyed and will no longer work.
* \param b The brake status to be set. true enables the brake.
* \code
SetBrake(true);
\endcode
*/
void SetBrake(bool b)
{

}

/** \brief Set the motor power.
*
* The elevator uses a DC motor. This function sets how much power is
* provided to the motor. The values range from -1 to 1. A value of -1
* means maximum velocity down. A value of 1 means maximum velocity up.
* Values outside this range will be bounded to the range [-1, 1]. Motor
* power translates to torque and is not an indicator of speed.
* \param power The power setting in the range -1 to 1.
* \code
SetMotorPower(0.4);
\endcode
*/
void SetMotorPower(double power)
{
	Lib_Power = power;
}

/** \brief Get the absolute position of the elevator in meters.
*
* This function returns the position of the elevator in the shaft in
* meters relative to the first floor. A value
* of zero means the elevator is at the first floor door. The constant FloorSpacing
* indicates the spacing between floors. If this function returns the value in
* FloorSpacing, the elevator is at the second floor. This is a continuous
* value and can be negative if the elevator is below the first floor door.
* \return The current position in meters relative to the first floor door.
* \code
if(GetPosition() > FloorSpacing)
{
printf("The elevator is above the second floor\n");
}
\endcode
*/
double GetPosition()
{
	return Lib_CurrentCarPosition;
}

/** \brief Get the velocity of the elevator in meters per second.
*
* This function returns the current velocity of the elevator car
* in meters per second. A positive value indicates the elevator is
* going up. A negative value indicates the elevator is going down.
* Do not exceed the maximum design velocity of 1 meter per second in
* either direction.
* \return The elevator car velocity in meters per second
* \code
if(fabs(GetVelocity()) < 0.33)
{
printf("It is okay to brake\n");
}
\endcode
*/
double GetVelocity()
{
	return Lib_CurrentCarVelocity;
}

/** \brief Get the current floor the elevator is on as a floating point value.
*
* This function returns the position of the elevator car in floors. A return
* value of 1 indicates the elevator is at the first floor door. A value of 3
* indicates the elevator is on the third floor door. This is a floating
* point value and will indicate locations between floors.
*
* This function return value is equivalent to: GetPosition() / FloorSpacing + 1
* \return The current floor as a floating point value.
*/
double GetFloor()
{
	return (GetPosition() / Lib_FloorSpacing + 1);
}

/** \brief Get the nearest integer floor to the elevator.
*
* This function returns floor the elevator car is nearest as an integer value.
* A return
* value of 1 indicates the elevator is nearest to the first floor. A value of 3
* indicates the elevator is nearest to the third floor.
*
* This function return value is equivalent to the value of GetFloor() rounded to
* the nearest integer.
* \return The nearest floor as an integer value.
*/
int GetNearestFloor()
{
	//printf("%f,%f,%f,%f\n",floor(1.2),ceil(1.2),floor(1.5),ceil(1.5));     // 1,2,1,2
	//printf("%f,%f,%f,%f\n",floor(-1.2),ceil(-1.2),floor(-1.5),ceil(-1.5)); // -2,-1,-2,-1
	return (int)floor(GetFloor()+0.5); // 四舍五入
	//return (int)GetFloor();
}

/** \brief Change the current number of passengers on the elevator.
*
* This function will cause the number of passengers on the elevator to change
* to a new random number. This happens automatically when the door opens on any
* floor, but can also be used to test elevator control code by selecting random
* loadings.
*/
void ChangeLoading()
{

}

/** \brief Set the passenger load for the elevator car.
*
* This function sets the passengers in the elevator car. The passengers are
* represented by a bitwise or of 1 for Noel, 2 for Neil, and 4 for Sparty. To
* turn on all passengers, use a value of 1 | 2 | 4 = 7.
* \param r Binary or of passenger values.
* This code example enables all passengers.
* \code
SetLoading(7);
\endcode
*
*/
void SetLoading(int r)
{

}

/*
* Timer
*/

/** \brief Reset a timer available for program use.
*
* This function, when called, resets a timer to zero. It is equivalent to pressing
* the reset button on a stopwatch. The stopwatch can then be read using the GetTimer
* function. There is only one timer available to an application.
*/
void ResetTimer()
{

}

/** \brief Read the current value of the timer.
*
* This function read the current value of an available timer. The timer can be
* reset using the ResetTimer call. The value read is in second and is a floating point
* value.
* \return Number of seconds since the last time ResetTimer was called.
*/
double GetTimer()
{
	return 0.0;
}



/*
* Button testing help
*/

/** \brief Determine what floor the elevator should be going to.
*
* This function will check the elevator buttons and determine what
* floor the elevator should be going to. When going up, it will look for
* floors above the current floor that have an up call button or
* elevator panel button pressed.
* If so, it will return the nearest one. Otherwise, it will check for floors
* above the current floor with a down button pressed. If it finds one, it
* will return the farthest of these. If it finds no floors in the up direction,
* it repeats the process in the down direction.
*
* The process when going down is similar, mirroring the up and down states.
*
* This function is usually called when the elevator is stopped to determine
* where it should go next.
*
* This function is implemented by calling WhatFloorToGoToInThisDirection
* first with the value of up, then, if no floor is selected, with the value of
* !up.
* \param up If true, call buttons and the elevator buttons are first checked
* in the up direction, then down. If no floors are selected, the function returns
* a value of -1. Return values for floors are 1, 2, or 3.
*/
// int WhatFloorToGoTo(bool up)
/************************************************************************
* 静态监测
* 电梯处于空闲状态, 确定下一步的运动方向和所到楼层
*  参数：up 当返回值>0时，下一步电梯的运动方向，true表示向上，false表示向下
*  返回要到的楼层，否则返回-1
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
			*up = !goingUp;
			Lib_goingUp = *up;
		}
	}
	else {  //向下
		ret = IdleGoingDownToFloor();
		if(ret < 0) { // 只能改变方向转而向上
			ret = IdleGoingUpToFloor(); 
			*up = !goingUp;
			Lib_goingUp = *up;
		}
	}

	if(ret > 0) ASSERT(ret>=1 && ret<=Lib_FloorNum);  // 确保返回合法的楼层或-1

	Lib_WillToFloor = ret;
	return ret;
}

/************************************************************************
* 静态监测
* 电梯处于空闲状态,检查：上行要到的楼层，否则返回-1
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
* 电梯处于空闲状态,检查：下行要到的楼层，否则返回-1
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
* 动态监测, 电梯正在上升时，检测将要到达停止的最近楼层（目标楼层）
* 电梯正在上行,在当前楼层和上一层之间的一半高度以下，检查是否上一楼层是要到的楼层
* 如果过了一半，就不检查啦，返回原来存储的值。因为过了一半，就没有时间让直流电机停止啦。
* 这里的当前楼层指，刚刚上行经过的楼层，即(int)GetFloor()返回的楼层
* ------- 3Floor
* --- GetFloor()
* ------- 2Floor, (int)GetFloor() 当前层
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
* 动态监测, 电梯正在下降时，检测将要到达停止的最近楼层（目标楼层）
* 适用于10s后无动作，自动下降到一楼的情况.
* 电梯正在下行,在当前楼层和下一层之间的一半高度以上，检查是否下一楼层是要到的楼层
* 如果过了一半，就不检查啦，返回原来存储的值。因为过了一半，就没有时间让直流电机停止啦。
* 这里的当前楼层指，刚刚下行经过的楼层，即(int)GetFloor()返回的楼层 + 1
* ------- 3Floor 当前层
* --- GetFloor()
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
		// 如果到1楼，立即返回1. 适用于10s后无动作，自动下降到一楼的情况
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


/** \brief Determine what floor the elevator should be going to when traveling
* in a certain direction.
*
* This function will check the elevator buttons and determine what
* floor the elevator should be going to. When going up, it will look for
* floors above the current floor that have an up call button or
* elevator panel button pressed.
* If so, it will return the nearest one. Otherwise, it will check for floors
* above the current floor with a down button pressed. If it finds one, it
* will return the farthest of these. If it finds no floors in the up direction,
* it returns a value of -1.
*
* The process when going down is similar, mirroring the up and down states.
*
* This function can be continuously called while the elevator is moving to
* determine what floor it should stop on. The function will not return a
* floor that the elevator does not have time to safely stop for. For example, if
* the elevator is going up and is just below the second floor when the call button
* for the second floor is pressed, the function will not return the second
* floor because the elevator could not stop quickly enough to arrive at that floor.

* \param up If true, call buttons and the elevator buttons are checked
* in the up direction. If no floors are selected, the function returns
* a value of -1. Return values for floors are 1, 2, or 3.
*/
int WhatFloorToGoToInThisDirection(bool up)
{
	return 0;
}

/**
* 向mfc发送消息,更新电梯内外按钮灯的状态
* int type;  // 1: 表示电梯内楼层按钮灯, 2: 表示电梯内开关门按钮灯, 3: 表示电梯外Up/Down按钮灯
* int floor; // 楼层号
* bool LightOn; // true: 按钮灯On， false: 按钮能Off
* bool up;   // true: Up按钮灯，false: Down按钮灯; 电梯外Up/Down有此项                                                                 
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
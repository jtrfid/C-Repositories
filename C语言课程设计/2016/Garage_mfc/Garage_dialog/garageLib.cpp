#include "stdafx.h"  // mfc必须的
#include "GarageLib.h"

bool Lib_Running = false;        // 正在运行仿真: true; 否则false
bool Lib_ButtonPressed = false;  // 每次按下车库门开关，Lib_ButtonPressed = true; 在WasButtonPressed()内读取后，置为false
bool Lib_BeamBroken = false;     // 门下部的红外线探测到物体: true,停止关门动作; 否则false
// 当前门的相对位置，门的下边沿距离地面的相对高度。
// 门关闭时<=DoorTolerance, 打开时>=(DoorHeight-DoorTolerance)
double Lib_CurrentDoorPosition = 0.0;  
double Lib_Power = 0.0;   // 直流电机功率，-1.0到1.0之间，设为1.0,车库门以最大速度上升;设为-1.0,车库门以最大速度下降。

/* 系统是否运行 */
bool IsGarageRunning()
{
	return Lib_Running;
}

/* 开始系统仿真 */
void GarageStartup()
{
	Lib_Running = true;
}

/* 结束系统仿真 */
void GarageShutdown()
{
	Lib_Running = false;
}


/**
 是否按键，返回true，表示按键，随后的返回值为false，下次按键又返回true。
 即，保证读取一次按键，随后按键值置为false, 避免主控循环的下一次循环重复处理按键值。
 **/
bool WasButtonPressed()
{
	if (Lib_ButtonPressed) {
		Lib_ButtonPressed = false;
		return true;
	}
	return false;
}


/**
 门的下部红外探测到物体，返回true，否则返回false
 **/
bool IsBeamBroken()
{
	return Lib_BeamBroken;
}

/**
 设置直流电机功率， - 1.0到1.0之间，设为1.0, 车库门以最大速度上升; 设为 - 1.0, 车库门以最大速度下降。
 **/
void SetMotorPower(double power)
{
	Lib_Power = power;
}


/**
 当前门的相对位置，门的下边沿距离地面的相对高度。
 门关闭时<=DoorTolerance, 打开时>=(DoorHeight-DoorTolerance)
 **/
double GetDoorPosition()
{
	return Lib_CurrentDoorPosition;
}

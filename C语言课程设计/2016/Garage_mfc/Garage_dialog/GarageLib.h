/** 
 * 车库门仿真库函数说明
 */

#pragma once
#ifndef GARAGELIB_H_
#define GARAGELIB_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 常数（Constants）
 */


/** 
 门的高度（米，meters）
 */
#define Lib_DoorHeight 2.13

/**
 打开或关闭门的容差（米，meters）
 门关闭时，门的下边沿距离地面的相对高度<=DoorTolerance, 
 打开时, 门的下边沿距离地面的相对高度>=(DoorHeight-DoorTolerance)
 */
#define Lib_DoorTolerance 0.02

/************************************************************************
 状态常数                                     
 ************************************************************************/
#define DoorClosed 1
#define DoorOpening 2
#define DoorOpen 3
#define DoorClosing 4

/*******************************************************************************/
/* 说明在garageLib.cpp中定义的全局变量，作为Lib与mfc之间的共享的变量，进行信息传递。 */
/*******************************************************************************/
extern bool Lib_Running;        // 正在运行仿真: true; 否则false
extern bool Lib_ButtonPressed;  // 每次按下车库门开关，Lib_ButtonPressed = true; 在WasButtonPressed()内读取后，置为false
extern bool Lib_BeamBroken;     // 门下部的红外线探测到物体: true,停止关门动作; 否则false
// 当前门的相对位置，门的下边沿距离地面的相对高度。
// 门关闭时<=DoorTolerance, 打开时>=(DoorHeight-DoorTolerance)
extern double Lib_CurrentDoorPosition;  
extern double Lib_Power;   // 直流电机功率，-1.0到1.0之间，设为1.0,车库门以最大速度上升;设为-1.0,车库门以最大速度下降。

/************************************************************************
 * 在garage.cpp中定义，实现状态机代码
 * 主控循环： 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
 ************************************************************************/
extern void main_control(int *state);

/* 系统是否运行 */
extern bool IsGarageRunning();

/* 开始系统仿真 */
extern void GarageStartup();

/* 结束系统仿真 */
extern void GarageShutdown();


/**
 是否按键，返回true，表示按键，随后的返回值为false，下次按键又返回true。
 即，保证读取一次按键，随后按键值置为false, 避免主控循环的下一次循环重复处理按键值。
 **/
extern bool WasButtonPressed();

/**
 门的下部红外探测到物体，返回true，否则返回false
 **/
extern bool IsBeamBroken();

/**
 设置直流电机功率， - 1.0到1.0之间，设为1.0, 车库门以最大速度上升; 设为 - 1.0, 车库门以最大速度下降。
 **/
extern void SetMotorPower(double power);

/**
 当前门的相对位置，门的下边沿距离地面的相对高度。
 门关闭时<=DoorTolerance, 打开时>=(DoorHeight-DoorTolerance)
 **/
extern double GetDoorPosition();

#ifdef __cplusplus
}
#endif

#endif /* ELEVATORLIB_H_ */

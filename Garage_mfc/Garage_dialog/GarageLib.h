/*
 * Name:     GarageLib.h
 *
 *
 * Class:    GarageLib
 * Created : Mar 5, 2011
 * Author :  Charles B. Owen
 */

/** \file
 * \brief Garage Simulator library include file.
 *
 * This file is included to utilize the Garage Simulator.
 */

#pragma once
#ifndef GARAGELIB_H_
#define GARAGELIB_H_


/**
 *  \mainpage Garage Simulator
 *
 * This is the documentation for the CSE 251 Garage Simulator.
 * This describes the function calls necessary to utilize the
 * simulator. The Garage Simulator is a graphical representation
 * of an elevator in a three story building. Support is provided for
 * motor control, position sensing, button support, and indicators.
 *
 * See the documentation for the GarageLib.h file for all of the
 * function documentation. Applications include this file to gain
 * access to the functions and link to the file libGarageLib.a.
 * A makefile is provided with the project.
 *
 * This documentation is generated from sources using Doxygen.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Constants
 */


/** \brief Height of the door in meters.
 */
#define Lib_DoorHeight 2.13

/** \brief Tolerance for a safe door opening in meters.
 *
 * If the door is this close to open or closed we are happy.
 * We can't open past DoorHeight or close to less than 0.2cm.
 */
#define Lib_DoorTolerance 0.02

/************************************************************************/
/* Our possible garage door states                                      */
/************************************************************************/
#define DoorClosed 1
#define DoorOpening 2
#define DoorOpen 3
#define DoorClosing 4

/*******************************************************************************/
/* 说明在garageLib.cpp中定义的全局变量，作为Lib与mfc之间的共享的变量，进行信息传递。 */
/*******************************************************************************/
extern bool Lib_Running;        // 正在运行仿真: true; 否则false
extern bool Lib_ButtonPressed;  // 按下车库门开关: true; 否则false
extern bool Lib_BeamBroken;     // 门下部的红外线探测到物体: true,停止关门动作; 否则false
// 当前门的相对位置，门的下边沿距离地面的相对高度。
// 门关闭时<=DoorTolerance, 打开时>=(DoorHeight-DoorTolerance)
extern double Lib_CurrentDoorPosition;  
extern double Lib_Power;   // 直流电机功率，-1.0到1.0之间，设为1.0,车库门以最大速度上升;设为-1.0,车库门以最大速度下降。

/************************************************************************/
/*  在garage.cpp中定义，实现状态机代码                                    */
/************************************************************************/
extern void main_control(int *state);

/*
 * Management
 */

/** \brief Function to determine if the garage simulator is currently running. */
/**
 * This function should be called in a loop that exits when the garage is
 * no longer running. If the garage window is closed by the user, the calls
 * are not longer valid and should not be used (other than GarageShutdown).
 * \return true if the garage simulation is currently running.
 * \code
    while(IsGarageRunning())
    {

    }
 \endcode
 */
extern bool IsGarageRunning();

/** \brief Start the Garage Simulation.
 *
 * This function must be called before any other call can be made
 * to any Garage Simulator functions. It starts the garage simulator
 * in a new window.
 */
extern void GarageStartup();

/** \brief Shut down the Garage Simulation.
 *
 * This function should be called last in the program. If the simulator is
 * currently running, it shuts down the simulator and closes the window. Then
 * it releases any resources allocated to this program.
 */
extern void GarageShutdown();


/** \brief Determines of the button has been pressed.
 *
 * This function returns true if the button has been pressed. It resets so that
 * the next call will return false until the button is pressed again.
 * \return true if button was pressed.
 */
extern bool WasButtonPressed();

/** \brief Determines if the infrared beam at the bottom of door is broken.
 *
 * If the beam is broken, someone or something may be under the door, so
 * we can't close it on them.
 * \return true if beam is broken.
 */
extern bool IsBeamBroken();

/*
 * Motor control
 */

/** \brief Set the motor power.
 *
 * The door opener uses a DC motor. This function sets how much power is
 * provided to the motor. The values range from -1 to 1. A value of -1
 * means maximum velocity down. A value of 1 means maximum velocity up.
 * Values outside this range will be bounded to the range [-1, 1].
 * \param power The power setting in the range -1 to 1.
 * \code
      SetMotorPower(1);
 \endcode
 */
extern void SetMotorPower(double power);

/** \brief Get the position of the garage door in meters.
 *
 * This function returns the position of the door to the garage in
 * meters relative to closed. A value
 * of zero means the door is closed.
 * \return The current position in meters relative to closed.
 * \code
     if(GetDoorPosition() > DoorHeight - DoorTolerance)
     {
         printf("The garage door is open\n");
     }
\endcode
 */
extern double GetDoorPosition();

#ifdef __cplusplus
}
#endif

#endif /* ELEVATORLIB_H_ */

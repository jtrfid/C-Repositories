#include "stdafx.h"  // mfc必须的
#include "GarageLib.h"

bool Lib_Running = false;        // 正在运行仿真: true; 否则false
bool Lib_ButtonPressed = false;  // 每次按下车库门开关，Lib_ButtonPressed = !Lib_ButtonPressed;
bool Lib_BeamBroken = false;     // 门下部的红外线探测到物体: true,停止关门动作; 否则false
// 当前门的相对位置，门的下边沿距离地面的相对高度。
// 门关闭时<=DoorTolerance, 打开时>=(DoorHeight-DoorTolerance)
double Lib_CurrentDoorPosition = 0.0;  
double Lib_Power = 0.0;   // 直流电机功率，-1.0到1.0之间，设为1.0,车库门以最大速度上升;设为-1.0,车库门以最大速度下降。

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
bool IsGarageRunning()
{
	return Lib_Running;
}

/** \brief Start the Garage Simulation.
 *
 * This function must be called before any other call can be made
 * to any Garage Simulator functions. It starts the garage simulator
 * in a new window.
 */
void GarageStartup()
{
	Lib_Running = true;
}

/** \brief Shut down the Garage Simulation.
 *
 * This function should be called last in the program. If the simulator is
 * currently running, it shuts down the simulator and closes the window. Then
 * it releases any resources allocated to this program.
 */
void GarageShutdown()
{
	Lib_Running = false;
}

/** \brief Determines of the button has been pressed.
 *
 * This function returns true if the button has been pressed. It resets so that
 * the next call will return false until the button is pressed again.
 * \return true if button was pressed.
 */
bool WasButtonPressed()
{
	static bool LastPress = false; // LastPress=上次按键值,Lib_ButtonPressed=当前按键值
	bool ret = (Lib_ButtonPressed != LastPress);  
	if (ret) LastPress = Lib_ButtonPressed;  // 检查已经按键，然后复位，下次调用时，返回false;
	return ret;
}

/** \brief Determines if the infrared beam at the bottom of door is broken.
 *
 * If the beam is broken, someone or something may be under the door, so
 * we can't close it on them.
 * \return true if beam is broken.
 */
bool IsBeamBroken()
{
	return Lib_BeamBroken;
}

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
void SetMotorPower(double power)
{
	Lib_Power = power;
}


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
double GetDoorPosition()
{
	return Lib_CurrentDoorPosition;
}

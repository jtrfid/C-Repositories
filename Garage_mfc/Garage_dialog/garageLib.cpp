#include "stdafx.h"  // mfc�����
#include "GarageLib.h"

bool Lib_Running = false;        // �������з���: true; ����false
bool Lib_ButtonPressed = false;  // ÿ�ΰ��³����ſ��أ�Lib_ButtonPressed = !Lib_ButtonPressed;
bool Lib_BeamBroken = false;     // ���²��ĺ�����̽�⵽����: true,ֹͣ���Ŷ���; ����false
// ��ǰ�ŵ����λ�ã��ŵ��±��ؾ���������Ը߶ȡ�
// �Źر�ʱ<=DoorTolerance, ��ʱ>=(DoorHeight-DoorTolerance)
double Lib_CurrentDoorPosition = 0.0;  
double Lib_Power;              // ֱ��������ʣ�-1.0��1.0֮�䣬��Ϊ1.0,������������ٶ�����;��Ϊ-1.0,������������ٶ��½���

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
	static bool LastPress = false; // LastPress=�ϴΰ���ֵ,Lib_ButtonPressed=��ǰ����ֵ
	bool ret = (Lib_ButtonPressed != LastPress);  
	if (ret) LastPress = Lib_ButtonPressed;  // ����Ѿ�������Ȼ��λ���´ε���ʱ������false;
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

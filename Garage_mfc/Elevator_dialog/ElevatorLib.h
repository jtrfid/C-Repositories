/*****************************************************************
 * Elevator Simulator ����״̬���������
 * ��MFCΪ���ߣ�ͼ��չʾ������ݷ�������, �ṩ������ƣ�λ�ø�֪����ť�������
 * ���ļ��ǵ���״̬���������ͷ�ļ�
 * (1) ����״̬��״̬����
 * (2) �⺯��˵��
 ******************************************************************/
#pragma once
#ifndef ELEVATORLIB_H_
#define ELEVATORLIB_H_


#ifdef __cplusplus
extern "C" {
#endif

/*
 * ����(Constants)
 */


/**
 * ¥���(m)����¥���ĸ߶�
 */
#define Lib_FloorSpacing 3.28

/** 
 * �ж��Ƿ�Ŀ��¥����������
 * �ж��Ƿ��½���Ŀ��¥�㣺
 * fabs(GetFloor() - GoingDownToFloor() < Lib_FloorTolerance
 * �ж��Ƿ�������Ŀ��¥�㣺
 * fabs(GetFloor() - GoingUpToFloor() < Lib_FloorTolerance
 */
//#define Lib_FloorTolerance 0.16
#define Lib_FloorTolerance 0.01

// ¥����
#define Lib_FloorNum 3

// ���������±��ؾ������������Ը߶ȣ����������ʱ�����������±��ؾ���������Ը߶�
#define Lib_MaxCarPosition (Lib_FloorNum-1)*Lib_FloorSpacing

// ��mfc������Ϣ,���µ������ⰴť�Ƶ�״̬
#define WM_LIGHT_MESSAGE (WM_USER+100)
typedef struct  
{
	int type;     // 1: ��ʾ������¥�㰴ť��(Panel Floor Light), 2: ��ʾ�����ڿ����Ű�ť��, 3: ��ʾ������Up/Down��ť��(Call Light)
	int floor;    // ¥���, type=2ʱ,floor�����壬
	bool LightOn; // true: ��ť��On�� false: ��ť��Off
	bool up;      // true: Up��ť�ƣ�false: Down��ť��; ������Up/Down�д��type=1,2ʱ,����������
}Light_Msg;

// ��mfc������Ϣ,���µ������ⰴť�Ƶ�״̬
extern void postToMfc(int type,int floor,bool LightOn,bool up);

// ��mfc������Ϣ,��ʾ��ǰ״̬
#define WM_Status_MESSAGE (WM_USER+200)
extern void ViewStatus(CString status);

// ��mfc������Ϣ,��/�رյ�����
#define WM_Door_MESSAGE (WM_USER+300)

// floor: ��ʾ����¥��
// Open = true;���ţ��������
extern void OpenCloseDoor(int floor,bool Open);

// �����ھ�����ڶԻ���OnInitDialog�б���ֵ���������䷢����Ϣ
extern HWND MAIN_WIN;

/************************************************************************/
/* ��������״̬                                                          */
/************************************************************************/
#define Idle        1
#define MovingUp    2
#define MovingDown  3
#define DoorOpen    4
#define DoorClosing 5

/*******************************************************************************/
/* ˵����ElevatorLib.cpp�ж����ȫ�ֱ�������ΪLib��mfc֮��Ĺ���ı�����������Ϣ���ݡ� */
/*******************************************************************************/
extern bool Lib_Running;       // �������з���: true; ����false
// ���������±��ؾ���������Ը߶�,��ʼֵ0
extern double Lib_CurrentCarPosition;
// ���������ٶȣ�Up>0,Down<0
extern double Lib_CurrentCarVelocity;
// ������ʣ�-1.0��1.0֮�䣬��Ϊ1.0,��������������ٶ�����;��Ϊ-1.0,��������������ٶ��½���
extern double Lib_Power;
// ����: �������Ű�ť��
extern bool Lib_OpenDoorLight; 
extern bool Lib_CloseDoorLight;
// ����: ¥�㰴ť��,����Ԫ�ض�Ӧ����¥�����ְ�ť��״̬,ע���±�0 -- Lib_FloorNum-1
extern bool Lib_PanelFloorLight[Lib_FloorNum];
// ����: Up/Down��ť��(Call Light),����Ԫ�ض�Ӧ��¥������Up/Down��ť��״̬,ע���±�0 -- Lib_FloorNum-1
extern bool Lib_CallLightUp[Lib_FloorNum];
extern bool Lib_CallLightDown[Lib_FloorNum];

// ���ݵ�ǰ�˶�״̬������(true)�����½�(false)��
// IsgongUp()�����У������1¥��Lib_gongUp=true; ��������¥��Lib_gongUp=false��
// IdleWhatFloorToGoTo()��ά����ֵ
// ��elevator.cpp�ж�ȡ��ֵ������IsgoingUp()���ظ�ֵ��
extern bool Lib_goingUp;
extern bool IsgoingUp();

// ����Ҫ����¥��
extern int Lib_WillToFloor;

// �����Ŷ�ʱ���Ѿ���������ʾ���ڿ�����SetDoor()��ȡ��mfcά��
extern bool Lib_DoorTimerStarted;
// ���Ž���,��mfc��ά��
extern bool Lib_DoorOpened;
// ���Ž���,��mfc��ά��
extern bool Lib_DoorClosed;

// һ��ʱ���޶������Զ���һ¥,�ñ���ȷ����ʱ���Ƿ�����
extern bool Lib_AutoTimerStarted;
// һ��ʱ���޶������Զ���1¥
extern void AutoTo1Floor();
// ȡ���Զ���1¥
extern void CancelTo1Floor();
// 10sʱ�䵽���Զ�ִ�е�1¥�Ķ���
extern void To1Floor(int *state);

/////////////////// ������
// ���ݾ�����¥�������ͣ����¥��
extern int Lib_FloorArrived;

/************************************************************************
 *  ��elevator.cpp�ж��壬ʵ��״̬������
 * ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
 ************************************************************************/
extern void main_control(int *state);

/*
 * Management
 */

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
extern bool IsElevatorRunning();

/** \brief Start the Elevator Simulation.
 *
 * This function must be called before any other call can be made
 * to any Elevator Simulator functions. It starts the elevator simulator
 * in a new window.
 */
extern void ElevatorStartup();

/** \brief Shut down the Elevator Simulation.
 *
 * This function should be called last in the program. If the simulator is
 * currently running, it shuts down the simulator and closes the window. Then
 * it releases any resources allocated to this program.
 */
extern void ElevatorShutdown();


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
extern void SetCloseDoorLight(bool s);

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
extern bool GetCloseDoorLight();

/** \brief Set the status of the open door light on the panel in the elevator car.
 *
 * This function sets the lighted status of the open door button inside the
 * elevator car. If set to true, the light is turned on.
 * \param s Set to true to run the light on
 * \code
    SetOpenDoorLight(true);
 \endcode
 */
extern void SetOpenDoorLight(bool s);

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
extern bool GetOpenDoorLight();


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
extern void SetPanelFloorLight(int floor, bool s);

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
extern bool GetPanelFloorLight(int floor);

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
extern void SetCallLight(int floor, bool up, bool s);

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
extern bool GetCallLight(int floor, bool up);

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
extern void SetDoorIndicator(int floor, bool up, bool s);

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
extern bool GetDoorIndicator(int floor, bool up);

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
extern void SetDoor(int floor, bool open);

/** \brief Determines if the elevator door is open.
 *
 * This function returns true if the elevator door is completely open.
 * When a door is moving from open to closed or vice versa, this function
 * and IsDoorClosed will both return false. Use this function to determine
 * when the opening process has completed.
 * \param floor The floor number. 1, 2, or 3.
 */
extern bool IsDoorOpen(int floor);

/** \brief Determines if the elevator door is closed.
 *
 * This function returns true if the elevator door is completely closed.
 * When a door is moving from open to closed or vice versa, this function
 * and IsDoorOpen will both return false. Use this function to determine
 * when the closing process has completed.
 * \param floor The floor number. 1, 2, or 3.
 */
extern bool IsDoorClosed(int floor);

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
extern void SetOpenDoorHandler(void (*handler)());

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
extern void SetCloseDoorHandler(void (*handler)());

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
extern void SetCallPressedHandler(void (*handler)(int floor, bool up));

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
extern void SetFloorPressedHandler(void (*handler)(int floor));


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
extern void SetBrake(bool b);

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
extern void SetMotorPower(double power);

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
extern double GetPosition();

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
extern double GetVelocity();

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
extern double GetFloor();

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
extern int GetNearestFloor();

/** \brief Change the current number of passengers on the elevator.
 *
 * This function will cause the number of passengers on the elevator to change
 * to a new random number. This happens automatically when the door opens on any
 * floor, but can also be used to test elevator control code by selecting random
 * loadings.
 */
extern void ChangeLoading();

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
extern void SetLoading(int r);


/*
 * Timer
 */

/** \brief Reset a timer available for program use.
 *
 * This function, when called, resets a timer to zero. It is equivalent to pressing
 * the reset button on a stopwatch. The stopwatch can then be read using the GetTimer
 * function. There is only one timer available to an application.
 */
extern void ResetTimer();

/** \brief Read the current value of the timer.
 *
 * This function read the current value of an available timer. The timer can be
 * reset using the ResetTimer call. The value read is in second and is a floating point
 * value.
 * \return Number of seconds since the last time ResetTimer was called.
 */
extern double GetTimer();



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
//extern int WhatFloorToGoTo(bool up);
/************************************************************************
 * ��̬���
 * ���ݴ��ڿ���״̬, ȷ����һ�����˶����������¥��
 *  ������up ������ֵ>0ʱ����һ�����ݵ��˶�����true��ʾ���ϣ�false��ʾ����
 *  ����Ҫ����¥�㣬���򷵻�-1
 ************************************************************************/
extern int IdleWhatFloorToGoTo(bool *up);

/************************************************************************
 * ��̬���
 * ���ݴ��ڿ���״̬,��飺����Ҫ����¥�㣬���򷵻�-1
 ***********************************************************************/
extern int IdleGoingUpToFloor();

/************************************************************************
 * ��̬���
 * ���ݴ��ڿ���״̬,��飺����Ҫ����¥�㣬���򷵻�-1
 ***********************************************************************/
extern int IdleGoingDownToFloor();

/************************************************************************
 * ��̬���, ������������ʱ����⽫Ҫ����ֹͣ�����¥��
 * ������������,�ڵ�ǰ¥�����һ��֮���һ��߶����£�����Ƿ���һ¥����Ҫ����¥��
 * �������һ�룬�Ͳ������������ԭ���洢��ֵ����Ϊ����һ�룬��û��ʱ����ֱ�����ֹͣ����
 * ����ĵ�ǰ¥��ָ���ո����о�����¥�㣬��(int)GetFloor()���ص�¥��
 * ------- 3Floor
 * --- GetFloor()
 * ------- 2Floor, (int)GetFloor()  ��ǰ��
 ***********************************************************************/
extern int GoingUpToFloor();

/************************************************************************
 * ��̬���, ���������½�ʱ����⽫Ҫ����ֹͣ�����¥��
 * 10s���޶������Զ��½���һ¥����ʱ����1.
 * ������������,�ڵ�ǰ¥�����һ��֮���һ��߶����ϣ�����Ƿ���һ¥����Ҫ����¥��
 * �������һ�룬�Ͳ������������ԭ���洢��ֵ����Ϊ����һ�룬��û��ʱ����ֱ�����ֹͣ����
 * ����ĵ�ǰ¥��ָ���ո����о�����¥�㣬��(int)GetFloor()���ص�¥�� + 1
 * ------- 3Floor ��ǰ��
 * --- GetFloor()
 * ------- 2Floor, (int)GetFloor()
 ***********************************************************************/
extern int GoingDownToFloor();

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
//extern int WhatFloorToGoToInThisDirection(bool up);
extern int WhatFloorToGoToInThisDirection();

#ifdef __cplusplus
}
#endif

#endif /* ELEVATORLIB_H_ */

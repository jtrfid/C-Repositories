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

// c++�������c���Գɷֵı�־��.c�ļ������д˱�־��.cpp�ļ��д˱�־�����Ĳ��ֱ�ʾ����C���Գɷ�
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
// ����: ¥�㰴ť��(PanelFloorLight),����Ԫ�ض�Ӧ����¥�����ְ�ť��״̬,ע���±�0 -- Lib_FloorNum-1
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

// Ŀ��¥�㣬����һ����Ҫ����¥�㣬��ʼֵ-1
extern int Lib_WillToFloor;

// �����Ŷ�ʱ���Ѿ���������ʾ���ڿ�����SetDoor()��ȡ��mfcά��
extern bool Lib_DoorTimerStarted;
// ���Ž���,��mfc��ά��
extern bool Lib_DoorOpened;
// ���Ž���,��mfc��ά��
extern bool Lib_DoorClosed;

// һ��ʱ�䣨AutoTimerDuration���޶������Զ���һ¥,�ñ���ȷ����ʱ���Ƿ�����
extern bool Lib_AutoTimerStarted;
extern int AutoTimerDuration;  // ms��ʱ����ȱʡ10000ms,��10s
// һ��ʱ���޶������Զ���1¥
extern void AutoTo1Floor();
// ȡ���Զ���1¥
extern void CancelTo1Floor();
// AutoTimerDurationʱ�䵽���Զ�ִ�е�1¥�Ķ���
extern void To1Floor(int *state);

/////////////////// ������
// ���ݾ�����¥�������ͣ����¥��
extern int Lib_FloorArrived;

/************************************************************************
 * ��elevator.cpp�ж��壬ʵ��״̬������
 * ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
 ************************************************************************/
extern void main_control(int *state);

/** 
 * �����Ƿ������У�����true�����У�����û������
 */
extern bool IsElevatorRunning();

/** 
 * �������棬�ڳ�ʼ��ʱ����
 */
extern void ElevatorStartup();

/** 
 * ��������
 */
extern void ElevatorShutdown();


/******************************************************************
 * ��ť��(Lights): 
 * ��1�����ڿ����Ű�ť�ƣ���2������¥�㰴ť�ƣ�Panel Floor Light������3������Up/Down��ť��Call Light��
 ******************************************************************/


/** 
 * �������ڹ��Ű�ť�Ƶ�״̬
 * ����s: true,��ť������false����ť����
 */
extern void SetCloseDoorLight(bool s);

/** 
 * �������ڹ��Ű�ť�Ƶ�״̬
 * true,��ť������false����ť����
 */
extern bool GetCloseDoorLight();

/** 
 * �������ڿ��Ű�ť�Ƶ�״̬
 * ����s: true,��ť������false����ť����
 */
extern void SetOpenDoorLight(bool s);

/** 
 * �������ڿ��Ű�ť�Ƶ�״̬
 * true,��ť������false����ť����
 */
extern bool GetOpenDoorLight();


/** 
 * ��������¥�㰴ť�ƣ�Panel Floor Light����״̬
 * ����floor: ��ʾ¥��
 * ����s: true,��ť������false����ť����
 */
extern void SetPanelFloorLight(int floor, bool s);

/** 
 * ��������¥�㰴ť�ƣ�Panel Floor Light����״̬
 * ����floor: ��ʾ¥��
 * ���أ�true,��ť������false����ť����
 */
extern bool GetPanelFloorLight(int floor);

/** 
 * ��������Up/Down�ƣ�Call Light����״̬
 * ����floor: ��ʾ¥��
 * ����up: ��ʾ����Up(����)��ť��
 * ����s: true,��ť������false����ť����
 */
extern void SetCallLight(int floor, bool up, bool s);

/** 
 * ��������Up/Down�ƣ�Call Light����״̬
 * ����floor: ��ʾ¥��
 * ����up: ��ʾ����Up(����)��ť��
 * ����: true,��ť������false����ť����
 */
extern bool GetCallLight(int floor, bool up);


/********************************************
 * ����������(Doors)
 ********************************************/

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

/** 
 * �������Ƿ��Ǵ򿪵ģ�true��ʾ���Ǵ򿪵ģ��������ǹرյĻ������ڴ�/�ر�
 * ����floor: ��ʾ���ڵ�¥��
 */
extern bool IsDoorOpen(int floor);

/** 
 * �������Ƿ��ǹرյģ�true�رյģ��������Ǵ򿪵Ļ������ڴ�/�ر�
 * ����floor: ��ʾ���ڵ�¥��
 */
extern bool IsDoorClosed(int floor);

/**************************************************
 * ������ƣ�Motor control��
 **************************************************/

/** 
 * ɲ������ʱδ��
 * ����b��true��ʾʹ��ɲ��������ɲ��
 */
extern void SetBrake(bool b);

/**
 * ���õ������
 * ����power��-1��1֮�䡣-1������ٶ��½���1������ٶ�����
 */
extern void SetMotorPower(double power);

/** 
 * ��ȡ����λ��(m),�������±��ؾ���һ�����ľ��롣
 * ����ͣ����һ�㣬����0;
 * ����ͣ���ڶ��㣬����1*Lib_FloorSpacing;  
 * ����ͣ�������㣬����2*Lib_FloorSpacing; 
 * ���þ�����
 if(GetPosition() > FloorSpacing) {
       printf("The elevator is above the second floor\n");
   }
 */
extern double GetPosition();

/** 
 * ��ȡ���ݵ��ٶȣ�>0������<0�½�����ʱδ�� 
 */
extern double GetVelocity();

/**
 * ���ص�ǰ�������ڲ������Ǹ���������˿��Ա�ʾ¥��֮�䡣
 * ��Ч��: GetPosition() / FloorSpacing + 1
 */
extern double GetFloor();

/** 
 * ��ȡGetFloor()�������������������������ֵ
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

#ifdef __cplusplus
}
#endif

#endif /* ELEVATORLIB_H_ */

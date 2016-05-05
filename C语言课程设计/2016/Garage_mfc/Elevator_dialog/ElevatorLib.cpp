#include "stdafx.h"
#include <math.h>

#include "ElevatorLib.h"
#include "resource.h"
#include "Elevator_dialogDlg.h"

/*******************************************************************************
* ȫ�ֱ�������ΪLib��mfc֮��Ĺ���ı�����������Ϣ���ݡ�
 *******************************************************************************/
bool Lib_Running = false; // �������з���: true; ����false
// ���������±��ؾ���������Ը߶�,��ʼֵ0
double Lib_CurrentCarPosition = 0;
// ���������ٶȣ�Up>0,Down<0
double Lib_CurrentCarVelocity = 0;
// ������ʣ�-1.0��1.0֮�䣬��Ϊ1.0,��������������ٶ�����;��Ϊ-1.0,��������������ٶ��½�;��ʼֵ0.0��
double Lib_Power = 0.0;
// ����: �������Ű�ť��
bool Lib_OpenDoorLight = false; 
bool Lib_CloseDoorLight = false;
// ����: ¥�㰴ť��(PanelFloorLight),����Ԫ�ض�Ӧ����¥�����ְ�ť��״̬,ע���±�0 -- Lib_FloorNum-1
bool Lib_PanelFloorLight[Lib_FloorNum] = {false,false,false};
// ����: Up/Down��ť��(Call Light),����Ԫ�ض�Ӧ��¥������Up/Down��ť��״̬,ע���±�0 -- Lib_FloorNum-1
bool Lib_CallLightUp[Lib_FloorNum] = {false,false,false};
bool Lib_CallLightDown[Lib_FloorNum] = {false,false,false};

// ����״̬����ʱ�����Ǿ�ֹ�ģ����ݵ�ǰ�˶�����������(true)�����½�(false)
// IsgongUp()�����У������1¥��Lib_gongUp=true; ��������¥(Lib_FloorNum)��Lib_gongUp=false��
// IdleWhatFloorToGoTo()��ά����ֵ
// ��elevator.cpp�ж�ȡ��ֵ������IsgoingUp()���ظ�ֵ��
bool Lib_goingUp = true;
bool IsgoingUp() {
	if(GetNearestFloor() == 1) Lib_goingUp = true;
	else if (GetNearestFloor() == Lib_FloorNum) Lib_goingUp = false; 
	return Lib_goingUp;
}

// Ŀ��¥�㣬����һ����Ҫ����¥�㣬��ʼֵ-1
int Lib_WillToFloor = -1;

// �����Ŷ�ʱ���Ѿ���������ʾ���ڿ�����,SetDoor()��ȡ��mfcά��
bool Lib_DoorTimerStarted = false;
// ���Ž���,��mfc��ά��
bool Lib_DoorOpened = false;
// ���Ž���,��mfc��ά��
bool Lib_DoorClosed = false;

// һ��ʱ�䣨AutoTimerDuration���޶������Զ��½���1¥,�ñ���ȷ����ʱ���Ƿ�����
bool Lib_AutoTimerStarted = false;
int AutoTimerDuration = 10000;  // ms��ʱ����ȱʡ10000ms,��10s
// һ��ʱ���޶������Զ���1¥
extern void AutoTo1Floor() 
{
	if (!Lib_AutoTimerStarted)
	{
		SetTimer(CElevator_dialogDlg::MAIN_WIN,ID_AUTO_TIMER,AutoTimerDuration,NULL);//10s,10000ms
		Lib_AutoTimerStarted = true;
	}
}
// ȡ���Զ���1¥
extern void CancelTo1Floor() 
{
	if (Lib_AutoTimerStarted)
	{
		KillTimer(CElevator_dialogDlg::MAIN_WIN,ID_AUTO_TIMER);
		Lib_AutoTimerStarted = false;
	}
}
// AutoTimerDurationʱ�䵽���Զ�ִ�е�1¥�Ķ�������mfc��ʱ���ص������б�����
extern void To1Floor(int *state) 
{
	printf("%d(s)ʱ�䵽���Զ��½���1¥...\n",AutoTimerDuration/1000);
	// ��ʱ�����е������Up/Down Call Light��ť�������ģ����Թر����еģ�
	// �����ڵ�¥�㰴ťҲ�п��������ģ��ر�֮��
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
 * �����Ƿ������У�����true�����У�����û������
 */
bool IsElevatorRunning()
{
	return Lib_Running;
}

/** 
 * �������棬�ڳ�ʼ��ʱ����
 */
void ElevatorStartup()
{
	Lib_Running = true;
}

/** 
 * ��������
 */
void ElevatorShutdown()
{
	Lib_Running = false;
}

/******************************************************************
 * ��ť��(Lights): 
 * ��1�����ڿ����Ű�ť�ƣ���2������¥�㰴ť�ƣ�Panel Floor Light����
 * ��3������Up/Down��ť��Call Light��
 ******************************************************************/

/** 
 * �������ڹ��Ű�ť�Ƶ�״̬
 * ����s: true,��ť������false����ť����
 */
void SetCloseDoorLight(bool s)
{
	Lib_CloseDoorLight = s;
	// ��mfc������Ϣ�����µ������ⰴť��״̬������ 2: ��ʾ�����ڿ����Ű�ť��
	postToMfc(2,0,s,0);
}

/** 
 * �������ڹ��Ű�ť�Ƶ�״̬
 * true,��ť������false����ť����
 */
bool GetCloseDoorLight()
{
	return Lib_CloseDoorLight;
}

/** 
 * �������ڿ��Ű�ť�Ƶ�״̬
 * ����s: true,��ť������false����ť����
 */
void SetOpenDoorLight(bool s)
{
	Lib_OpenDoorLight = s;
	// ��mfc������Ϣ�����µ������ⰴť��״̬������ 2: ��ʾ�����ڿ����Ű�ť��
	postToMfc(2,0,s,0);
}

/** 
 * �������ڿ��Ű�ť�Ƶ�״̬
 * true,��ť������false����ť����
 */
bool GetOpenDoorLight()
{
	return Lib_OpenDoorLight;
}

/** 
 * ��������¥�㰴ť�ƣ�Panel Floor Light����״̬
 * ����floor: ��ʾ¥��
 * ����s: true,��ť������false����ť����
 */
void SetPanelFloorLight(int floor, bool s)
{
	Lib_PanelFloorLight[floor-1] = s;
	// ��mfc������Ϣ�����µ������ⰴť��״̬������ 1: ��ʾ������¥�㰴ť��(Panel Floor Light)
	postToMfc(1,floor,s,0);
}

/** 
 * ��������¥�㰴ť�ƣ�Panel Floor Light����״̬
 * ����floor: ��ʾ¥��
 * ���أ�true,��ť������false����ť����
 */
bool GetPanelFloorLight(int floor)
{
	return Lib_PanelFloorLight[floor-1];
}

/** 
 * ��������Up/Down�ƣ�Call Light����״̬
 * ����floor: ��ʾ¥��
 * ����up: ��ʾ����Up(����)��ť��
 * ����s: true,��ť������false����ť����
 */
void SetCallLight(int floor, bool up, bool s)
{
	if(up) Lib_CallLightUp[floor-1] = s;
	else Lib_CallLightDown[floor-1] = s;
	// ��mfc������Ϣ�����µ������ⰴť��״̬������ 2: ��ʾ������Up/Down��ť��
	postToMfc(3,floor,s,up);
}

/** 
 * ��������Up/Down�ƣ�Call Light����״̬
 * ����floor: ��ʾ¥��
 * ����up: ��ʾ����Up(����)��ť��
 * ����: true,��ť������false����ť����
 */
bool GetCallLight(int floor, bool up)
{
	if(up) return Lib_CallLightUp[floor-1];
	else return Lib_CallLightDown[floor-1];
}

/********************************************
 * ����������(Doors)
 ********************************************/

/**
 * ����/�رյ���������
 * ����: floor, ��ʾ����¥��
 *       open, true����ʾ���ţ�false����ʾ����
 */
void SetDoor(int floor, bool open)
{
	if(!Lib_DoorTimerStarted)  // ��ֹ������δ�������ٴ�ִ��
	{
		// ������Ǵ򿪵ģ��Ͳ����ٴ���; ������ǹرյģ��Ͳ����ٹر���
		if((IsDoorOpen(floor) && open) || (IsDoorClosed(floor) && !open)) return;
		// ���Ϳ�������Ϣ��mfc���������Ŷ�ʱ��
		OpenCloseDoor(floor,open);
	}
}

/** 
 * �������Ƿ��Ǵ򿪵ģ�true��ʾ���Ǵ򿪵ģ��������ǹرյĻ������ڴ�/�ر�
 * ����floor: ��ʾ���ڵ�¥��
 */
bool IsDoorOpen(int floor)
{
	// ��ʾ���Ź��̽��������ǿ���
	return Lib_DoorOpened;
}

/** 
 * �������Ƿ��ǹرյģ�true�رյģ��������Ǵ򿪵Ļ������ڴ�/�ر�
 * ����floor: ��ʾ���ڵ�¥��
 */
bool IsDoorClosed(int floor)
{
	return Lib_DoorClosed;
}


/**************************************************
 * ������ƣ�Motor control��
 **************************************************/

/** 
 * ɲ������ʱδ��
 * ����b��true��ʾʹ��ɲ��������ɲ��
 */
void SetBrake(bool b)
{

}

/**
 * ���õ������
 * ����power��-1��1֮�䡣-1������ٶ��½�; 1������ٶ�����; 0�� ֹͣ
 */
void SetMotorPower(double power)
{
	Lib_Power = power;
}

/** 
 * ��ȡ����λ��(m),�������±��ؾ������ľ��롣
 * ����ͣ����һ�㣬����0;
 * ��������/�½���1��2¥�м�,����0.5;
 * ����ͣ���ڶ��㣬����1*Lib_FloorSpacing;
 * ��������/�½���2��3¥�м�,����1.5*Lib_FloorSpacing;
 * ����ͣ�������㣬����2*Lib_FloorSpacing; 
 * ���þ�����
 if(GetPosition() > FloorSpacing) {
       printf("The elevator is above the second floor\n");
   }
 */
double GetPosition()
{
	return Lib_CurrentCarPosition;
}

/** 
 * ��ȡ���ݵ��ٶȣ�>0������<0�½�����ʱδ�� 
 */
double GetVelocity()
{
	return Lib_CurrentCarVelocity;
}

/**
 * ���ص�ǰ�������ڲ������Ǹ���������˿��Ա�ʾ¥��֮�䡣
 * ���磬1.5����ʾ�������崦��1�㵽2����м�
 * ��Ч��: GetPosition() / FloorSpacing + 1
 */
double GetFloor()
{
	return (GetPosition() / Lib_FloorSpacing + 1);
}

/** 
 * ��ȡGetFloor()�������������������������ֵ
 */
int GetNearestFloor()
{
	//printf("%f,%f,%f,%f\n",floor(1.2),ceil(1.2),floor(1.5),ceil(1.5));     // 1,2,1,2
	//printf("%f,%f,%f,%f\n",floor(-1.2),ceil(-1.2),floor(-1.5),ceil(-1.5)); // -2,-1,-2,-1
	return (int)floor(GetFloor()+0.5); // ��������
	//return (int)GetFloor();
}

/**
 * �ı�˿�(passenger)����
 * ��ÿ�ο��ź��Ǹı�˿������Ļ��ᡣ����ʱ������һ�������������˿�������
 * ��ʱû��
 */
void ChangeLoading()
{

}

/** 
 * ���ó˿���������ʱû��
 *
 */
void SetLoading(int r)
{

}

/************************************************************************
 * ��̬���
 * ���ݴ��ڿ���״̬, ȷ����һ�����˶����������¥��(Ŀ��¥��)
 *  ������up ������ֵ>0ʱ����һ�����ݵ��˶�����true��ʾ���ϣ�false��ʾ����
 *  ����Ŀ��¥�㣬���򷵻�-1
 * ���þ���:
 * int floor; bool up;
 * floor = IdleWhatFloorToGoTo(&up);
 ************************************************************************/
int IdleWhatFloorToGoTo(bool *up)
{
	int ret;
	bool goingUp = IsgoingUp();

	*up = goingUp;
	if(goingUp) {  // ����
		ret = IdleGoingUpToFloor();
		if(ret < 0) { // ֻ�ܸı䷽��ת������
			ret = IdleGoingDownToFloor(); 
			if (ret > 0){
				*up = !goingUp;
				Lib_goingUp = *up;
			}
		}
	}
	else {  //����
		ret = IdleGoingDownToFloor();
		if(ret < 0) { // ֻ�ܸı䷽��ת������
			ret = IdleGoingUpToFloor(); 
			if (ret > 0) {
				*up = !goingUp;
				Lib_goingUp = *up;
			}
		}
	}

	if(ret > 0) ASSERT(ret>=1 && ret<=Lib_FloorNum);  // ȷ�����غϷ���¥���-1

	Lib_WillToFloor = ret;
	return ret;
}

/************************************************************************
 * ��̬���
 * ���ݴ��ڿ���״̬,��飺����Ҫ����¥��(Ŀ��¥��)�����򷵻�-1
 ***********************************************************************/
int IdleGoingUpToFloor()
{
	int floor,ret1, ret2,ret;

	// �Ѿ������¥��, ����������
	if (GetNearestFloor() == Lib_FloorNum)
	{
		return -1;
	}

	ret1 = -1, ret2 = -1,ret = -1;
	// �������¥�㰴ť����ǰ¥���ԡ��ϡ��Ƿ���Ҫ����¥��
	for(floor = GetNearestFloor() + 1; floor <= Lib_FloorNum; floor++) {
		if(GetPanelFloorLight(floor)) { ret1 = floor; break; }
	}
	// �������Up��ť��Call Light��,��ǰ¥���ԡ��ϡ��Ƿ�������
	for(floor = GetNearestFloor() + 1; floor <= Lib_FloorNum; floor++) {
		if(GetCallLight(floor,true)) { ret2 = floor; break; }
	}
	// ����������㣬ȡ����ߣ�С�ߣ�
	if (ret1 > 0 && ret2 > 0) ret = ret1 < ret2 ? ret1 : ret2;
	else if(ret1 > 0 && ret2 < 0) ret = ret1;
	else if(ret1 < 0 && ret2 > 0) ret = ret2;
	else ret = -1; // ��������
	if(ret > 0) return ret;

	// �������Down��ť��Call Light��,��ǰ¥���ԡ��ϡ��Ƿ�������
	for(floor = GetNearestFloor() + 1; floor <= Lib_FloorNum; floor++) {
		if(GetCallLight(floor,false)) { ret = floor; break; }
	}

	if(ret > 0) ASSERT(ret>=1 && ret<=Lib_FloorNum);  // ȷ�����غϷ���¥���-1

	return ret;
}

/************************************************************************
 * ��̬���
 * ���ݴ��ڿ���״̬,��飺����Ҫ����¥��(Ŀ��¥��)�����򷵻�-1
 ***********************************************************************/
int IdleGoingDownToFloor()
{
	int floor,ret1, ret2, ret;

	// �Ѿ���1¥��, ����������
	if (GetNearestFloor() == 1)
	{
		return -1;
	}

	ret1 = -1, ret2 = -1; ret = -1;
	// ������ڰ�ť����ǰ¥���ԡ��¡��Ƿ���Ҫ����¥��
	for(floor = GetNearestFloor() - 1; floor >= 1; floor--) {
		if(GetPanelFloorLight(floor)) { ret1 = floor; break; }
	}
	// �������Down��ť��Call Light��,��ǰ¥���ԡ��¡��Ƿ�������
	for(floor = GetNearestFloor() - 1; floor >= 1; floor--) {
		if(GetCallLight(floor,false)) { ret2 = floor; break; }
	}

	// ����������㣬ȡ�����(����)
	if (ret1 < 0 && ret2 < 0) ret = -1;
	else return (ret1 > ret2 ? ret1 : ret2);

	// �������Up��ť��Call Light��,��ǰ¥���ԡ��¡��Ƿ�������
	for(floor = GetNearestFloor() -1; floor >= 1; floor--) {
		if(GetCallLight(floor,true)) { ret = floor; break; }
	}

	if(ret > 0) ASSERT(ret>=1 && ret<=Lib_FloorNum);  // ȷ�����غϷ���¥���-1

	return ret;
}

/************************************************************************
 * ��̬���, ������������ʱ����⽫Ҫ����ֹͣ�����¥��(Ŀ��¥��)
 * ������������,�ڵ�ǰ¥�����һ��֮���һ��߶����£�����Ƿ���һ¥����Ҫ����¥��
 * �������һ�룬�Ͳ������������ԭ���洢��ֵ����Ϊ����һ�룬��û��ʱ����ֱ�����ֹͣ����
 * ����ĵ�ǰ¥��ָ���ո����о�����¥�㣬��(int)GetFloor()���ص�¥��
 * ------- 3Floor
 * --- GetFloor()
 * ------- 2Floor, (int)GetFloor()  ��ǰ��
***********************************************************************/
int GoingUpToFloor()
{
	// int floor =  GetNearestFloor(); // ʹ������������ʽ��GetNearestFloor()���жϸ���
	int floor =  (int)GetFloor(); // ��ǰ¥�㣬���ով�����¥��

	// ��ʾ���о�����¥��
	if(fabs(GetFloor() - floor) < Lib_FloorTolerance) {
		CString status;
		status.Format(_T("����,[%d]¥"),floor);
		ViewStatus(status);
		// �����ǰ�����Ҫ����¥��Ͽ췵��
		if(Lib_WillToFloor == floor) return Lib_WillToFloor;
	}

	// �����һ�����Ҫ����¥��,�Ͳ����ж��ˡ�
	if(Lib_WillToFloor == floor + 1) return Lib_WillToFloor;

	// �������һ�룬�Ͳ������������ԭ���洢��ֵ����Ϊ����һ�룬��û��ʱ����ֱ�����ֹͣ����
	if(GetFloor() - floor > 0.5) return Lib_WillToFloor;

	// �������¥�㰴ť����ǰ¥�����һ���Ƿ���Ҫ����¥��
	floor++; 
	ASSERT(floor>=1 && floor<=Lib_FloorNum); // ���Դ�ʱ����û��Խ����������

	if(GetPanelFloorLight(floor))  { Lib_WillToFloor = floor; }
	// �������Up��ť��Call Light��,��ǰ¥�����һ���Ƿ�������
	else if(GetCallLight(floor,true)) { Lib_WillToFloor = floor; }
	// ��ǰ¥������û��Ҫ����¥�㣬ֻ�ܼ�鵱ǰ¥�����һ���down��ť
	// �������Down��ť��Call Light��,��ǰ¥�����һ���Ƿ�������
	else if(Lib_WillToFloor < floor && GetCallLight(floor,false)) { Lib_WillToFloor = floor; }

	if(Lib_WillToFloor > 0) ASSERT(Lib_WillToFloor>=1 && Lib_WillToFloor<=Lib_FloorNum);  // ȷ�����غϷ���¥���-1

	// ���û�иı䣬����ԭ���洢��ֵ��
	return Lib_WillToFloor;
}

/************************************************************************
 * ��̬���, ���������½�ʱ����⽫Ҫ����ֹͣ�����¥��(Ŀ��¥��)
 * AutoTimerDuration(10s)���޶������Զ��½���һ¥����ʱ����1.
 * ������������,�ڵ�ǰ¥�����һ��֮���һ��߶����ϣ�����Ƿ���һ¥����Ҫ����¥��
 * �������һ�룬�Ͳ������������ԭ���洢��ֵ����Ϊ����һ�룬��û��ʱ����ֱ�����ֹͣ����
 * ����ĵ�ǰ¥��ָ���ո����о�����¥�㣬��(int)GetFloor()���ص�¥�� + 1
 * ------- 3Floor ��ǰ��
 * --- GetFloor()
 * ------- 2Floor, (int)GetFloor()
 ***********************************************************************/
int GoingDownToFloor()
{
	//int floor =  GetNearestFloor(); // ʹ������������ʽ��GetNearestFloor()���жϸ���
	int floor =  (int)GetFloor(); // ��ǰ¥�����һ�㣬���ով�����¥����floor+1

	// ��ʾ���о�����¥��
	if(fabs(GetFloor() - floor) < Lib_FloorTolerance) {
		CString status;
		status.Format(_T("����,[%d]¥"),floor);
		ViewStatus(status);
		// �����1¥����������1. ������10s���޶������Զ��½���һ¥�����
		if(floor == 1) { Lib_WillToFloor = floor; return floor; }
	}

	// ���floor����Ҫ����¥��,�Ͳ����ж��ˡ�
	if(Lib_WillToFloor == floor) return Lib_WillToFloor;

	// �������һ�룬�Ͳ������������ԭ���洢��ֵ����Ϊ����һ�룬��û��ʱ����ֱ�����ֹͣ����
	if(GetFloor() - floor < 0.5) return Lib_WillToFloor;

	// �������¥�㰴ť����ǰ¥�����һ���Ƿ���Ҫ����¥��
	if(GetPanelFloorLight(floor))  { Lib_WillToFloor = floor; }
	// �������Down��ť��Call Light��,��ǰ¥�����һ���Ƿ�������
	else if(GetCallLight(floor,false)) { Lib_WillToFloor = floor; }
	// ��ǰ¥������û��Ҫ����¥�㣬ֻ�ܼ�鵱ǰ¥�����һ���up��ť
	// �������up��ť��Call Light��,��ǰ¥�����һ���Ƿ�������
	else if(Lib_WillToFloor >= floor && GetCallLight(floor,true)) { Lib_WillToFloor = floor; }

	if(Lib_WillToFloor > 0) ASSERT(Lib_WillToFloor>=1 && Lib_WillToFloor<=Lib_FloorNum);  // ȷ�����غϷ���¥���-1

	// ���û�иı䣬����ԭ���洢��ֵ��
	return Lib_WillToFloor;
}

/**
* ��mfc������Ϣ,���µ������ⰴť�Ƶ�״̬
* int type;  // 1: ��ʾ������¥�㰴ť��(Panel Floor Light), 2: ��ʾ�����ڿ����Ű�ť��, 3: ��ʾ������Up/Down��ť��(Call Light)
* int floor; // ¥���, type=2ʱ,floor������
* bool LightOn; // true: ��ť��On�� false: ��ť��Off
* bool up;      // true: Up��ť�ƣ�false: Down��ť��; ������Up/Down�д���                                                                 
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

// ��mfc������Ϣ,��ʾ��ǰ״̬
void ViewStatus(CString status)
{
	CString *msg =new CString(status);
	::PostMessage(CElevator_dialogDlg::MAIN_WIN,WM_Status_MESSAGE,0,(LPARAM)msg);
}

// ��mfc������Ϣ,��/�رյ�����
// floor: ��ʾ����¥��
// Open = true;���ţ��������
void OpenCloseDoor(int floor,bool Open)
{
	::PostMessage(CElevator_dialogDlg::MAIN_WIN,WM_Door_MESSAGE,(WPARAM) floor,(LPARAM)Open);
}
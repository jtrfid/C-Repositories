#include "stdafx.h"  // mfc�����
#include "GarageLib.h"

bool Lib_Running = false;        // �������з���: true; ����false
bool Lib_ButtonPressed = false;  // ÿ�ΰ��³����ſ��أ�Lib_ButtonPressed = true; ��WasButtonPressed()�ڶ�ȡ����Ϊfalse
bool Lib_BeamBroken = false;     // ���²��ĺ�����̽�⵽����: true,ֹͣ���Ŷ���; ����false
// ��ǰ�ŵ����λ�ã��ŵ��±��ؾ���������Ը߶ȡ�
// �Źر�ʱ<=DoorTolerance, ��ʱ>=(DoorHeight-DoorTolerance)
double Lib_CurrentDoorPosition = 0.0;  
double Lib_Power = 0.0;   // ֱ��������ʣ�-1.0��1.0֮�䣬��Ϊ1.0,������������ٶ�����;��Ϊ-1.0,������������ٶ��½���

/* ϵͳ�Ƿ����� */
bool IsGarageRunning()
{
	return Lib_Running;
}

/* ��ʼϵͳ���� */
void GarageStartup()
{
	Lib_Running = true;
}

/* ����ϵͳ���� */
void GarageShutdown()
{
	Lib_Running = false;
}


/**
 �Ƿ񰴼�������true����ʾ���������ķ���ֵΪfalse���´ΰ����ַ���true��
 ������֤��ȡһ�ΰ�������󰴼�ֵ��Ϊfalse, ��������ѭ������һ��ѭ���ظ�������ֵ��
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
 �ŵ��²�����̽�⵽���壬����true�����򷵻�false
 **/
bool IsBeamBroken()
{
	return Lib_BeamBroken;
}

/**
 ����ֱ��������ʣ� - 1.0��1.0֮�䣬��Ϊ1.0, ������������ٶ�����; ��Ϊ - 1.0, ������������ٶ��½���
 **/
void SetMotorPower(double power)
{
	Lib_Power = power;
}


/**
 ��ǰ�ŵ����λ�ã��ŵ��±��ؾ���������Ը߶ȡ�
 �Źر�ʱ<=DoorTolerance, ��ʱ>=(DoorHeight-DoorTolerance)
 **/
double GetDoorPosition()
{
	return Lib_CurrentDoorPosition;
}

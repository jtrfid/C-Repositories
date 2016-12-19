/** 
 * �����ŷ���⺯��˵��
 */

#pragma once
#ifndef GARAGELIB_H_
#define GARAGELIB_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ������Constants��
 */


/** 
 �ŵĸ߶ȣ��ף�meters��
 */
#define Lib_DoorHeight 2.13

/**
 �򿪻�ر��ŵ��ݲ�ף�meters��
 �Źر�ʱ���ŵ��±��ؾ���������Ը߶�<=DoorTolerance, 
 ��ʱ, �ŵ��±��ؾ���������Ը߶�>=(DoorHeight-DoorTolerance)
 */
#define Lib_DoorTolerance 0.02

/************************************************************************
 ״̬����                                     
 ************************************************************************/
#define DoorClosed 1
#define DoorOpening 2
#define DoorOpen 3
#define DoorClosing 4

/*******************************************************************************/
/* ˵����garageLib.cpp�ж����ȫ�ֱ�������ΪLib��mfc֮��Ĺ���ı�����������Ϣ���ݡ� */
/*******************************************************************************/
extern bool Lib_Running;        // �������з���: true; ����false
extern bool Lib_ButtonPressed;  // ÿ�ΰ��³����ſ��أ�Lib_ButtonPressed = true; ��WasButtonPressed()�ڶ�ȡ����Ϊfalse
extern bool Lib_BeamBroken;     // ���²��ĺ�����̽�⵽����: true,ֹͣ���Ŷ���; ����false
// ��ǰ�ŵ����λ�ã��ŵ��±��ؾ���������Ը߶ȡ�
// �Źر�ʱ<=DoorTolerance, ��ʱ>=(DoorHeight-DoorTolerance)
extern double Lib_CurrentDoorPosition;  
extern double Lib_Power;   // ֱ��������ʣ�-1.0��1.0֮�䣬��Ϊ1.0,������������ٶ�����;��Ϊ-1.0,������������ٶ��½���

/************************************************************************
 * ��garage.cpp�ж��壬ʵ��״̬������
 * ����ѭ���� ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
 ************************************************************************/
extern void main_control(int *state);

/* ϵͳ�Ƿ����� */
extern bool IsGarageRunning();

/* ��ʼϵͳ���� */
extern void GarageStartup();

/* ����ϵͳ���� */
extern void GarageShutdown();


/**
 �Ƿ񰴼�������true����ʾ���������ķ���ֵΪfalse���´ΰ����ַ���true��
 ������֤��ȡһ�ΰ�������󰴼�ֵ��Ϊfalse, ��������ѭ������һ��ѭ���ظ�������ֵ��
 **/
extern bool WasButtonPressed();

/**
 �ŵ��²�����̽�⵽���壬����true�����򷵻�false
 **/
extern bool IsBeamBroken();

/**
 ����ֱ��������ʣ� - 1.0��1.0֮�䣬��Ϊ1.0, ������������ٶ�����; ��Ϊ - 1.0, ������������ٶ��½���
 **/
extern void SetMotorPower(double power);

/**
 ��ǰ�ŵ����λ�ã��ŵ��±��ؾ���������Ը߶ȡ�
 �Źر�ʱ<=DoorTolerance, ��ʱ>=(DoorHeight-DoorTolerance)
 **/
extern double GetDoorPosition();

#ifdef __cplusplus
}
#endif

#endif /* ELEVATORLIB_H_ */

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 ������ݷ������״̬����
 ѧ�ţ�
 ������
 ���ڣ�
 **********************************************/

/***********************************************
 * ����ѭ���� ״̬����ÿ��һ��ʱ��(�磬100ms)������һ�Σ��ɼ�ϵͳ������״̬
 ***********************************************/
void main_control(int *state)
{  
    if(IsElevatorRunning())
    {
		switch(*state)
		{
		case Idle:
		case MovingUp:
		case MovingDown:
		case DoorOpen:
		case DoorClosing:
		default:
			printf("û������״̬!!!\n");  
		}
    }
}

#include <stdio.h>
#include "stdafx.h"
#include "garage.h"

/* DoorClosed״̬���� */
void StateDoorClosed(int *state)
{
	printf("��ǰ״̬����DoorClosed��\n");
	printf("if (���������¼�����)�����������ͬʱ����״̬��Ǩ����DoorOpening����\n");
	*state = DoorOpening;
	printf("Transition: from DoorClosed to DoorOpening.\n");
}

/* DoorOpening״̬���� */
void StateDoorOpening(int *state)
{
	printf("��ǰ״̬����DoorOpening��\n");
	printf("if (�ŵ��±��ؾ������ľ����Ѿ������ŵĸ߶�)�����ֹͣ��ͬʱ����״̬��Ǩ����DoorOpen��\n");
	printf("if (���������¼�����)������½���ͬʱ����״̬��Ǩ����DoorClosing��\n");
	printf("����ģ�����\n");
	*state = DoorClosing;
	printf("Transition: from DoorOpening to DoorClosing.\n");
}

/* DoorClosing״̬���� */
void StateDoorClosing(int *state)
{
	printf("��ǰ״̬����DoorClosing��\n");
	printf("if (�ŵ��±��ؾ������ľ����Ѿ�С��0)�����ֹͣ��ͬʱ����״̬��Ǩ����DoorClosed��\n");
	printf("if (���������¼�����������⵽�ϰ���)�����������ͬʱ����״̬��Ǩ����DoorOpenging��\n");
	printf("����ģ�����\n");
	*state = DoorOpening;
	printf("Transition: from DoorClosing to DoorOpening.\n");
}

/* DoorOpen״̬���� */
void StateDoorOpen(int *state)
{
	printf("��ǰ״̬����DoorOpen��\n");
	printf("if (���������¼�����)�����������ͬʱ����״̬��Ǩ����DoorClosing��\n");
	*state = DoorClosing;
	printf("Transition: from DoorOpen to DoorClosing.\n");
}

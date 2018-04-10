#include <stdio.h>
#include "stdafx.h"
#include "garage.h"

/* DoorClosed状态函数 */
void StateDoorClosed(int *state)
{
	printf("当前状态：【DoorClosed】\n");
	printf("if (【按键】事件发生)，电机上升；同时引起状态变迁到【DoorOpening】。\n");
	*state = DoorOpening;
	printf("Transition: from DoorClosed to DoorOpening.\n");
}

/* DoorOpening状态函数 */
void StateDoorOpening(int *state)
{
	printf("当前状态：【DoorOpening】\n");
	printf("if (门的下边沿距离地面的距离已经大于门的高度)，电机停止；同时引起状态变迁到【DoorOpen】\n");
	printf("if (【按键】事件发生)，电机下降；同时引起状态变迁到【DoorClosing】\n");
	printf("这里模拟后者\n");
	*state = DoorClosing;
	printf("Transition: from DoorOpening to DoorClosing.\n");
}

/* DoorClosing状态函数 */
void StateDoorClosing(int *state)
{
	printf("当前状态：【DoorClosing】\n");
	printf("if (门的下边沿距离地面的距离已经小于0)，电机停止；同时引起状态变迁到【DoorClosed】\n");
	printf("if (【按键】事件发生或红外检测到障碍物)，电机上升；同时引起状态变迁到【DoorOpenging】\n");
	printf("这里模拟后者\n");
	*state = DoorOpening;
	printf("Transition: from DoorClosing to DoorOpening.\n");
}

/* DoorOpen状态函数 */
void StateDoorOpen(int *state)
{
	printf("当前状态：【DoorOpen】\n");
	printf("if (【按键】事件发生)，电机上升；同时引起状态变迁到【DoorClosing】\n");
	*state = DoorClosing;
	printf("Transition: from DoorOpen to DoorClosing.\n");
}

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ElevatorLib.h"

/**********************************************
 三层电梯仿真程序状态函数
 学号：
 姓名：
 日期：
 **********************************************/

/***********************************************
 * 主控循环： 状态机，每隔一定时间(如，100ms)被调用一次，采集系统的运行状态
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
			printf("没有这种状态!!!\n");  
		}
    }
}

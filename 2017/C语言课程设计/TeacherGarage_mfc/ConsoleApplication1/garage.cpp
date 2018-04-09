#include <stdio.h>
#include "stdafx.h"
#include "garage.h"

/* DoorClosed״̬���� */
void StateDoorClosed(int *state)
{
	*state = DoorOpening;
	printf("Transition: from DoorClosed to DoorOpen.\n");
}

/* DoorOpening״̬���� */
void StateDoorOpening(int *state)
{
	*state = DoorClosing;
	printf("Transition: from DoorOpening to DoorClosing.\n");
}

/* DoorClosing״̬���� */
void StateDoorClosing(int *state)
{
	*state = DoorOpening;
	printf("Transition: from DoorClosing to DoorOpening.\n");
}

/* DoorOpen״̬���� */
void StateDoorOpen(int *state)
{
	*state = DoorClosing;
	printf("Transition: from DoorOpen to DoorClosing.\n");
}

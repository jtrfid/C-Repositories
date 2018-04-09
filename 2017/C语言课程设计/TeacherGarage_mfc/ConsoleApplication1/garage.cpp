#include <stdio.h>
#include "stdafx.h"
#include "garage.h"

/* DoorClosed×´Ì¬º¯Êý */
void StateDoorClosed(int *state)
{
	*state = DoorOpening;
	printf("Transition: from DoorClosed to DoorOpen.\n");
}

/* DoorOpening×´Ì¬º¯Êý */
void StateDoorOpening(int *state)
{
	*state = DoorClosing;
	printf("Transition: from DoorOpening to DoorClosing.\n");
}

/* DoorClosing×´Ì¬º¯Êý */
void StateDoorClosing(int *state)
{
	*state = DoorOpening;
	printf("Transition: from DoorClosing to DoorOpening.\n");
}

/* DoorOpen×´Ì¬º¯Êý */
void StateDoorOpen(int *state)
{
	*state = DoorClosing;
	printf("Transition: from DoorOpen to DoorClosing.\n");
}

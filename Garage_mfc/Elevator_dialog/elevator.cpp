#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#include "ElevatorLib.h"

/*
 * Name :        <insert name here>
 * Description : Project 2 - The elevator controller
 */

/*
 * Name :        main()
 * Description : Program entry point.
 */
void main_control()
{
    /*
     * This call starts the elevator system running
     */
    printf("Elevator Startup\n");
    //ElevatorStartup();

    /*
     * This loop runs until we shut the elevator system down
     * by closing the window it runs in.
     */
    if(IsElevatorRunning())
    {


    }

    /*
     * This call shuts down the elevator system
     */
    printf("Elevator Shutdown\n");
    //ElevatorShutdown();
}

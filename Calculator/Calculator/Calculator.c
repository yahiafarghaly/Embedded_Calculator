/*
 * Calculator.c
 *
 * Created: 9/16/2015 11:49:02 PM
 *  Author: Yahia
 */ 

#include "Components/std_types.h"
#include "Components/micro_config.h"
#include "Components/common_macros.h"
#include "Components/initialization.h"
#include "calculator_blocks.h"


	uint8 Flag;/*Contains the state of the program flow*/

int main(void)
{

	InitPeripherals(); /*Initiate the LCD driver*/

    while(1)
    {
		ResetData();
		GetData(); /*Don't get out until pressing equal sign*/
	}
}

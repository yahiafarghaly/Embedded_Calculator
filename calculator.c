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


	uint8 Flag=StartPoint;

int main(void)
{

	InitPeripherals();

    while(1)
    {
		ResetData();
		LCD_clearScreen();
		Flag=StartPoint;
		 while(Flag==StartPoint) GetData();
		 while(Flag==AnalyzePoint) Analyze();
		 while(Flag==CalculatePoint) Calculate();
		 while(Flag==DisplayPoint) DisplayResult();
		 	
	}
	
	
}







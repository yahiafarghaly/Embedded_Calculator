/*
 * calculator_blocks.h
 *
 * Created: 9/17/2015 5:22:01 PM
 *  Author: Yahia
 */ 


#ifndef CALCULATOR_BLOCKS_H_
#define CALCULATOR_BLOCKS_H_

#include "Components/std_types.h"
#include "Components/micro_config.h"
#include "Components/common_macros.h"
#include "Components/lcd.h"
#include "Components/keypad.h"
/*Replace these includes with your own library*/
#include "math.h"

/*The limits on the inputs which you are set*/
#define MaxNumberOfDigits 15
#define CalculatorDynamic 0

enum StageFlag{Reset, Intercept};
enum keys{EnterKey=13,EqualKey= '='};

extern uint8 Flag;

void ResetData(); /*To get all the globel variables in .c file set to zero when doing new operation*/
void GetData(); /*Get the data from the keypad until pressing "equal key"*/
#endif /* CALCULATOR_BLOCKS_H_ */
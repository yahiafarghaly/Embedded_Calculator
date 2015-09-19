# Embedded_Calculator
A simple calculator which can perform Signed operations on 64bits numbers with max of two different math symbols or any number of operands but with the same math symbol.

Last video with sign operand added: https://goo.gl/0wEWUq

# Consist of
	1- 4 * 4 keypad interface.
	2-16*2 LM016L LCD interface.
	3-Avr Atmega 32l.
	4-Atmel studio and proteus 7.7 tool for simulation.

# Functionalities
	1-Can perform (+,-,/,*) operations whether signed or unsigned.
	2- Generic number of operands with only one math operation like (2*2*2*5*..)
  		/*the number of operands can be set before compilation in 
  		calculator_blocks.h in (MaxNumberOfOperands) macro*/
	3-Only can handle two different math operations like 2*3+10 with priority.
	4-Almost detecting the user mistyping and throw exception .

# Project files
	std_types.h /*Contains new types of standards data types*/
	calculator_blocks.h /*Contains the configuarion you set to how 
		much the calculator can handle number of operands,operation*/
	calculator_blocks.c /*Contains the main work of the calculator as will be explain later*/
	Calculator.c /*the main of the application*/
	 

lcd and keypad drivers i don't include it here as you may want to use your own library
you will just replace my functions with yours
#
	-uint8 KeyPad_getPressedKey();
  	-LCD_goToRowColumn(uint8 row, uint8 col);
	-LCD_displayString(uint8* msg);
	
#The main Workflow of the program
	/*this is just a general explaintion of what you will see in the code
		but the code is already commented when needed to be able to trace it*/

	-calculator.c contains globel flag which can contain state of the program flow inside the code
		in enum var as defined in calculator_blocks.h: 
		enum StageFlag{Reset,StartPoint,AnalyzePoint,CalculatePoint,DisplayPoint,ErrorPoint};
  		and by that i cannot move to the next stage until i complete the previous stage 
  		correctly with a flag is set to the next value when sucess. 
  
	-the next idea is that my " uint8 KeyPad_getPressedKey(); " returns one byte of data[0..9] 
		but if the user enters 54578,how i convert it to valuable data to make operations on it? 
		this is done using a simple math trick:
    		1- storing the keypad inputs in MyArr for instance
    		2- then taking the last element
    			MYArr[len-1] + MyArr[len-2]*10 +..+MyArr[len-i]*PowerOf(10,i)= Valuable Number 
    			to use( it's deciaml number of base 10 ;) )
    
	-When the application starts, i stored all the inputs coming from the user inside an array
	   using GetData();then i analyze it using Analyze() and seperate the operands in another 
	   array and the operations as well.

	-then i do the required operation in calculate();then display if the input is valid.

	- Higher range number(greater than signed 32bits) weather inputs or a result won't display 
	   on the LCD correctly due to the limiation in itoa.c standard function, so i modifiy it 
	   to LCD_Signed_Int64_ToString function :) .

# Problems to be fixed
    1-it cannot do more than two different math operation in the same line.
    2-Need an optimized way to do the two different math operation.
    3-Unhandle exception if the result is out of range.(just show the wrong answer)
    
    
    
    
    

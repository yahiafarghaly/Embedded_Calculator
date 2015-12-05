/*
 * calculator_blocks.c
 *
 * Created: 9/17/2015 5:20:44 PM
 *  Author: Yahia
 */
#include "calculator_blocks.h"
sint64 Operand[3];
uint8 Operation[3];
uint8 Sign[3];
uint8 Index=0;
uint8 DigitLength=0;

#if (0 != CalculatorDynamic)
/* disable character limit */
#undef MaxNumberOfDigits
#define MaxNumberOfDigits 256
#endif

void ResetData()
{
	for (uint8 i = 0; i < 3; ++i)
		Operand[i] = Operation[i] = Sign[i] = 0;

	Index = DigitLength = 0;
}
void SynchronousDisplay(uint8 data)
{
	if(data >= 0 && data <= 9) LCD_Signed_Int64_ToString(data);
	else LCD_displayCharacter(data);
}

/* apply operation at index i */
void ApplyOp(uint8 i)
{
	if (Index <= i)
		return;

	if(Sign[i])
	{
		Operand[i] *= -1;
		Sign[i] = 0;
	}
	if(Sign[i + 1])
	{
		Operand[i + 1] *= -1;
		Sign[i + 1] = 0;
	}
	switch (Operation[i])
	{
	case '+':
		Operand[i] += Operand[i + 1];
		break;
	case '-':
		Operand[i] -= Operand[i + 1];
		break;
	case '*':
		Operand[i] *= Operand[i + 1];
		break;
	case '%':
		Operand[i] /= Operand[i + 1];
		break;
	default:
		return;
	}
	Operation[i] = Operation[i + 1];

	if (0 == i && 2 == Index)
	{
		Operand[1] = Operand[2];
		Operation[1] = Operation[2];
		Sign[1] = Sign[2];
	}
	Operand[Index] = Operation[Index] = Sign[Index] = 0;
	--Index;
}
void GetData(void)
{
	Flag = Intercept;
	uint8 temp = 0;
	for (uint8 i = 0; i < MaxNumberOfDigits && Intercept == Flag; ++i)
	{
		temp = KeyPad_getPressedKey();

#if (0 == CalculatorDynamic)
		SynchronousDisplay(temp);
#endif

		if (temp >= 0 && temp <= 9)
		{
			if (0 == DigitLength)
				Operand[Index] = temp;
			else
				Operand[Index] = (Operand[Index] * 10) + temp;

			++DigitLength;
		}
		else if ('+' == temp || '-' == temp)
		{
			if (0 == DigitLength)	/* if we are at the start of a new number */
			{
				if ('-' == temp)
				{
					Sign[Index] ^= 1;	/* flip the sign */
				}                    	/* 'plus sign' has no effect here */
			}
			else
			{
				DigitLength = 0;
				Operation[Index] = temp;
				if (1 == Index)
				{
					ApplyOp(0);
				}
				else if (2 == Index)
				{
					// if ('*' == Operation[0] || '%' == Operation[0])	/* impossible. mul/div are always handled at index = 1 */
					// {
					// 	ApplyOp(0);
					// 	ApplyOp(0);	/* is index one, but after being shifted left once */
					// }
					// else	/* then order may be irrelevant, or 2nd op should occur 1st */
					// {
					// 	ApplyOp(1);
					// 	ApplyOp(0);
					// }

					/* code commented above can be reduced into */
					ApplyOp(1);
					ApplyOp(0);
				}
				++Index;
			}
		}
		else if ('*' == temp || '%' == temp)
		{
			if (0 == DigitLength)
				ThrowException("Invalid Syntax");
			else
			{
				DigitLength = 0;
				Operation[Index] = temp;
				if (1 == Index)
				{
					if ('*' == Operation[0] || '%' == Operation[0])
					{
						ApplyOp(0);
					}
				}
				else if(2 == Index)
				{
					// if ('*' == Operation[0] || '%' == Operation[0])	/* impossible. mul/div's are always handled at index = 1 */
					// {
					// 	ApplyOp(0);
					//
					// 	if ('*' == Operation[0] || '%' == Operation[0])	/* 2nd run, to eliminate index 1 mul/div's  */
					// 	{
					// 		ApplyOp(0);
					// 	}
					// }
					// else if ('*' == Operation[1] || '%' == Operation[1])
					// {
					// 	ApplyOp(1);
					// }
					// else /* impossible, too. two subsequent add/sub's should have been eliminated earlier */
					// {
					// 	ApplyOp(0);
					// }

					/* code commented above can be reduced into */
					ApplyOp(1);
				}
				++Index;
			}
		}
		else if (EqualKey == temp)
		{

#if (0 != CalculatorDynamic)
			LCD_displayCharacter('=');
#endif

			if (Index > 0 && 0 == DigitLength)
				--Index;

			if (1 == Index)
			{
				ApplyOp(0);
			}
			else if (2 == Index)
			{
				// if ('*' == Operation[0] || '%' == Operation[0])	/* impossible. mul/div are always handled at index = 1 */
				// {
				// 	ApplyOp(0);
				// 	ApplyOp(0);	/* is index one, but after being shifted left once */
				// }
				// else	/* then order may be irrelevant, or 2nd op should occur 1st */
				// {
				// 	ApplyOp(1);
				// 	ApplyOp(0);
				// }

				/* code commented above can be reduced into */
				ApplyOp(1);
				ApplyOp(0);
			}

			LCD_goToRowColumn(1, 0);
			LCD_Signed_Int64_ToString(Operand[0]);
			while (EnterKey != KeyPad_getPressedKey());
			LCD_clearScreen();
			Flag = Reset;
		}
		else if (EnterKey == temp)
		{
			LCD_clearScreen();
			Flag = Reset;
		}

#if (0 != CalculatorDynamic)
		if (Intercept == Flag)
		{
			LCD_clearScreen();
			for (int k = 0; k <= Index; ++k)
			{
				if(Sign[k])
					LCD_displayCharacter('-');
				if(0 != Operand[k] || 0 != DigitLength || k != Index)
					LCD_Signed_Int64_ToString(Operand[k]);
				if(0 != Operation[k])
					LCD_displayCharacter(Operation[k]);
			}
		}
#endif

		_delay_ms(300);
	}

	if (Intercept == Flag)
	{
		temp = 0;
		while(EnterKey != temp && EqualKey != temp)
			temp = KeyPad_getPressedKey();

		if (EqualKey == temp)
		{
			LCD_displayCharacter('=');

			if (Index > 0 && 0 == DigitLength)
				--Index;

			if (1 == Index)
			{
				ApplyOp(0);
			}
			else if (2 == Index)
			{
				// if ('*' == Operation[0] || '%' == Operation[0])	/* impossible. mul/div are always handled at index = 1 */
				// {
				// 	ApplyOp(0);
				// 	ApplyOp(0);	/* is index one, but after being shifted left once */
				// }
				// else	/* then order may be irrelevant, or 2nd op should occur 1st */
				// {
				// 	ApplyOp(1);
				// 	ApplyOp(0);
				// }

				/* code commented above can be reduced into */
				ApplyOp(1);
				ApplyOp(0);
			}

			LCD_goToRowColumn(1, 0);
			LCD_Signed_Int64_ToString(Operand[0]);
			while (EnterKey != KeyPad_getPressedKey());
		}
		LCD_clearScreen();
		Flag = Reset;
	}
}

void ThrowException(char *msg)
{
	LCD_goToRowColumn(1, 0);
	LCD_displayString(msg);
	while(KeyPad_getPressedKey() != EnterKey); /* Don't get from the exception until pressing enter key */
	LCD_clearScreen();
	Flag = Reset;
}

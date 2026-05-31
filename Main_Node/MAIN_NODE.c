#include <lpc21xx.h>
#include "CAN.h"
#include "CAN_defines.h"
#include "LCD.h"
#include "DS18B20.h"
#include "EINT.h"
#include "delay.h"
#include "pro_funcs.h"

/*================ GLOBAL VARIABLES ================*/

volatile unsigned char f1 = 0;
volatile unsigned char f2 = 0;
volatile unsigned char f3 = 0;

struct CAN_Frame TX_frame1, TX_frame2, Rx_frame;


/*================ MAIN FUNCTION ================*/

int main()
{
	/* INITIALIZATIONS */

	Init_CAN1();

	init_LCD();

	Interrupt_Init();

	load_right_arrow();

	load_left_arrow();

	IODIR0 |= (1<<21);

	IOSET0 = (1<<21);


	/*================ CAN CONFIG ================*/

	TX_frame1.ID = 1;
	TX_frame1.vbf.RTR = 0;
	TX_frame1.vbf.DLC = 4;

	TX_frame2.ID = 2;
	TX_frame2.vbf.RTR = 0;
	TX_frame2.vbf.DLC = 8;


	/*================ STARTUP SCREEN ================*/

	vehicle();

	cmd_LCD(0x01);


	/*================ MAIN LOOP ================*/

	while(1)
	{

		/*================ LEFT INDICATOR ================*/

		if(f1==1 && f2==0 && f3==0)
		{
			Left_Indicator();

			/* CAN TRANSMISSION */

			TX_frame1.Data1 = 1;
			TX_frame1.Data2 = 0;

			CAN1_Tx(TX_frame1);

			delay_ms(100);
		}



		/*================ RIGHT INDICATOR ================*/

		else if(f2==1 && f1==0 && f3==0)
		{
			Right_Indicator();

			/* CAN TRANSMISSION */

			TX_frame1.Data1 = 2;
			TX_frame1.Data2 = 0;

			CAN1_Tx(TX_frame1);

			delay_ms(100);
		}



		/*================ REVERSE MODE ================*/

		else if(f3==1 && f1==0 && f2==0)
		{
			/* REQUEST DISTANCE */

			TX_frame2.Data1 = 1;
			TX_frame2.Data2 = 0;

			CAN1_Tx(TX_frame2);


			/* RECEIVE DISTANCE */

			if(C1GSR & 0x01)
			{
				CAN1_Rx(&Rx_frame);

				if(Rx_frame.ID == 3)
				{
					/* DISPLAY REVERSE UI */

					Reverse_Mode(Rx_frame.Data1);
				}
			}

			delay_ms(100);
		}



		/*================ NORMAL DASHBOARD ================*/

		else
		{
			Vehicle_Dashboard();

			delay_ms(200);
		}
	}
}


#include <lpc21xx.h>
#include "CAN.h"
#include "CAN_defines.h"
#include "delay.h"

int i, j;

struct CAN_Frame RX_frame;


void LEFT_IND(void)
{
    IOSET0 = 0xFF;

    for(i = 0; i < 8; i++)
    {
        IOCLR0 = 1 << i;

        delay_ms(100);
    }
}


void RIGHT_IND(void)
{
    IOSET0 = 0xFF;

    for(j = 7; j >= 0; j--)
    {
        IOCLR0 = 1 << j;

        delay_ms(100);

        if(j == 0)
        {
            break;
        }
    }
}


void IND_OFF(void)
{
    IOSET0 = 0xFF;
}


int main()
{
    // CAN INITIALIZATION
    Init_CAN1();

    // LED PINS OUTPUT
    IODIR0 |= 0xFF;

    // INITIAL OFF
    IND_OFF();

    while(1)
    {
        // CHECK CAN RECEIVE BUFFER
        if(C1GSR & 0x01)
        {
            // RECEIVE FRAME
            CAN1_Rx(&RX_frame);

            // CHECK FRAME ID
            if(RX_frame.ID == 1)
            {

                /*================ RIGHT =================*/

                if(RX_frame.Data1 == 2 && RX_frame.Data2 == 0)
                {
                    RIGHT_IND();
                }


                /*================ LEFT =================*/

                else if(RX_frame.Data1 == 1 && RX_frame.Data2 == 0)
                {
                    LEFT_IND();
                }


                /*================ OFF =================*/

                else if(RX_frame.Data1 == 0 && RX_frame.Data2 == 0)
		    	{
                    IND_OFF();
                }
            }
	    	else IND_OFF();
        }
		else IND_OFF();
    }
}


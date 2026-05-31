#include <lpc21xx.h>
#include "delay.h"
#include "CAN.h"
#include "CAN_defines.h"
#include "ultrasonic.h"

 float distance;

struct CAN_Frame Tx_frame;
struct CAN_Frame Rx_frame;

int main()
{

    Init_CAN1();

    init_Ultrasonic();

	IODIR0|=1<<0;
	IOSET0=1<<0;


    while(1)
    {
        // CHECK RECEIVE BUFFER
        if(C1GSR & 0x01)
        {
            CAN1_Rx(&Rx_frame);

            // REVERSE MODE ON
            if((Rx_frame.ID == 2) && (Rx_frame.Data1 == 1))
            {
				IOCLR0=1<<0;
                distance = dist_cal();

                // LIMIT TO 255
                if(distance > 255)
                {
                    distance = 255;
                }

				

				Tx_frame.ID = 3;
    			Tx_frame.vbf.RTR = 0;
    			Tx_frame.vbf.DLC = 8;

                // SEND DISTANCE
                Tx_frame.Data1 = (unsigned int)distance;
                Tx_frame.Data2 = 0;

                CAN1_Tx(Tx_frame);

                delay_ms(100);
            }

            // REVERSE MODE OFF
        else if((Rx_frame.ID == 2) && (Rx_frame.Data1 == 0))
            {
                distance = 0;
            }
        }
    }
}


#ifndef __CAN_H__
#define __CAN_H__

struct CAN_Frame
{
	unsigned int ID;
	struct BitField
	{
		unsigned int RTR : 1;
		unsigned int DLC : 4;  
	}vbf; 
	unsigned int Data1,Data2;					//8-bytes
};

void Init_CAN1(void);
void CAN1_Tx(struct CAN_Frame);
void CAN1_Rx(struct CAN_Frame *);
#endif

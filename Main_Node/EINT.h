/*====================================================================
 *              EXTERNAL INTERRUPT DECLARATIONS
 *====================================================================*/

extern volatile unsigned char f1, f2, f3;

/*-------------------- ISR PROTOTYPES --------------------*/
void EINT0_ISR(void) __irq;         // ISR for External Interrupt 0
void EINT1_ISR(void) __irq;         // ISR for External Interrupt 1
void EINT2_ISR(void) __irq;         // ISR for External Interrupt 2

/*-------------------- INITIALIZATION --------------------*/
void Interrupt_Init(void);          // Function to initialize interrupts

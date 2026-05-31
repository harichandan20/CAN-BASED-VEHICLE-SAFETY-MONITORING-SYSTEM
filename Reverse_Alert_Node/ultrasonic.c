#include <lpc21xx.h>
#include "delay.h"

#define trr_pin 20
#define echo_pin 21

void init_Ultrasonic(void)
{
    IODIR0 |= (1<<trr_pin);
    IODIR0 &= ~(1<<echo_pin);

    T0PR = 14;      // 1us timer tick
}

void send_pulse(void)
{
    IOSET0 = (1<<trr_pin);

    delay_us(10);

    IOCLR0 = (1<<trr_pin);
}

unsigned int read_echo(void)
{
    unsigned int duration = 0;

    while(((IOPIN0>>echo_pin)&1)==0);

    T0TCR = 0x02;      // reset timer
    T0TCR = 0x01;      // start timer

    while(((IOPIN0>>echo_pin)&1)==1);

    T0TCR = 0x00;      // stop timer

    duration = T0TC;

    return duration;
}

float dist_cal(void)
{
    unsigned int pulse;
    float dis;

    send_pulse();

    pulse = read_echo();

    dis = (float)pulse / 58.0;

    return dis;
}


#include <lpc21xx.h>
#include "LCD.h"
#include "DS18B20.h"
#include "delay.h"

/*================ GLOBAL VARIABLES ================*/

int integer, decimal;
int temp;


/*================ RIGHT ARROW CGRAM ================*/

void load_right_arrow()
{
    cmd_LCD(0x40);

    char_LCD(0x04);
    char_LCD(0x06);
    char_LCD(0x1F);
    char_LCD(0x1F);
    char_LCD(0x06);
    char_LCD(0x04);
    char_LCD(0x00);
    char_LCD(0x00);
}


/*================ LEFT ARROW CGRAM ================*/

void load_left_arrow()
{
    cmd_LCD(0x48);

    char_LCD(0x04);
    char_LCD(0x0C);
    char_LCD(0x1F);
    char_LCD(0x1F);
    char_LCD(0x0C);
    char_LCD(0x04);
    char_LCD(0x00);
    char_LCD(0x00);
}


/*================ LEFT INDICATOR MOVE ================*/

void left_arrow_move()
{
    int pos;

    for(pos = 8; pos >= 6; pos--)
    {
        cmd_LCD(0xD4 + pos);

        char_LCD(1);

        delay_ms(180);
    }

    for(pos = 8; pos >= 6; pos--)
    {
        cmd_LCD(0xD4 + pos);

        char_LCD(' ');
    }
}


/*================ RIGHT INDICATOR MOVE ================*/

void right_arrow_move()
{
    int pos;

    for(pos = 17; pos <= 19; pos++)
    {
        cmd_LCD(0xD4 + pos);

        char_LCD(0);

        delay_ms(180);
    }

    for(pos = 17; pos <= 19; pos++)
    {
        cmd_LCD(0xD4 + pos);

        char_LCD(' ');
    }
}


/*================ STARTUP SCREEN ================*/

void vehicle(void)
{
	int i;

	cmd_LCD(0x01);

	cmd_LCD(0x80);
	str_LCD(" SMART VEHICLE   ");

	cmd_LCD(0xC0);
	str_LCD(" SYSTEM READY... ");

	cmd_LCD(0x94);
	str_LCD("[");

	for(i=0;i<10;i++)
	{
		char_LCD(255);

		delay_ms(120);
	}

	str_LCD("]");

	cmd_LCD(0xD4);
	str_LCD(" READY TO DRIVE  ");

	delay_s(2);

	cmd_LCD(0x01);
}


/*================ DASHBOARD ================*/

void Vehicle_Dashboard(void)
{
	temp = ReadTemp();

	integer = temp/16;

	decimal = (((int)temp%16)*10)/16;


	/* LINE 1 */

	cmd_LCD(0x80);

	str_LCD(" VEH DASHBOARD   ");


	/* LINE 2 */

	cmd_LCD(0xC0);

	str_LCD("TMP:");

	num_LCD(29);

	char_LCD('.');

	num_LCD(5);

	char_LCD(0xDF);

	char_LCD('C');

	str_LCD("   ");

	cmd_LCD(0xC0 + 11);

	str_LCD("FWD  ");


	/* LINE 3 */

	cmd_LCD(0x94);

	str_LCD("ENGINE STATUS:");

	cmd_LCD(0x94 + 15);

	if(integer < 40)
	{
		str_LCD("SAFE ");
	}
	else if(integer >=40 && integer <70)
	{
		str_LCD("WARM!");
	}
	else
	{
		str_LCD("HOT!!");
	}


	/* LINE 4 */

	cmd_LCD(0xD4);

	str_LCD("IND:");

	/* CLEAR LAST LINE */

	cmd_LCD(0xD4 + 4);

	str_LCD("                ");


	/* LEFT SIDE */

	cmd_LCD(0xD4 + 5);

	char_LCD(1);
	char_LCD(1);
	char_LCD(1);

	cmd_LCD(0xD4 + 10);

	str_LCD("L");


	/* RIGHT SIDE */

	cmd_LCD(0xD4 + 13);

	str_LCD("R");

	cmd_LCD(0xD4 + 17);

	char_LCD(0);
	char_LCD(0);
	char_LCD(0);
}


/*================ LEFT INDICATOR ================*/

void Left_Indicator(void)
{
	Vehicle_Dashboard();

	cmd_LCD(0xD4 + 5);

	str_LCD("   ");

	if(integer < 70)
	{
		left_arrow_move();
	}

	cmd_LCD(0xD4 + 5);

	char_LCD(1);
	char_LCD(1);
	char_LCD(1);
}


/*================ RIGHT INDICATOR ================*/

void Right_Indicator(void)
{
	Vehicle_Dashboard();

	cmd_LCD(0xD4 + 17);

	str_LCD("   ");

	if(integer < 70)
	{
		right_arrow_move();
	}

	cmd_LCD(0xD4 + 17);

	char_LCD(0);
	char_LCD(0);
	char_LCD(0);
}


/*================ REVERSE MODE ================*/

void Reverse_Mode(int dist)
{
	cmd_LCD(0x01);

	cmd_LCD(0x80);

	str_LCD("<< REVERSE MODE>>");


	/* LINE 2 */

	cmd_LCD(0xC0);

	str_LCD("DIST:");

	num_LCD(dist);

	str_LCD(" CM");

	cmd_LCD(0xC0 + 12);

	str_LCD("BACK");


	/* SAFE */

	if(dist > 50)
	{
		cmd_LCD(0x94);

		str_LCD("( (          ) ) ");

		cmd_LCD(0xD4);

		str_LCD(" SAFE TO MOVE    ");
	}


	/* OBJECT NEARBY */

	else if(dist <=50 && dist >20)
	{
		cmd_LCD(0x94);

		str_LCD("(( ((    )) ))  ");

		cmd_LCD(0xD4);

		str_LCD(" OBJECT NEARBY   ");
	}


	/* VERY CLOSE */

	else if(dist <=20 && dist >10)
	{
		cmd_LCD(0x94);

		str_LCD("((((((  ))))))  ");

		cmd_LCD(0xD4);

		str_LCD(" MOVE SLOWLY     ");
	}


	/* WARNING */

	else
	{
		cmd_LCD(0x80);

		str_LCD(" !!! WARNING !!!");

		cmd_LCD(0xC0);

		str_LCD("DIST:");

		num_LCD(dist);

		str_LCD(" CM");

		cmd_LCD(0xC0 + 12);

		str_LCD("BACK");

		cmd_LCD(0x94);

		str_LCD("((((((((()))))))");

		cmd_LCD(0xD4);

		str_LCD(" APPLY BRAKES !!");
	}
}

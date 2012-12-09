 /* Description	: LED controller. It show some numbers for debug.
 *	Author		: Nate Hu
 *	Data		: 14 Nov,2012
 */

#include <DB1.H>
#include "api.h"
unsigned char table[]=
			{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
			//table 0-9 A-F  HEX
sbit led_a = P0 ^ 1;//A
sbit led_b = P0 ^ 5;//B
sbit led_c = P2 ^ 3;//C
sbit led_d = P2 ^ 1;//D
sbit led_e = P2 ^ 0;//E
sbit led_f = P0 ^ 2;//F
sbit led_g = P2 ^ 4;//G
sbit led_p = P2 ^ 2;//P

sbit led_1 = P0 ^ 0;
sbit led_2 = P0 ^ 3;
sbit led_3 = P0 ^ 4;
sbit led_4 = P2 ^ 5;
#define LED_DELAY (20)
#define DY_P0M1SET		0x00 //设置I/O口工作方式//00100010
#define DY_P0M0SET		0x19 //设置I/O口工作方式//10000000（PxM0为1时设置为强推）
//#define DY_P1M1SET		0x00 //设置I/O口工作方式//00000000（左到右，高到低位）
//#define DY_P1M0SET		0x00 //设置I/O口工作方式//10000000
#define DY_P2M1SET		0x00 //设置I/O口工作方式//01000100
#define DY_P2M0SET		0x20 //设置I/O口工作方式//10000000


void dis_off (void){
	P0 = ~DY_P0M0SET; //关所有显示
	P2 = ~DY_P2M0SET; //关所有显示
//	delay(10-DY_PWM); 
}
void show_one_num(unsigned char val)//show one digit
{
	unsigned char index = val & 0x0f;
	unsigned char num = table[index];
	led_a = num & 0x1;
	led_b = (num >> 1) & 0x1;
	led_c = (num >> 2) & 0x1;
	led_d = (num >> 3) & 0x1;
	led_e = (num >> 4) & 0x1;
	led_f = (num >> 5) & 0x1;
	led_g = (num >> 6) & 0x1;
}
void show_byte(unsigned short val)//show two digit,  pos 1: high byte, 0 low byte
{
	

	
		led_1 = 1;
		show_one_num(val>>12);
		delay(LED_DELAY);
		//show_one_num(0xff);
		dis_off();
		led_1 = 0;
		delay(LED_DELAY);

		led_2 = 1;
		show_one_num(val>>8);
		delay(LED_DELAY);
		//show_one_num(0xff);
		dis_off();
		led_2 = 0;
		delay(LED_DELAY);

	
		led_3 = 1;
		show_one_num(val>>4);
		delay(LED_DELAY);
		//show_one_num(0xff);
		dis_off();
		led_3 = 0;
		delay(LED_DELAY);
		led_4 = 1;
		show_one_num(val);
		delay(LED_DELAY);
		//show_one_num(0xff);
		dis_off();
		led_4 = 0;
		delay(LED_DELAY);

}
void init_led()
{
	//show_byte(0,1);
	//show_byte(0,0);
	P0M0 = DY_P0M0SET; //设置I/O口工作方式（行为推挽，列为普通输入/输出）
	P0M1 = DY_P0M1SET;
	//P1M0 = DY_P1M0SET;
	//P1M1 = DY_P1M1SET;
	P2M0 = DY_P2M0SET;
	P2M1 = DY_P2M1SET;
	P0 = 0xff;//IO接口初始为高电平
	P2 = 0xff;
	led_1 = 0;
	led_2 = 0;
	led_3 = 0;
	led_4 = 0;
}

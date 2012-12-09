/* Description	: control steering. Use PWM,  0.5 - 2.5ms high level, total is 20ms.	 0.5ms high level is 0 degree
			  2.5ms high level is 180 degree.
 *	Author		: Nate Hu
 *	Data		: 21 Nov,2012
 */

#include <DB1.H>
#include "api.h"
#define S_P5M0 0x4//0x4 
#define	S_P5M1 0x0

/*We need this offset.Degree must be 0x1b to 0xd2. So use 0x1d*/
#define OFFSET (0xB * 0x1D) 
sbit S_PWM = P5 ^ 2;
unsigned int degree = 0; //0 - 180

bit out = 0;
extern unsigned int cmd_func;
extern bit sg_flag;
void init_steering()
{
	/* Use timer 1*/
	P5M0 = S_P5M0;
	P5M1 = S_P5M1;

	S_PWM = 0;

	TMOD &= ~0xF0;
	TMOD |= 0x10;//MODE 1,16bit timer
	TH1 = 0xFF;
	TL1	= 0xF0;

//	AUXR &= ~0x40;
//	AUXR |= 0x40; 

 	ET1 = 1;
	EA = 1;
}

void steering_timer1()interrupt 3 using 1
{
	unsigned int val;
	if(out == 0){
		val = 0xB * degree + 0x1F3 +OFFSET;//0xB is one degree(2.5 - 0.5)*1000/180,  0x1F4 means 0.5ms
		val = 0xFFFF - val;
		TH1 = val / 0xFF;
		TL1 = val % 0xFF;
		S_PWM = 1;
		out = 1;
	}
	else {
		val = 0x4E1F - 0x1F3 - 0xB * degree - OFFSET;//0x4E20 means 20ms
		val = 0xFFFF - val;
		TH1 = val / 0xFF;
		TL1 = val % 0xFF;
		S_PWM = 0;
		out = 0;
	}				 
			
}
/**********************************************************
turn up steering gear 

***********************************************************/
void up()
{
	cmd_func = UP;
	/* max degree is 90*/
	if( degree < 90){
		degree = degree + 10;
	} 
	//degree += 5;
	cmd_func = degree;
	sg_flag = 1;
	TR1 = 1;
	
	//set_pwm1(100);

}
/**********************************************************
turn down steering gear 

***********************************************************/
void down()
{
	cmd_func = DOWN;
	if(degree > 0){
		degree = degree - 10;
	} 
	//degree -= 5;
	cmd_func = degree;
	sg_flag = 1;
	TR1 = 1;

	//set_pwm1(0);
}

void stop_sg()
{
	TR1 = 0;
	S_PWM = 0;

}
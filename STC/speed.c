/* Description	: adjust the speed to let car go straght. Use two IR sensors.
			  
 *	Author		: Nate Hu
 *	Data		: 25 Nov,2012
 */

#include <DB1.H>
#include "api.h"


sbit left_ir = P3 ^ 2;
sbit right_ir = P3 ^ 3;

unsigned int left_dis = 0;
unsigned int right_dis = 0;

unsigned int left_diff = 0;
unsigned int right_diff = 0;
void init_speed()
{
	IT0 = 1;
	IT1 = 1;
	EX0 = 0;
	EX1 = 0;
	PX0 = 1;
	PX1 = 1;
	EA = 1;

	//enable_speed();
}
void enable_speed()
{
	EX0 = 1;
	EX1 = 1;
	right_dis = 0;
	left_dis = 0;
}
void disable_speed()
{
	EX0 = 0;
	EX1 = 0;

}
void left_int()interrupt 0
{
   unsigned int n;
   left_dis++;
   if(left_dis < right_dis){
   		n =  right_dis - left_dis;
		if(n*10 > PWM_MAX)
			n = 0;
		else
			n = PWM_MAX - n * 10;
   		/* increase left speed*/
		
		set_pwm0(100);
		set_pwm1(n);
   }
   else
   {
        n = left_dis - right_dis;
		if(n*10 > PWM_MAX)
			n = 0;
		else
			n = PWM_MAX - n * 10;
   		/* increase right speed*/
	
		set_pwm1(100);
		set_pwm0(n);
   }

}

void right_int()interrupt 2
{
	unsigned int n;
   right_dis++;
   if(right_dis < left_dis){
      	n = left_dis - right_dis;
		if(n*10 > PWM_MAX)
			n = 0;
		else
			n = PWM_MAX - n * 10;
   		/* increase right speed*/
	
		set_pwm1(100);
		set_pwm0(n);
   }
   else
   {
      	n =  right_dis - left_dis;
		if(n*10 > PWM_MAX)
			n = 0;
		else
			n = PWM_MAX - n * 10;
   		/* increase left speed*/
		
		set_pwm0(100);
		set_pwm1(n);
   }
}
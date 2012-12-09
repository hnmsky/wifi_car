/* Description	: control motor.There are two motors on the car.
 *	Author		: Nate Hu
 *	Data		: 18 Nov,2012
 */

#include <DB1.H>
#include "api.h"
#define MOTOR_P4M0_NOR 0x0//normal mode 
#define MOTOR_P4M1_NOR 0x0
#define MOTOR_P4M0_PULL 0xf//normal mode 
#define MOTOR_P4M1_PULL 0x0
#define MOTOR_P4M0_INPUT 0x0//input mode 
#define MOTOR_P4M1_INPUT 0x33//11 1100
#define MOTOR_IN_MAST 0x33//P0 P1 P4 P5

#define MOTOR_P4SW 0x30

enum motor_state{
	MOTOR_STOP,
	MOTOR_FORWARD,
	MOTOR_BACK,
	MOTOR_LEFT,
	MOTOR_RIGHT,

};
sbit in1 = P4 ^ 0;
sbit in2 = P4 ^ 1;
sbit in3 = P4 ^ 4;
sbit in4 = P4 ^ 5;
sbit ena = P4 ^ 2;
sbit enb = P4 ^ 3;
unsigned int cmd_func = 0;
unsigned char motor_stat = MOTOR_STOP;
bit sg_flag = 0;
extern unsigned int degree;


void init_pwm();
/**************************************************************
* Set left motor forward
***************************************************************/
void left_forward()
{
	in1 = 0;
	in2 = 1;
}
/**************************************************************
* Set left motor reverse
***************************************************************/
void left_reverse()
{
	in1 = 1;
	in2 = 0;
}
/**************************************************************
* Set right motor forward
***************************************************************/
void right_forward()
{
	in3 = 1;
	in4 = 0;
}
/**************************************************************
* Set right motor reverse
***************************************************************/
void right_reverse()
{
	in3 = 0;
	in4 = 1;
}

void init_motor()
{
	stop();
	P4SW = MOTOR_P4SW;
	init_pwm();
	
}
/**************************************************************
* Config the pins, prepare to move
***************************************************************/
void start()
{
	
	P4M0 &= ~MOTOR_IN_MAST;
	P4M0 |= MOTOR_P4M0_NOR & MOTOR_IN_MAST;
	P4M1 &=  ~MOTOR_IN_MAST;
	P4M1 |=  MOTOR_P4M1_NOR & MOTOR_IN_MAST;
	start_pwm();
	set_pwm0(100);
	set_pwm1(100);
	
}
/**************************************************************
* stop
***************************************************************/
void stop()
{
	motor_stat = MOTOR_STOP;
	cmd_func = STOP;
	//in1 = 0;
	//in2 = 0;
	//in3 = 0;
	//in4 = 0;
	disable_speed();
	stop_pwm();

	
	/* set iniput */	
	P4M0 &= ~MOTOR_IN_MAST;
	P4M0 |= MOTOR_P4M0_INPUT & MOTOR_IN_MAST;
	P4M1 &=  ~MOTOR_IN_MAST;
	P4M1 |=  MOTOR_P4M1_INPUT & MOTOR_IN_MAST;	
}
/**************************************************************
* move forward
***************************************************************/
void forward()
{
	/* for same commands,only the first one is valid*/
	if(motor_stat == MOTOR_FORWARD)
		return;
	else{
		motor_stat = MOTOR_FORWARD;
	}

	start();
	cmd_func = FORWARD;

	left_forward();
	right_forward();
	enable_speed();
}
/**************************************************************
* move back
***************************************************************/
void back()
{
	if(motor_stat == MOTOR_BACK)
		return;
	else{
		motor_stat = MOTOR_BACK;
	}
	start();
	cmd_func = BACK;
	left_reverse();
	right_reverse();
	enable_speed();
}
/**************************************************************
*turn right 
***************************************************************/
void right()
{
	if(motor_stat == MOTOR_RIGHT)
		return;
	else{
		motor_stat = MOTOR_RIGHT;
	}
	start();
	cmd_func = RIGHT;
	disable_speed();
	left_forward();
	right_reverse();
	
	set_pwm0(70);
	set_pwm1(70);

}
/**************************************************************
* 	turn left
***************************************************************/
void left()
{
	if(motor_stat == MOTOR_LEFT)
		return;
	else{
		motor_stat = MOTOR_LEFT;
	}

	start();
	cmd_func = LEFT;
	disable_speed();
	right_forward();
	left_reverse();
	
	
	set_pwm0(70);
	set_pwm1(70);			    
}


void init_pwm()
{
	CMOD = 0x4;//use timer 0
	CH = 0;
	CL = 0;
	CCAPM0 = 0x42;//8 bit pwm, no interrupt
	CCAPM1 = 0x42;//8 bit pwm, no interrupt

	PCA_PWM0 = 0x33;
	PCA_PWM1 = 0x33;

	CCAP0L = 0xFF;
	CCAP0H = 0xFF;
	CCAP1L = 0xFF;
	CCAP1H = 0xFF;

 	/* init timer 0 */
	TMOD &= ~0x0F;
	TMOD |= 0x02;
	TL0 = 0x0;//PCA frequecy is Fos / 256
	TH0 = 0x0;

	AUXR &= ~0x80;
	AUXR |= 0x80;
	//CR = 1;

	/* set pwm to p4 port*/
	AUXR1 &= ~0x40;
	AUXR1 |= 0x40;

    /* set pwm pin to low */
	//ena = 0;
	//enb = 0;
}
void start_pwm()
{
	PCA_PWM0 = 0x0;
	PCA_PWM1 = 0x0;
	TR0 = 1;
	CR = 1;
}
void stop_pwm()
{
 	PCA_PWM0 = 0x33;
	PCA_PWM1 = 0x33;
	TR0 = 0;
	CR = 0;
	/* set pwm pin to low */
	//ena = 0;
	//enb = 0;
}
/*******************************************
(0, CL) < (EPC0L,CCAP0L)  output is 0
 (0, CL) >= (EPC0L,CCAP0L)  output is 1
 val is Duty Cycle, 0 to 100
 pwm0 is left
*******************************************/
void set_pwm0(unsigned char val)
{
	unsigned int ccap;
	

	if(val > PWM_MAX){//output is 1
		//PCA_PWM1 = 0;
		CCAP0L = 0;
		CCAP0H = 0;
		
	}
	else if(val < PWM_MIN) {//output is 0
		//PCA_PWM1 = 0x03;	
		CCAP0L = 0xFF;
		CCAP0H = 0xFF;
		
	}
	else {
		ccap = 0xFF - val * 0xFF / 100;
		//PCA_PWM1 = 0;
		CCAP0L = ccap;
		CCAP0H = ccap;
		
	}
	
	
} 
/*******************************************
(0, CL) < (EPC1L,CCAP1L)  output is 0
 (0, CL) >= (EPC1L,CCAP1L)  output is 1
 val is Duty Cycle, 0 to 100
  pwm1 is right
*******************************************/
void set_pwm1(unsigned char val)
{
	unsigned int ccap;
	
	if(val > PWM_MAX){//output is 1
		CCAP1L = 0;
		CCAP1H = 0;
	}
	else if(val < PWM_MIN) {//output is 0
		//PCA_PWM1 = 0x33;
		CCAP1L = 0xFF;
		CCAP1H = 0xFF;	
	}
	else {
		ccap = 0xFF - val * 0xFF / 100;
		CCAP1L = ccap;
		CCAP1H = ccap;
	}
	
	
}


#ifndef __API_H__
#define __API_H__

#define START0 'Z'//start ZY
#define START1 'Y'
#define END0	'W'//end WV
#define END1	'V'
#define FORWARD	'F'
#define BACK	'B'
#define LEFT	'L'
#define RIGHT	'R'
#define UP		'U'
#define DOWN	'D'
#define STOP	'S'

#define PWM_MIN  30 //min is 10%
#define	PWM_MAX	 90 //max is 90%

void delay(unsigned int val);
void show_byte(unsigned short val);
void init_led();
void delay1ms (unsigned int a);
void init_motor();
void init_steering();
void stop_sg();
//void led_vcc_on();

void init_motor();
void forward();
void back();
void left();
void right();
void stop();
void up();
void down();
void start_pwm();
void stop_pwm();
void set_pwm0(unsigned char val);//left
void set_pwm1(unsigned char val);//right

void init_speed();
void enable_speed();
void disable_speed();

void uart2_init();
#endif
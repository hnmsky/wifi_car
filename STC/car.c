 /* Description	: routine for wifi car
 *	Author		: Nate Hu
 *	Data		: 12 Nov,2012
 */
#include <DB1.H>
#include "api.h"
extern bit sg_flag;
extern unsigned int cmd_func;
extern unsigned int left_dis;
extern unsigned int right_dis;	
extern unsigned char uart_data;
extern unsigned int left_diff;
extern unsigned int right_diff;
void delay(unsigned int val)
{
	
	while(val--);
}
void delay1ms (unsigned int a){ // 1ms延时程序（12MHz 10倍于51单片机速度时）
	unsigned int i;
	while( --a != 0){
		for(i = 0; i < 600; i++); 
	}   				   
}

void delay1ms_show (unsigned int a, unsigned int val){ // 1ms延时程序（12MHz 10倍于51单片机速度时）
	unsigned int i;
	while( --a != 0){
		for(i = 0; i < 600; i++); 
		show_byte((cmd_func << 8) | (val & 0xff));
	}   				   
}
 void main()
 {
 	unsigned short val = 0;
	unsigned int show_data = 0;
	//unsigned short last_data = 0;
 	uart2_init();
	init_led();
	init_motor();
	init_steering();
	init_speed();
	show_byte(0);

	//led_vcc_on();
 	while(1){
	//		show_byte(0x12,1);
		
		if((uart_data >= '0') && (uart_data <= 'z')){
			//last_data = val;
			val = uart_data;
		}
		//show_data = (cmd_func << 8) | (val & 0xff);
		show_data = ((left_dis << 8)&0xff00) | (right_dis & 0xff);
		//show_data = (left_diff << 8) | (right_diff & 0xff);

		show_byte(show_data);

		if(sg_flag == 1){
			sg_flag = 0;
			delay1ms_show(1000 * 3,show_data);
			if(sg_flag == 0){//maybe call up() twice in 3s
				stop_sg();
			}
			 	
		
		}
		//uart2_send('a');
		//uart2_send('b');
		//uart2_send('c');
	}
 }
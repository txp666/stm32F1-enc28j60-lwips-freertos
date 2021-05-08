#include "includes.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "malloc.h"
#include "lwip_comm.h"
#include "bsp_delay.h"
#include "tcp_client.h"
#include "key.h" 
#include "lcd.h"
#include "usmart.h"
#include "sram.h"
#include "led.h"
#include "mqttclient.h"
#include "cJSON.h"

u8 cnt_flag=0,sub_flag=0,pub_flag=0;
u16 time=0;

#define MAIN_TASK_PRIO   ( tskIDLE_PRIORITY + 1 )
#define display_TASK_PRIO   ( tskIDLE_PRIORITY + 3 )
#define DHCP_TASK_PRIO   ( tskIDLE_PRIORITY + 2 )

void Main_task(void *pvParameters);
void display_task(void *pdata);
void show_address(u8 mode);


int main(void)
{ 
	delay_init();	    				//延时函数初始化	 
	DWT_init();
	uart_init(115200);					//初始化串口
	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD 
	usmart_dev.init(72);		//初始化USMART	
	FSMC_SRAM_Init();			//初始化外部SRAM	 
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池
 	POINT_COLOR=RED;			//设置字体为红色 
	LCD_ShowString(30,30,200,16,16,"ENC28J60+STM32");	
	LCD_ShowString(30,50,200,16,16,"LWIP+FreeRTOS+MQTT");	
	LCD_ShowString(30,70,200,16,16,"TXP");
	LCD_ShowString(30,90,200,16,16,"2021/4/15");  
	POINT_COLOR = BLUE; 	//蓝色字体
	while(lwip_comm_init()) //lwip初始化
	{
			LCD_ShowString(30,110,200,20,16,"LWIP Init Falied!");
		  delay_xms(1200);
		  LCD_Fill(30,110,230,130,WHITE); //清除显示
		  LCD_ShowString(30,110,200,16,16,"Retrying..."); 
	}
	LCD_ShowString(30,110,200,20,16,"LWIP Init Success!");	//lwip初始化成功
	xTaskCreate(Main_task,"Main", configMINIMAL_STACK_SIZE * 2, NULL, MAIN_TASK_PRIO, NULL);
	vTaskStartScheduler();
}
void Main_task(void *pvParameters)
{	
	mqtt_thread_init();
#if 0
	xTaskCreate(LwIP_DHCP_task, "DHCP", configMINIMAL_STACK_SIZE * 2, NULL,DHCP_TASK_PRIO, NULL);	
#endif
	xTaskCreate(display_task, "dispaly", configMINIMAL_STACK_SIZE * 2, NULL,display_TASK_PRIO, NULL);
}
//显示地址等信息
void display_task(void *pdata)
{
	while(1)
	{ 
#if LWIP_DHCP									//当开启DHCP的时候
		if(lwipdev.dhcpstatus != 0) 			//开启DHCP
		{
			show_address(lwipdev.dhcpstatus );	//显示地址信息  
#else
		show_address(0); 						//显示静态地址			   	
#endif //LWIP_DHCP
		}
	}
}


void show_address(u8 mode)
{
	u8 buf[30];
	if(mode==2)
	{
		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(30,130,210,16,16,buf); 
		sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		LCD_ShowString(30,150,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		LCD_ShowString(30,170,210,16,16,buf); 
	}
	else 
	{
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(30,130,210,16,16,buf); 
		sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		LCD_ShowString(30,150,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		LCD_ShowString(30,170,210,16,16,buf); 
	}	
} 


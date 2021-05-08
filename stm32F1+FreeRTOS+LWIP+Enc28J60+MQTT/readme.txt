实验器材:
	Mini STM32F103 开发板
	
实验目的:
	学习使用FreeRTOS的静态任务创建 xTaskCreateStatic()和任务删除函数vTaskDelete()。
	
硬件资源:
	1,DS0(连接在PA8)，DS1(连接在PD2上)
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面) 
	3,ALIENTEK 2.8/3.5/4.3寸LCD模块(仅支持MCU屏) 
	
实验现象:
	实验中会创建两个应用任务task1_task和task2_task，当task1_task运行
	5次以后就会删除掉任务task2_task。

注意事项:
	无. 

参考资料：STM32F103 FreeRTOS开发手册.pdf 第六章


-------------------------------------------------------------------------------------------

◆其他重要连接：
  开发板光盘资料下载地址（视频+文档+源码等）：http://www.openedv.com/posts/list/13912.htm


◆友情提示：如果您想以后及时免费的收到正点原子所有开发板资料更新增加通知，请关注微信公众平台：
 2种添加方法：（动动手提升您的学习效率，惊喜不断哦）
（1）打开微信->添加朋友->公众号->输入“正点原子”->点击关注
（2）打开微信->添加朋友->输入“alientek_stm32"->点击关注
 具体微信添加方法，请参考帖子：http://www.openedv.com/posts/list/45157.htm
 


						

						淘宝店铺： http://openedv.taobao.com
						           http://eboard.taobao.com
						公司网站：www.alientek.com
						技术论坛：www.openedv.com
                                                微信公众平台：正点原子
						电话：020-38271790
						传真：020-36773971
						广州市星翼电子科技有限公司
						正点原子@ALIENTEK
						     2016-11月
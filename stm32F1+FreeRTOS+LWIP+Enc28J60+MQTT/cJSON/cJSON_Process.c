#include "cJSON_Process.h"
#include "cJSON.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/******服务器发送的数据格式*******/
#define BOOKCASEID "bookcaseId"
#define BOOKCELLID "bookcellId"
#define BOOKCOPYID "bookcopyId"
#define STATUS     "status"
#define OPERATION  "operation"
#define BOOKLSBN   "booklsbn"
/*************数组存储数据*********/
char bookcaseId[100]; //书柜ID
char bookcellId[100];//书格id
char bookcopyId[100];//用于生成条码
char operation[100];//打印条码
char status;//锁状态
/***********
 *@brief: 以json格式解析字符串并获取对应type类型的值
 *@param: msg 需要解析的字符串
 *@param: type 想要获取值的类型
 *@param：value 将获取的值传给value
 *@return: 成功返回value,失败返回NULL
 *************/
char * parse_command(char* msg,const char * type,char* value)
{ 
	 char * string; 
   cJSON *root = cJSON_Parse(msg);
	 
	 if(root != NULL)
	 {	
       string=cJSON_GetObjectItem(root,type)->valuestring;
       printf("string: %s\r\n",string);	
       memset(value,0,strlen(value));//注意数据长度，会造成发送数据包错误		 
		   memcpy(value,string,strlen(string));
		   cJSON_Delete(root); // 释放内存空间 
		   printf("parse: %s\r\n",value);
		   return value;
	 }
	 return NULL;
}

/****************
 *@brief:解析命令
 *@param: msg 收到的命令字符串
 *@return： 1：书柜的打开关闭
 *          2：取书
 *          3：存书 
 *          4：打印条码
 *************************/

int parse_topic(char* msg,char*bookcaseId,char*bookcellId,char*bookcopyId,char*operation,char*status)
{
		//char p[30],q[30];
	/**********书柜的开关**************/
	if(strstr(msg,BOOKCASEID)!=NULL&&strstr(msg,OPERATION)==NULL)
	{
	   parse_command(msg,BOOKCASEID,bookcaseId);//获取书柜号
	  
	   parse_command(msg,STATUS,status);//获取状态值
		    if(*status=='1')
				{
					printf("open all: %s , %s\r\n",bookcaseId,status);
				}
				if(*status=='0')
				{
				  printf("close all: %s , %s\r\n",bookcaseId,status); 
				}
	    
		return 1;	
	 }		
	 /***************书籍的存取*****************/
	 
	   if(strstr(msg,BOOKCELLID)!=NULL)
	   {
	     parse_command(msg,BOOKCELLID,bookcellId);//获取书柜号
	     
	        parse_command(msg,STATUS,status);//获取值
		      if(*status=='1')
				  {
					  printf("store book\r\n");
						return 2;
				  }
				  if(*status=='0')
				  {
				    printf("return book\r\n");
            return 3;						
				  }
	     }
			 
	 	
  /**************打印条码**********************/
  	if(strstr(msg,BOOKCOPYID)!=NULL)
	   {
			 parse_command(msg,BOOKCOPYID,bookcaseId);//获取书柜号
	     parse_command(msg,BOOKCOPYID,bookcopyId);//生成条码
			 parse_command(msg,BOOKCOPYID,operation);//打印条码
	     if(*bookcopyId!='\0')
	     {
	        printf("rfid\r\n");
	     }
			 return 4;
		 }			 
	 
}
/********
 * @brief: 拼接响应报文
 * @param: buf 存储拼接好的数据
 * @param: tye 数据报文的类型
 * @param: bookcaseId 书柜号
 * @param: bookcellId 柜子上的格子号
 * @param: code 错误状态 成功1 or 失败 0
 * @param：bookId  书的ID
 * @param: booklsbn 扫描到书格里书的条形码
 * @param: RFID  存书扫码
 * @return：无
 *************/
void make_cmd_response(char *buf,int type,char*bookcaseId,char*bookcellId,int code,char* bookId,char*booklsbn,char*RFID)
{
   switch (type)
	 {
		 case 1://打开书柜
			 sprintf(buf,"{\"bookcaseId\":\"%s\",\"code\":\"%d\"}",bookcaseId,code);//打开关闭书柜响应数据
			 break;
		 case 2:
			 sprintf(buf,"{\"bookcellId\":\"%s\",\"code\":\"%d\",\"bookId\":\"%s\"}",bookcellId,code,bookId);//取书响应数据
		 
			 break;
		 case 3:
			 sprintf(buf,"{\"bookcellId\":\"%s\",\"code\":\"%d\",\"booklsbn\":\"%s\"}",bookcellId,code,booklsbn);//还书响应数据
		 
			 break;
		 case 4://打开书柜
			 sprintf(buf,"{\"bookcaseId\":\"%s\",\"code\":\"%d\",\"RFID\":\"%s\"}",bookcaseId,code,RFID);//打印条码响应数据
			 break;
		 default:
		  break;
	}  			 
}
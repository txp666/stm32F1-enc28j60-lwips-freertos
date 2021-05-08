#include "cJSON_Process.h"
#include "cJSON.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/******���������͵����ݸ�ʽ*******/
#define BOOKCASEID "bookcaseId"
#define BOOKCELLID "bookcellId"
#define BOOKCOPYID "bookcopyId"
#define STATUS     "status"
#define OPERATION  "operation"
#define BOOKLSBN   "booklsbn"
/*************����洢����*********/
char bookcaseId[100]; //���ID
char bookcellId[100];//���id
char bookcopyId[100];//������������
char operation[100];//��ӡ����
char status;//��״̬
/***********
 *@brief: ��json��ʽ�����ַ�������ȡ��Ӧtype���͵�ֵ
 *@param: msg ��Ҫ�������ַ���
 *@param: type ��Ҫ��ȡֵ������
 *@param��value ����ȡ��ֵ����value
 *@return: �ɹ�����value,ʧ�ܷ���NULL
 *************/
char * parse_command(char* msg,const char * type,char* value)
{ 
	 char * string; 
   cJSON *root = cJSON_Parse(msg);
	 
	 if(root != NULL)
	 {	
       string=cJSON_GetObjectItem(root,type)->valuestring;
       printf("string: %s\r\n",string);	
       memset(value,0,strlen(value));//ע�����ݳ��ȣ�����ɷ������ݰ�����		 
		   memcpy(value,string,strlen(string));
		   cJSON_Delete(root); // �ͷ��ڴ�ռ� 
		   printf("parse: %s\r\n",value);
		   return value;
	 }
	 return NULL;
}

/****************
 *@brief:��������
 *@param: msg �յ��������ַ���
 *@return�� 1�����Ĵ򿪹ر�
 *          2��ȡ��
 *          3������ 
 *          4����ӡ����
 *************************/

int parse_topic(char* msg,char*bookcaseId,char*bookcellId,char*bookcopyId,char*operation,char*status)
{
		//char p[30],q[30];
	/**********���Ŀ���**************/
	if(strstr(msg,BOOKCASEID)!=NULL&&strstr(msg,OPERATION)==NULL)
	{
	   parse_command(msg,BOOKCASEID,bookcaseId);//��ȡ����
	  
	   parse_command(msg,STATUS,status);//��ȡ״ֵ̬
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
	 /***************�鼮�Ĵ�ȡ*****************/
	 
	   if(strstr(msg,BOOKCELLID)!=NULL)
	   {
	     parse_command(msg,BOOKCELLID,bookcellId);//��ȡ����
	     
	        parse_command(msg,STATUS,status);//��ȡֵ
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
			 
	 	
  /**************��ӡ����**********************/
  	if(strstr(msg,BOOKCOPYID)!=NULL)
	   {
			 parse_command(msg,BOOKCOPYID,bookcaseId);//��ȡ����
	     parse_command(msg,BOOKCOPYID,bookcopyId);//��������
			 parse_command(msg,BOOKCOPYID,operation);//��ӡ����
	     if(*bookcopyId!='\0')
	     {
	        printf("rfid\r\n");
	     }
			 return 4;
		 }			 
	 
}
/********
 * @brief: ƴ����Ӧ����
 * @param: buf �洢ƴ�Ӻõ�����
 * @param: tye ���ݱ��ĵ�����
 * @param: bookcaseId ����
 * @param: bookcellId �����ϵĸ��Ӻ�
 * @param: code ����״̬ �ɹ�1 or ʧ�� 0
 * @param��bookId  ���ID
 * @param: booklsbn ɨ�赽��������������
 * @param: RFID  ����ɨ��
 * @return����
 *************/
void make_cmd_response(char *buf,int type,char*bookcaseId,char*bookcellId,int code,char* bookId,char*booklsbn,char*RFID)
{
   switch (type)
	 {
		 case 1://�����
			 sprintf(buf,"{\"bookcaseId\":\"%s\",\"code\":\"%d\"}",bookcaseId,code);//�򿪹ر������Ӧ����
			 break;
		 case 2:
			 sprintf(buf,"{\"bookcellId\":\"%s\",\"code\":\"%d\",\"bookId\":\"%s\"}",bookcellId,code,bookId);//ȡ����Ӧ����
		 
			 break;
		 case 3:
			 sprintf(buf,"{\"bookcellId\":\"%s\",\"code\":\"%d\",\"booklsbn\":\"%s\"}",bookcellId,code,booklsbn);//������Ӧ����
		 
			 break;
		 case 4://�����
			 sprintf(buf,"{\"bookcaseId\":\"%s\",\"code\":\"%d\",\"RFID\":\"%s\"}",bookcaseId,code,RFID);//��ӡ������Ӧ����
			 break;
		 default:
		  break;
	}  			 
}
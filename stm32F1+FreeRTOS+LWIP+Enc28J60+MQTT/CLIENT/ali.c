/*******************************************************************************
 * Copyright (c) 2012, 2016 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *    Ian Craggs - change delimiter option from char to string
 *    Al Stockdill-Mander - Version using the embedded C client
 *    Ian Craggs - update MQTTClient function names
 *******************************************************************************/

#include <stdio.h>

#include "mqttclient.h"

#include <stdio.h>



#define EXAMPLE_PRODUCT_KEY			"a11xsrWmW14"
#define EXAMPLE_DEVICE_NAME			"paho_mqtt"
#define EXAMPLE_DEVICE_SECRET       "Y877Bgo8X5owd3lcB5wWDjryNPoBUdiH"

/* declare the external function aiotMqttSign() */
extern int aiotMqttSign(const char *productKey, const char *deviceName, const char *deviceSecret, 
                     	char clientId[150], char username[65], char password[65]);

volatile int toStop = 0;



void messageArrived(MessageData* md)
{
	MQTTMessage* message = md->message;
	printf("%.*s\t", md->topicName->lenstring.len, md->topicName->lenstring.data);
	printf("%.*s\n", (int)message->payloadlen, (char*)message->payload);
}

/* main function */
int main(int argc, char** argv)
{
	int rc = 0;

	/* setup the buffer, it must big enough for aliyun IoT platform */
	unsigned char buf[1000];
	unsigned char readbuf[1000];

	Network n;
	MQTTClient c;
	char *host = EXAMPLE_PRODUCT_KEY".iot-as-mqtt.cn-shanghai.aliyuncs.com";
	short port = 443;

	const char *subTopic = "/"EXAMPLE_PRODUCT_KEY"/"EXAMPLE_DEVICE_NAME"/user/get";
	const char *pubTopic = "/"EXAMPLE_PRODUCT_KEY"/"EXAMPLE_DEVICE_NAME"/user/update";

	/* invoke aiotMqttSign to generate mqtt connect parameters */
	char clientId[150] = {0};
	char username[65] = {0};
	char password[65] = {0};

	if ((rc = aiotMqttSign(EXAMPLE_PRODUCT_KEY, EXAMPLE_DEVICE_NAME, EXAMPLE_DEVICE_SECRET, clientId, username, password) < 0)) {
		printf("aiotMqttSign -%0x4x\n", -rc);
		return -1;
	}
	printf("clientid: %s\n", clientId);
	printf("username: %s\n", username);
	printf("password: %s\n", password);

	signal(SIGINT, cfinish);
	signal(SIGTERM, cfinish);

	/* network init and establish network to aliyun IoT platform */
	NetworkInit(&n);
	rc = NetworkConnect(&n, host, port);
	printf("NetworkConnect %d\n", rc);

	/* init mqtt client */
	MQTTClientInit(&c, &n, 1000, buf, sizeof(buf), readbuf, sizeof(readbuf));
 
	/* set the default message handler */
	c.defaultMessageHandler = messageArrived;

	/* set mqtt connect parameter */
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
	data.willFlag = 0;
	data.MQTTVersion = 3;
	data.clientID.cstring = clientId;
	data.username.cstring = username;
	data.password.cstring = password;
	data.keepAliveInterval = 60;
	data.cleansession = 1;
	printf("Connecting to %s %d\n", host, port);

	rc = MQTTConnect(&c, &data);
	printf("MQTTConnect %d, Connect aliyun IoT Cloud Success!\n", rc);
    
    printf("Subscribing to %s\n", subTopic);
	rc = MQTTSubscribe(&c, subTopic, 1, messageArrived);
	printf("MQTTSubscribe %d\n", rc);

	int cnt = 0;
    unsigned int msgid = 0;
	while (!toStop)
	{
		MQTTYield(&c, 1000);	

		if (++cnt % 5 == 0) {
			MQTTMessage msg = {
				QOS1, 
				0,
				0,
				0,
				"Hello world",
				strlen("Hello world"),
			};
            msg.id = ++msgid;
			rc = MQTTPublish(&c, pubTopic, &msg);
			printf("MQTTPublish %d, msgid %d\n", rc, msgid);
		}
	}

	printf("Stopping\n");

	MQTTDisconnect(&c);
	NetworkDisconnect(&n);

	return 0;
}

#include "Mqtt_com.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <exception>
#include <memory>

Mqtt_com::Mqtt_com(std::unique_ptr<mqtt::MqttSettings>& mqttSettings):
settings(mqttSettings.get())
{

}


Mqtt_com::~Mqtt_com()
{
#ifndef DEBUG
	loop_stop();            // Kill the thread
	mosqpp::lib_cleanup();    // Mosquitto library cleanup
#endif
}

bool Mqtt_com::send_message(const char* topic, const  char * message)
{
	// Send message - depending on QoS, mosquitto lib managed re-submission this the thread
	//
	// * NULL : Message Id (int *) this allow to latter get status of each message
	// * topic : topic to be used
	// * lenght of the message
	// * message
	// * qos (0,1,2)
	// * retain (boolean) - indicates if message is retained on broker or not
	// Should return MOSQ_ERR_SUCCESS
#ifndef DEBUG
	int ret = mosquittopp::publish(NULL,topic,strlen(message), message,1,false);
	return ( ret == MOSQ_ERR_SUCCESS );
#else
	return true;
#endif

}
void Mqtt_com::connect(){


#ifndef DEBUG
	mosqpp::mosquittopp(settings->id.c_str());
	mosqpp::lib_init();        // Mandatory initialization for mosquitto library
	connect_async(settings->host.c_str(),     // non blocking blocking connection to broker request
			settings->port,
			settings->keepalive);
	loop_start();            // Start thread managing connection / publish / subscribe
#endif
	settings->status = mqtt::Status::Connnecting;
}

void Mqtt_com::disconnect(){
#ifndef DEBUG
	mosquittopp::disconnect();
#endif
}


void Mqtt_com::subscribe(const char* subject)
{
#ifndef DEBUG

	// int subscribe(int *mid, const char *sub, int qos=0);
	int messageId = 0; // if we want to track if the message has been sent (ack)
	int rc = mosquittopp::subscribe(&messageId,subject);

	if(rc != MOSQ_ERR_SUCCESS){throw std::invalid_argument("Subscription failed");}
#endif
}

void Mqtt_com::on_disconnect(int rc)
{
	std::cout << ">> Mqtt_com - disconnection(" << rc << ")" << std::endl;
	settings->status = mqtt::Status::Disconnected;

	// Reconnect?! if *status...
	// reconnect_async();

}

void Mqtt_com::on_connect(int rc)
{
	if ( rc == 0 )
	{
		std::cout << ">> Mqtt_com - connected with server" << std::endl;
		settings->status = mqtt::Status::Connected;
	} else {
		std::cout << ">> Mqtt_com - Impossible to connect with server(" << rc << ")" << std::endl;
		settings->status = mqtt::Status::Disconnected;
	}
}

void Mqtt_com::on_message(const struct mosquitto_message *message)
{

	//	struct mosquitto_message{
	//		uint16_t mid;
	//		char *topic;
	//		uint8_t *payload;
	//		uint32_t payloadlen;
	//		int qos;
	//		bool retain;
	//	};
	std::cout <<
			"Topic: " << message->topic << ", payload: " <<
			reinterpret_cast<char*>(message->payload) << std::endl;
}

void Mqtt_com::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
	// Fetch if the subscription was ok!
	std::cout << "mid: " << std::to_string(mid);
}

void Mqtt_com::on_publish(int mid)
{
	// Fetch if the message was ok!
	std::cout << ">> Mqtt_com - Message (" << mid << ") succeed to be published " << std::endl;
}

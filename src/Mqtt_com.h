/*
 * Mqtt.h
 *
 *  Created on: Oct 30, 2017
 *      Author: johan
 */

#pragma once
#include <mosquittopp.h>
#include "MqttPkg.h"
#include "CommonPkg.h"

class Mqtt_com: public mosqpp::mosquittopp
{
public:
	Mqtt_com(std::unique_ptr<mqtt::MqttSettings>& mqttSettings);
	~Mqtt_com();
	void subscribe(const char* subject);
	bool sendMessage(const char* topic, const  char * message);
	void connect();
	void disconnect();
	void unsubscribe(const char *sub);
private:
#ifdef DEBUG
public:
#endif

	void on_connect(int rc);
	void on_disconnect(int rc);
	void on_publish(int mid);
	void on_subscribe(int mid, int qos_count, const int *granted_qos);
	void on_message(const struct mosquitto_message *message);


	// const char    *     topic;
	mqtt::MqttSettings* settings;

};

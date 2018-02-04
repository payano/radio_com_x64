/*
 * Mqtt.h
 *
 *  Created on: Oct 30, 2017
 *      Author: johan
 */

#pragma once
#include <mosquittopp.h>
#include "MqttPkg.h"
class Mqtt_com: public mosqpp::mosquittopp
{
public:
	Mqtt_com(const char *id, const char *host, int port, mqtt::Status* status);
	~Mqtt_com();
	void subscribe(const char* subject);
	bool send_message(const char* message);
	void disconnect();
private:
#ifdef DEBUG
public:
#endif

	void on_connect(int rc);
	void on_disconnect(int rc);
	void on_publish(int mid);
	void on_subscribe(int mid, int qos_count, const int *granted_qos);
	void on_message(const struct mosquitto_message *message);

	const char* id;
	const char* host;
	int port;
	int keepalive;
	mqtt::Status* status;
	// const char    *     topic;

};

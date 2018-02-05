/*
 * Mqttt.h
 *
 *  Created on: Feb 2, 2018
 *      Author: johan
 */

#pragma once
#include <map>
#include <memory>
#include "MessagePkg.h"
#include "MqttPkg.h"
#include "Mqtt_com.h"
#include <thread>


namespace mqtt  {
static constexpr int threadDelay = 10; // ms

class Mqtt {
public:
	Mqtt(std::unique_ptr<mqtt::MqttSettings>& mqttSettings);
	Mqtt(std::unique_ptr<mqtt::MqttSettings>&& mqttSettings);
	virtual ~Mqtt();
	const std::unique_ptr<MqttSettings>& getSettings();
	const Status& getMqttStatus();
	const Status& getRunningStatus();
	void start();
	void stop();

private:
#ifdef DEBUG
public:
	void TestChangeStatus(Status newStatus);
#endif
	void run();
	void handleSubscriptions(bool subscribe=true);
	std::unique_ptr<MqttSettings> mqttSettings;
	std::unique_ptr<std::thread> runningThread;
	Status runningStatus;
	Mqtt_com mqtt_com;

};
}

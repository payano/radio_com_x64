/*
 * Mqttt.cpp
 *
 *  Created on: Feb 2, 2018
 *      Author: johan
 */

#include "Mqtt.h"
#include <iostream>
#include <exception>

namespace mqtt {

Mqtt::Mqtt(std::unique_ptr<mqtt::MqttSettings>& mqttSettings):
mqttSettings(std::move(mqttSettings)),
runningThread(nullptr),
runningStatus(Status::Stopped),
mqtt_com(this->mqttSettings->id.c_str(), this->mqttSettings->host.c_str(), this->mqttSettings->port, &this->mqttSettings->status)
{}

Mqtt::Mqtt(std::unique_ptr<mqtt::MqttSettings>&& mqttSettings):
mqttSettings(std::move(mqttSettings)),
runningThread(nullptr),
runningStatus(Status::Stopped),
mqtt_com(this->mqttSettings->id.c_str(), this->mqttSettings->host.c_str(), this->mqttSettings->port, &this->mqttSettings->status)
{}

Mqtt::~Mqtt()
{
	// Stop thread
	stop();
}

const Status& Mqtt::getMqttStatus(){return mqttSettings->status;}
const Status& Mqtt::getRunningStatus(){return runningStatus;}
const std::unique_ptr<MqttSettings>& Mqtt::getSettings(){return mqttSettings;}

void Mqtt::start()
{
	if(runningThread != nullptr){throw std::invalid_argument("Thread already started.");}
	// Create a thread and start running the instance.
	runningThread = std::make_unique<std::thread>(&Mqtt::run, this);
	// Detach it so it runs in background
	runningThread->detach();
}

void Mqtt::run()
{
	if(runningStatus == Status::Runnning){return;}

	runningStatus = Status::Runnning;
	while(runningStatus == Status::Runnning)
	{
		std::cout << "YAY!\n";
		std::this_thread::sleep_for(std::chrono::seconds(1));

	}
	// Remove all connections to Mqtt broker server...
	mqtt_com.disconnect();
	// Reset all statuses in settings.

	runningStatus = Status::Stopped;
	runningThread.reset();
}

void Mqtt::stop()
{
	runningStatus = Status::Stopping;
	//join, set runningThread to nullptr / reset it.
}

#ifdef DEBUG
void Mqtt::TestChangeStatus(Status newStatus){mqttSettings->status = newStatus;}
#endif

}

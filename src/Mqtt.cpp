/*
 * Mqttt.cpp
 *
 *  Created on: Feb 2, 2018
 *      Author: johan
 */

#include "Mqtt.h"
#include <iostream>
#include <exception>
#include <sys/time.h>
#include <assert.h>

namespace mqtt {

Mqtt::Mqtt(std::unique_ptr<mqtt::MqttSettings>& mqttSettings):
	mqttSettings(std::move(mqttSettings)),
	runningThread(nullptr),
	runningStatus(common::Status::Stopped),
	mqtt_com(this->mqttSettings)
{}

Mqtt::Mqtt(std::unique_ptr<mqtt::MqttSettings>&& mqttSettings):
	mqttSettings(std::move(mqttSettings)),
	runningThread(nullptr),
	runningStatus(common::Status::Stopped),
	mqtt_com(this->mqttSettings)
{}

Mqtt::~Mqtt()
{
	// Stop thread
	stop();
}

const common::Status& Mqtt::getMqttStatus(){return mqttSettings->status;}
const common::Status& Mqtt::getRunningStatus(){return runningStatus;}
const std::unique_ptr<MqttSettings>& Mqtt::getSettings(){return mqttSettings;}

void Mqtt::start()
{
	assert(mqttSettings != nullptr);
	assert(mqttSettings->recv != nullptr);
	assert(mqttSettings->send != nullptr);

	// Want to run Mqtt inside its own thread.
	if(mqttThread.get() == nullptr){
		// start a new thread and return to the caller.
		mqttThread = std::make_unique<std::thread>(&Mqtt::start, this);
		mqttThread->detach();
		return;
	}

	if(runningThread != nullptr){throw std::invalid_argument("Thread already started.");}
	// Create a thread and start running the instance.
	runningThread = std::make_unique<std::thread>(&Mqtt::run, this);
	// Detach it so it runs in background
	runningThread->join();

	// Make it runnable again
	runningThread.reset();
	mqttThread.reset();
}

void Mqtt::run()
{
	using namespace common;
	std::cout << "Started thread Mqtt.\n";

	if(runningStatus == Status::Runnning){return;}

	runningStatus = Status::Runnning;

	handleSubscriptions();

	unsigned int lastRecvQueueLen = 0u;
	__time_t nextTimeout = 0u;

	while(runningStatus == Status::Runnning)
	{
		if(mqttSettings->status == Status::Disconnected)
		{
			// Disconnect, need to reconnect.
			timeval tv;
			gettimeofday(&tv,NULL);
			if(tv.tv_sec > nextTimeout)
			{
				std::cout << "MQTT: disconnected from server, reconnecting.\n";
				nextTimeout = tv.tv_sec + RECONNECT_TIMEOUT;
				mqtt_com.connect();
			}
		}

		if(!mqttSettings->recv->isEmpty())
		{
			// These are going to MQTT Broker (received from radio)
			MessagePkg::Message send;
			mqttSettings->recv->pop(send);
			std::string message = send.base + send.topic;
			mqtt_com.sendMessage(message.c_str(), send.value.c_str());

			std::cout << "WOOT INCOMING MESSAGE, SENDING TO HOME ASSISTANT!!\n";
			std::cout << "topic: " << message << ", value: " << send.value << std::endl;
		}
		if(mqttSettings->send->size() != lastRecvQueueLen){
			std::cout << "Message size: " << mqttSettings->send->size() << ", is on the way to the radio for transmitting\n";
		}
		lastRecvQueueLen = mqttSettings->send->size();

		std::this_thread::sleep_for(std::chrono::milliseconds(THREADDELAY));
	}
	std::cout << "Closing thread Mqtt.\n";

	// Remove all connections to Mqtt broker server...
	handleSubscriptions(false);
	mqtt_com.disconnect();
	// Reset all statuses in settings.

	runningStatus = Status::Stopped;
//	runningThread.reset();
	std::cout << "Quit thread Mqtt!\n";

}

void Mqtt::stop()
{
	using namespace common;
	runningStatus = Status::Stopping;
	// Give thread time to quit
//	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void Mqtt::handleSubscriptions(bool subscribe){
	using namespace common;
	mqtt_com.connect();
	mqttSettings->status = Status::Connnecting;

	for(auto& sub: mqttSettings->accessories)
	{
		for(auto& topic: sub.topics)
		{
			// subscribe to all getters
			std::string subscription = sub.base + topic.first + topic.second.get;
			if(subscribe)
			{
				mqtt_com.subscribe(subscription.c_str());
			}else{
				mqtt_com.unsubscribe(subscription.c_str());
			}
		}
	}
}

}

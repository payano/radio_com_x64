//============================================================================
// Name        : radio_com_x64.cpp
// Author      : Johan Svensson
// Version     :
// Copyright   : Free For All
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <chrono>
#include "MessagePkg.h"
#include "RadioPkg.h"
#include "Mqtt.h"
#include "Radio.h"
#include <future>
#include <csignal>
#include <string.h>
#include <memory>
#include "MqttPkg.h"
#include "CommonPkg.h"


#ifdef DEBUG
//#include "test/mqtt_test.h"
#include "test/radio_test.h"
#include "gtest/gtest.h"
#endif
//
using namespace MessagePkg;
#undef DEBUG
//void signal_handler(int signal)
//{
//	std::cout << "SIGNAL: " << strsignal(signal) << std::endl;
//	exit(1);
////  gSignalStatus = signal;
//}
int main(int argc, char **argv) {
#ifdef DEBUG
	std::cout << "DEBUGGING!!\n";
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#endif
	// Install a signal handler
	std::unique_ptr<mqtt::MqttSettings> mqttSettings = std::make_unique<mqtt::MqttSettings>();
	std::unique_ptr<radio::RadioSettings> radioSettings = std::make_unique<radio::RadioSettings>();
	auto mqttRecv = std::make_shared<MessagePkg::Queue<MessagePkg::Message>>();
	auto radioRecv = std::make_shared<MessagePkg::Queue<MessagePkg::Message>>();
	mqttSettings->recieve = mqttRecv;
	mqttSettings->send= radioRecv;

	radioSettings->recieve = radioRecv;
	radioSettings->send = mqttRecv;

	using namespace mqtt;
	using namespace common;
	// Create RGB Accessory
	Accessory rgb;
	rgb.type = Type::RGB;
	rgb.name = "RGB Light";
	rgb.base = "kitchen/rgb/";
	rgb.topics["light"].get = "set"; // Home assistant will set and we will get it.
	rgb.topics["light"].set = "get"; // Home assistant will get and we will send it.
	rgb.topics["brightness"].get = "set"; // Home assistant will set and we will get it.
	rgb.topics["brightness"].set = "get"; // Home assistant will get and we will send it.
	rgb.topics["rgb"].get = "set"; // Home assistant will set and we will get it.
	rgb.topics["rgb"].set = "get"; // Home assistant will get and we will send it.
	mqttSettings->accessories.emplace_back(rgb);

	// Create ordinary lightning without RGB.
	Accessory lamp;
	lamp.type = Type::LED;
	lamp.name = "Led light";
	lamp.base = "kitchen/white/";
	lamp.topics["light"].get = "set"; // Home assistant will set and we will get it.
	lamp.topics["light"].set = "get"; // Home assistant will get and we will send it.
	lamp.topics["brightness"].get = "set"; // Home assistant will set and we will get it.
	lamp.topics["brightness"].set = "get"; // Home assistant will get and we will send it.
	mqttSettings->accessories.emplace_back(lamp);

	mqttSettings->host = "192.168.0.100";
	mqttSettings->id = "0";
	mqttSettings->port = 1883;
	mqttSettings->keepalive = 60;


//	std::signal(SIGINT, signal_handler);
//	std::signal(SIGTERM, signal_handler);

	mqtt::Mqtt mqtt(mqttSettings);
//	radio::Radio radio(radioSettings);

	mqtt.start();
//	radio.start();

	std::cout << "Starting application.\n";
	std::promise<void> p;
	p.get_future().wait();


	return 0;
}

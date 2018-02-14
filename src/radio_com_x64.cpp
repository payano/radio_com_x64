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
#ifdef DEBUG
//#include "test/mqtt_test.h"
#include "test/radio_test.h"
#include "gtest/gtest.h"
#endif
//
using namespace MessagePkg;

std::unique_ptr<mqtt::MqttSettings> mqttSettings = std::make_unique<mqtt::MqttSettings>();
std::unique_ptr<radio::RadioSettings> radioSettings = std::make_unique<radio::RadioSettings>();

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
//	std::signal(SIGINT, signal_handler);
//	std::signal(SIGTERM, signal_handler);

	mqtt::Mqtt mqtt(mqttSettings);
	radio::Radio radio(radioSettings);

	mqtt.start();
	radio.start();

	std::cout << "Starting application.\n";
	std::promise<void> p;
	p.get_future().wait();


	return 0;
}

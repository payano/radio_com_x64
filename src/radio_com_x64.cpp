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
#include "Mqtt.h"

#ifdef DEBUG
#include "test/mqtt_test.h"
#include "gtest/gtest.h"
#endif
//
using namespace MessagePkg;

mqtt::MqttSettings mqttSettings;
void mqtt_thread(){
//	mqtt::Mqtt(mqttSettings);

}
void zigbee_thread(){
//	Zigbee(to_mqtt, to_zigbee);
}

int main(int argc, char **argv) {
#ifdef DEBUG
	std::cout << "DEBUGGING!!\n";
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#endif
	std::cout << "Starting application.\n";
//    std::cout << "Starting Mqtt thread.\n";
//    std::thread mqtt(mqtt_thread);
//    std::cout << "Starting Zigbee thread.\n";
//    std::thread zigbee(zigbee_thread);
//
//    mqtt.join();
//    zigbee.join();
//    std::cout << "Exiting application.\n";

	return 0;
}

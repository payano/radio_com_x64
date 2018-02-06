/*
 * mqtt_test.h
 *
 *  Created on: Feb 3, 2018
 *      Author: johan
 */

#pragma once

#include "gtest/gtest.h"
#include <memory>
#include "../Mqtt.h"
#include "../MqttPkg.h"
#include "../CommonPkg.h"

namespace MqttTest {

std::unique_ptr<mqtt::MqttSettings> setup(){
	using namespace MessagePkg;
	using namespace mqtt;
	using namespace common;

	std::unique_ptr<MqttSettings> settings = std::make_unique<MqttSettings>();
	settings->recv = std::make_shared<MessagePkg::Queue<MessagePkg::Message>>();
	settings->send= std::make_shared<MessagePkg::Queue<MessagePkg::Message>>();
	settings->id = "0";
	settings->host = "localhost";
	settings->port = 1883;
	settings->keepalive = 60;
	settings->status = Status::Disconnected;


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
	settings->accessories.emplace_back(rgb);

	// Create ordinary lightning without RGB.
	Accessory lamp;
	lamp.type = Type::LED;
	lamp.name = "Led light";
	lamp.base = "kitchen/white/";
	lamp.topics["light"].get = "set"; // Home assistant will set and we will get it.
	lamp.topics["light"].set = "get"; // Home assistant will get and we will send it.
	lamp.topics["brightness"].get = "set"; // Home assistant will set and we will get it.
	lamp.topics["brightness"].set = "get"; // Home assistant will get and we will send it.

	return settings;
}
TEST(Mqtt, testIntialSettings) {
	using namespace common;
	mqtt::Mqtt mqtt(setup());

	// Get a const of settings
	auto& settingsCopy = mqtt.getSettings();

	EXPECT_EQ(settingsCopy->id, "0");
	EXPECT_EQ(settingsCopy->host, "localhost");
	EXPECT_EQ(settingsCopy->port, 1883);
	EXPECT_EQ(settingsCopy->keepalive, 60);
	EXPECT_EQ(settingsCopy->recv->size(),0u);
	EXPECT_EQ(settingsCopy->send->size(),0u);

	EXPECT_EQ(mqtt.getMqttStatus(), Status::Disconnected);

	// Set new status
	mqtt.mqtt_com.on_connect(0);
	EXPECT_EQ(mqtt.getMqttStatus(), Status::Connected);

	mqtt.mqtt_com.on_disconnect(0);
	EXPECT_EQ(mqtt.getMqttStatus(), Status::Disconnected);

	EXPECT_NE(settingsCopy->recv.get(), nullptr);
	EXPECT_NE(settingsCopy->send.get(), nullptr);
}
TEST(Mqtt, testThreadStartStop) {
	using namespace common;

	mqtt::Mqtt mqtt(setup());

	// Start mqtt instance
	mqtt.start();
	sleep(1); // Give the Mqtt a change to start
	EXPECT_EQ(mqtt.getRunningStatus(), Status::Runnning);

	// Try to stop it
	mqtt.stop();
	EXPECT_EQ(mqtt.getRunningStatus(), Status::Stopping);
	sleep(1);
	EXPECT_EQ(mqtt.getRunningStatus(), Status::Stopped);

	mqtt.start();
	sleep(1); // Give the Mqtt a change to start
	EXPECT_EQ(mqtt.getRunningStatus(), Status::Runnning);

	// Try to stop it
	mqtt.stop();
	EXPECT_EQ(mqtt.getRunningStatus(), Status::Stopping);
	sleep(1);
	EXPECT_EQ(mqtt.getRunningStatus(), Status::Stopped);
};

TEST(Mqtt, testMqttRecvMessage) {
	using namespace common;

	std::unique_ptr<mqtt::MqttSettings> settings = setup();
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> recvQueue = settings->recv;
	mqtt::Mqtt mqtt(settings);
	mqtt.start();
	sleep(1); // Give the Mqtt a change to start

	// Fake a connection
	mqtt.mqtt_com.on_connect(0);

	recvQueue->push(MessagePkg::Message{"kitchen/rgb/", "get", "255,255,255"});
	EXPECT_EQ(recvQueue->size(),1u);
	sleep(1);
	EXPECT_EQ(recvQueue->size(),0u);
	mqtt.stop();
	while(mqtt.runningStatus != Status::Stopped){std::this_thread::sleep_for(std::chrono::milliseconds(10));}
}

TEST(Mqtt, testMqttSendMessage) {
	using namespace common;

	std::unique_ptr<mqtt::MqttSettings> settings = setup();
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> sendQueue = settings->send;
	mqtt::Mqtt mqtt(settings);
	mqtt.start();
	sleep(1); // Give the Mqtt a change to start

	sendQueue->push(MessagePkg::Message{"kitchen/rgb/", "set", "255,255,255"});
	EXPECT_EQ(sendQueue->size(),1u);
	sleep(1);
	mqtt.stop();
	while(mqtt.runningStatus != Status::Stopped){std::this_thread::sleep_for(std::chrono::milliseconds(10));}
}
TEST(Mqtt, testMqttDisconnect) {
	using namespace common;

	std::unique_ptr<mqtt::MqttSettings> settings = setup();
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> recvQueue = settings->recv;
	mqtt::Mqtt mqtt(settings);
	recvQueue->push(MessagePkg::Message{"kitchen/rgb/", "get", "255,255,255"});

	mqtt.start();
	sleep(1); // Give the Mqtt a change to start
	EXPECT_EQ(mqtt.getMqttStatus(), Status::Connnecting);

	// Disconnect event.
	mqtt.mqtt_com.on_disconnect(0);
	EXPECT_EQ(mqtt.getMqttStatus(), Status::Disconnected);
	sleep(1);
	// not a successful connect
	mqtt.mqtt_com.on_connect(1);
	sleep(1);
	EXPECT_EQ(mqtt.getMqttStatus(), Status::Disconnected);
	sleep(5);

	mqtt.mqtt_com.on_connect(1);
	sleep(1);
	EXPECT_EQ(mqtt.getMqttStatus(), Status::Disconnected);
	sleep(5);
	EXPECT_EQ(mqtt.getMqttStatus(), Status::Connnecting);

	// Connect
	mqtt.mqtt_com.on_connect(1);
	mqtt.mqtt_com.on_connect(0);
	sleep(1);
	EXPECT_EQ(mqtt.getMqttStatus(), Status::Connected);

	mqtt.stop();

	while(mqtt.runningStatus !=  Status::Stopped){std::this_thread::sleep_for(std::chrono::milliseconds(10));}
}


}

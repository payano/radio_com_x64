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
namespace MqttTest {

std::unique_ptr<mqtt::MqttSettings> setup(){
	using namespace MessagePkg;
	using namespace mqtt;

	std::unique_ptr<MqttSettings> settings = std::make_unique<MqttSettings>();
	settings->recv = std::make_shared<MessagePkg::Queue<MessagePkg::Message>>();
	settings->send= std::make_shared<MessagePkg::Queue<MessagePkg::Message>>();
	settings->id = "0";
	settings->host = "localhost";
	settings->port = 1883;
	settings->keepalive = 60;
	return settings;
}
TEST(Mqtt, test) {
	mqtt::Mqtt mqtt(setup());

	// Get a const of settings
	auto& settingsCopy = mqtt.getSettings();

	EXPECT_EQ(settingsCopy->id, "0");
	EXPECT_EQ(settingsCopy->host, "localhost");
	EXPECT_EQ(settingsCopy->port, 1883);
	EXPECT_EQ(settingsCopy->keepalive, 60);
	EXPECT_EQ(settingsCopy->recv->size(),0u);
	EXPECT_EQ(settingsCopy->send->size(),0u);

	EXPECT_EQ(mqtt.getMqttStatus(), mqtt::Status::Disconnected);

	// Set new status
	mqtt.mqtt_com.on_connect(0);
	EXPECT_EQ(mqtt.getMqttStatus(), mqtt::Status::Connected);

	mqtt.mqtt_com.on_disconnect(0);
	EXPECT_EQ(mqtt.getMqttStatus(), mqtt::Status::Disconnected);

	// Start mqtt instance
	mqtt.start();
	sleep(1); // Give the Mqtt a change to start
	EXPECT_EQ(mqtt.getRunningStatus(), mqtt::Status::Runnning);

	// Try to stop it
	mqtt.stop();
	EXPECT_EQ(mqtt.getRunningStatus(), mqtt::Status::Stopping);
	sleep(1);
	EXPECT_EQ(mqtt.getRunningStatus(), mqtt::Status::Stopped);

	mqtt.start();
	sleep(1); // Give the Mqtt a change to start
	EXPECT_EQ(mqtt.getRunningStatus(), mqtt::Status::Runnning);


};
}

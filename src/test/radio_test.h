/*
 * radio_test.h
 *
 *  Created on: Feb 6, 2018
 *      Author: johan
 */

#pragma once

#include "gtest/gtest.h"
#include <memory>
#include "../Radio.h"
#include "../RadioPkg.h"
#include "../CommonPkg.h"
#include <string.h>
#include <vector>
#include <utility>      // std::move

namespace RadioTest {
std::unique_ptr<radio::RadioSettings> setup(){
	using namespace MessagePkg;
	using namespace radio;
	using namespace common;

	std::unique_ptr<RadioSettings> settings = std::make_unique<RadioSettings>();
	settings->recieve = std::make_shared<MessagePkg::Queue<MessagePkg::Message>>();
	settings->send = std::make_shared<MessagePkg::Queue<MessagePkg::Message>>();
	settings->keepalive = 60;
	settings->host_addr.addr.pan_id = 0x0023;
	settings->host_addr.family = AF_IEEE802154;
	settings->host_addr.addr.addr_type = IEEE802154_ADDR_SHORT;
	settings->host_addr.addr.short_addr = 312;
	settings->host_addr.addr.short_addr = 0x0002;
	settings->status = Status::Disconnected;

	// Create the one node that has both RGB and LED-warm.

	node oneNode;
	oneNode.connected = Status::Disconnected;
	oneNode.src.family = AF_IEEE802154;
	oneNode.src.addr.addr_type = IEEE802154_ADDR_SHORT;
	oneNode.src.addr.short_addr = 0x6001;
	oneNode.src.addr.pan_id = settings->host_addr.addr.pan_id;

	publishedResource resource1;
	resource1.type = Type::RGB;
	resource1.base = "kitchen/rgb/";
	resource1.topics["light"].get = "set"; // Home assistant will set and we will get it.
	resource1.topics["light"].set = "get"; // Home assistant will get and we will send it.
	resource1.topics["brightness"].get = "set"; // Home assistant will set and we will get it.
	resource1.topics["brightness"].set = "get"; // Home assistant will get and we will send it.
	resource1.topics["rgb"].get = "set"; // Home assistant will set and we will get it.
	resource1.topics["rgb"].set = "get"; // Home assistant will get and we will send it.
	oneNode.resources.emplace_back(resource1);

	publishedResource resource2;
	resource2.type = Type::LED;
	resource2.base = "kitchen/white/";
	resource2.topics["light"].get = "set"; // Home assistant will set and we will get it.
	resource2.topics["light"].set = "get"; // Home assistant will get and we will send it.
	resource2.topics["brightness"].get = "set"; // Home assistant will set and we will get it.
	resource2.topics["brightness"].set = "get"; // Home assistant will get and we will send it.
	oneNode.resources.emplace_back(resource2);

	settings->nodes.emplace_back(oneNode);

	return settings;
}

TEST(Radio, testInitialSettings) {
	using namespace common;
	radio::Radio radio(setup());
	auto& settingsCopy = radio.getSettings();

	EXPECT_EQ(settingsCopy->host_addr.family, AF_IEEE802154);
	EXPECT_EQ(settingsCopy->host_addr.addr.addr_type, IEEE802154_ADDR_SHORT);
	EXPECT_EQ(settingsCopy->host_addr.addr.pan_id, 0x0023);
	EXPECT_EQ(settingsCopy->host_addr.addr.short_addr, 0x0002);

	EXPECT_EQ(settingsCopy->nodes.size(),1u);
	EXPECT_EQ(settingsCopy->nodes[0].connected, Status::Disconnected);
	EXPECT_EQ(settingsCopy->nodes[0].src.family, AF_IEEE802154);
	EXPECT_EQ(settingsCopy->nodes[0].src.addr.short_addr, 0x6001);
	EXPECT_EQ(settingsCopy->nodes[0].src.addr.pan_id, 0x0023);

	EXPECT_EQ(settingsCopy->nodes[0].resources.size(), 2u);

}

TEST(Mqtt, testThreadStartStop) {
	using namespace common;
	radio::Radio radio(setup());

	//Start radio.
	radio.start();
	sleep(1); // Give the Mqtt a change to start
	EXPECT_EQ(radio.getRunningStatus(), Status::Runnning);

	// Try to stop it
	radio.stop();
	EXPECT_EQ(radio.getRunningStatus(), Status::Stopping);
	sleep(1);
	EXPECT_EQ(radio.getRunningStatus(), Status::Stopped);

	radio.start();
	sleep(1); // Give the Mqtt a change to start
	EXPECT_EQ(radio.getRunningStatus(), Status::Runnning);

	// Try to stop it
	radio.stop();
	EXPECT_EQ(radio.getRunningStatus(), Status::Stopping);
	sleep(1);
	EXPECT_EQ(radio.getRunningStatus(), Status::Stopped);

}

TEST(Radio, testRadioRecvMessage) {
	using namespace common;

	std::unique_ptr<radio::RadioSettings> settings = setup();
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> recvQueue = settings->recieve;
	radio::Radio radio(settings);
	radio.start();
	sleep(1); // Give the Mqtt a change to start

	// Fake a connection
//	mqtt.mqtt_com.on_connect(0);

	recvQueue->push(MessagePkg::Message{common::Method::get, "kitchen/rgb/", "get", "255,255,255"});
	EXPECT_EQ(recvQueue->size(),1u);
	sleep(1);
	EXPECT_EQ(recvQueue->size(),0u);
	radio.stop();
	while(radio.runningStatus != Status::Stopped){std::this_thread::sleep_for(std::chrono::milliseconds(10));}
}

}

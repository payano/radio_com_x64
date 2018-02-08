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

namespace RadioTest {
std::unique_ptr<radio::RadioSettings> setup(){
	using namespace MessagePkg;
	using namespace radio;
	using namespace common;

	std::unique_ptr<RadioSettings> settings = std::make_unique<RadioSettings>();
	settings->recv = std::make_shared<MessagePkg::Queue<MessagePkg::Message>>();
	settings->send= std::make_shared<MessagePkg::Queue<MessagePkg::Message>>();
	settings->keepalive = "60";
	settings->family = AF_IEEE802154;
	settings->panid = 0xdead;
	settings->host_addr = {0xd6, 0x55, 0x2c, 0xd6, 0xe4, 0x1c, 0xeb, 0x57};
	settings->status = Status::Disconnected;


	// Create RGB Accessory
//	Accessory rgb;
//	rgb.type = Type::RGB;
//	rgb.name = "RGB Light";
//	rgb.base = "kitchen/rgb/";
//	rgb.topics["light"].get = "set"; // Home assistant will set and we will get it.
//	rgb.topics["light"].set = "get"; // Home assistant will get and we will send it.
//	rgb.topics["brightness"].get = "set"; // Home assistant will set and we will get it.
//	rgb.topics["brightness"].set = "get"; // Home assistant will get and we will send it.
//	rgb.topics["rgb"].get = "set"; // Home assistant will set and we will get it.
//	rgb.topics["rgb"].set = "get"; // Home assistant will get and we will send it.
//	settings->accessories.emplace_back(rgb);
//
//	// Create ordinary lightning without RGB.
//	Accessory lamp;
//	lamp.type = Type::LED;
//	lamp.name = "Led light";
//	lamp.base = "kitchen/white/";
//	lamp.topics["light"].get = "set"; // Home assistant will set and we will get it.
//	lamp.topics["light"].set = "get"; // Home assistant will get and we will send it.
//	lamp.topics["brightness"].get = "set"; // Home assistant will set and we will get it.
//	lamp.topics["brightness"].set = "get"; // Home assistant will get and we will send it.

	return settings;
}

}

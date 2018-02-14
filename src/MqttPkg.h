/*
 * MqttPackage.h
 *
 *  Created on: Feb 4, 2018
 *      Author: johan
 */
#include <string>
#include <vector>
#include "MessagePkg.h"
#include <map>
#include "CommonPkg.h"

#pragma once

namespace mqtt {



struct Accessory{
	common::Type type;
	std::string name;
	std::string base;
	std::map<std::string, common::Topic> topics; // basestring + topic
};

struct MqttSettings{
	std::string id;
	std::string host;
	int port;
	int keepalive;
	common::Status status;
	std::vector<Accessory> accessories;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> recv;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> send;
};

}



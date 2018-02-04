/*
 * MqttPackage.h
 *
 *  Created on: Feb 4, 2018
 *      Author: johan
 */
#include <string>
#include <vector>
#include "MessagePkg.h"

#pragma once

namespace mqtt {

enum Type {
	LED = 0,
	RGB = 1,
	BUTTON = 2
};
enum Status {
	Disconnected = 0,
	Connnecting = 1,
	Connected = 2,
	Stopped = 3,
	Runnning = 4,
	Stopping = 5
};
struct Topic{
	Status status;
	std::string get;
	std::string set;
};
struct Accessory{
	Type type;
	std::string name;
	std::vector<Topic> topics;
};

struct MqttSettings{
	std::string id;
	std::string host;
	int port;
	int keepalive;
	Status status;
	std::vector<Accessory> accessories;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> recv;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> send;
};

}



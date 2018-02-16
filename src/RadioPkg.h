/*
 * RadioPkg.h
 *
 *  Created on: Feb 6, 2018
 *      Author: johan
 */

#pragma once
#include <string>
#include <vector>
#include "MessagePkg.h"
#include "CommonPkg.h"
#include <map>
#include "af_ieee802154.h"

namespace radio {

struct publishedResource {
	common::Type type;
	std::string base;
	std::map<std::string, common::Topic> topics;

};

struct node {
	sockaddr_ieee802154 src;
	common::Status connected;
	std::vector<publishedResource> resources;

	// Published services
	// https://courses.soe.ucsc.edu/courses/cmpe156/Spring14/01/attachments/25556
};

class RadioSettings{
public:
	int keepalive;
	sockaddr_ieee802154 host_addr;
	common::Status status;
	std::vector<node> nodes;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> recieve;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> send;
};

}

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

struct node {
	sockaddr_ieee802154 src;
	common::Status connected;
	std::string base;
	std::string topic;

	// Published services
	// https://courses.soe.ucsc.edu/courses/cmpe156/Spring14/01/attachments/25556
};

struct RadioSettings{
	int keepalive;
	sockaddr_ieee802154 host_addr;
	common::Status status;
	std::vector<node> nodes;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> recv;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> send;
};

}

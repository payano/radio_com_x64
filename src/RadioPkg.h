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
	sockaddr_ieee802154 yo;
	common::Status connected;
};

struct Radio{
	int keepalive;
	sa_family_t family;
	uint16_t panid;
	uint8_t host_addr[IEEE802154_ADDR_LEN];
	common::Status status;
	std::vector<node> nodes;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> recv;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> send;
};

}

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
#include <map>
#include "af_ieee802154.h"

namespace radio {

struct node {
	sockaddr_ieee802154 yo;
	// Status
};

struct Radio{
	int keepalive;
	sa_family_t family;
	uint16_t panid;
	uint8_t host_addr[IEEE802154_ADDR_LEN];
//	Status status;
//	std::vector<Accessory> accessories;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> recv;
	std::shared_ptr<MessagePkg::Queue<MessagePkg::Message>> send;
};

}

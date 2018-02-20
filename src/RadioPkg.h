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
#include <sys/types.h>
#include <sys/socket.h>
//#include <stdio.h>
//#include "af_ieee802154.h"


#define IEEE802154_ADDR_LEN 8
#define MAX_PACKET_LEN 127
#define EXTENDED 0

enum {
	IEEE802154_ADDR_NONE = 0x0,
	IEEE802154_ADDR_SHORT = 0x2,
	IEEE802154_ADDR_LONG = 0x3,
};


struct ieee802154_addr_sa {
	int addr_type;
	uint16_t pan_id;
	union {
		uint8_t hwaddr[IEEE802154_ADDR_LEN];
		uint16_t short_addr;
	};
};

struct sockaddr_ieee802154 {
	sa_family_t family;
	struct ieee802154_addr_sa addr;
};

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

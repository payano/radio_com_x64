/*
 * IEEE 802.15.4 socket example
 *
 * Copyright (C) 2016 Samsung Electronics Co., Ltd.
 *
 * Author: Stefan Schmidt <stefan@osg.samsung.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* gcc af_ieee802154_rx.c -o af_ieee802154_rx */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

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

int ieee802154_recv();
int ieee802154_send();

#ifdef __cplusplus
}
#endif

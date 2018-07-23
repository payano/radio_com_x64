#include "af_ieee802154.h"
#include <string.h>

int ieee802154_recv() {
	int ret, sd;
	struct sockaddr_ieee802154 src, dst;
	unsigned char buf[MAX_PACKET_LEN + 1];
	socklen_t addrlen;
	/* IEEE 802.15.4 extended address to receive frames on, adapt to your setup */
	uint8_t long_addr[IEEE802154_ADDR_LEN] = {0xd6, 0x55, 0x2c, 0xd6, 0xe4, 0x1c, 0xeb, 0x57};

	/* Create IEEE 802.15.4 address family socket for the SOCK_DGRAM type */
	sd = socket(PF_IEEE802154, SOCK_DGRAM, 0);
	if (sd < 0) {
		perror("socket");
		return 1;
	}

	/* Prepare source socket address struct */
	memset(&src, 0, sizeof(src));
	src.family = AF_IEEE802154;
	/* Used PAN ID is 0x23 here, adapt to your setup */
	src.addr.pan_id = 0x0023;

#if EXTENDED /* IEEE 802.15.4 extended address usage */
	src.addr.addr_type = IEEE802154_ADDR_LONG;
	memcpy(&src.addr.hwaddr, &long_addr, IEEE802154_ADDR_LEN);
#else
	src.addr.addr_type = IEEE802154_ADDR_SHORT;
	src.addr.short_addr = 0x0002;
#endif

	/* Bind socket on this side */
	ret = bind(sd, (struct sockaddr *)&src, sizeof(src));
	if (ret) {
		perror("bind");
		close(sd);
		return 1;
	}

	addrlen = sizeof(dst);

	/* Infinite loop receiving 802.15.4 frames and print out */
	while (1) {
		ret = recvfrom(sd, buf, MAX_PACKET_LEN, 0, (struct sockaddr *)&dst, &addrlen);
		if (ret < 0) {
			perror("recvfrom");
			continue;
		}
		buf[ret] = '\0';
#if EXTENDED
		printf("Received (from %s): %s\n", dst.addr.hwaddr, buf);
#else
		printf("Received (from %x): %s\n", dst.addr.short_addr, buf);
#endif
	}

	shutdown(sd, SHUT_RDWR);
	close(sd);
	return 0;
}

int ieee802154_send(){
	int sd;
	ssize_t len;
	struct sockaddr_ieee802154 dst;
	unsigned char buf[MAX_PACKET_LEN + 1];
	memset(buf,0,MAX_PACKET_LEN + 1);
	/* IEEE 802.15.4 extended send address, adapt to your setup */
	uint8_t long_addr[IEEE802154_ADDR_LEN] = {0xd6, 0x55, 0x2c, 0xd6, 0xe4, 0x1c, 0xeb, 0x57};

	/* Create IEEE 802.15.4 address family socket for the SOCK_DGRAM type */
	sd = socket(PF_IEEE802154, SOCK_DGRAM, 0);
	if (sd < 0) {
		perror("socket");
		return 1;
	}

	/* Prepare destination socket address struct */
	memset(&dst, 0, sizeof(dst));
	dst.family = AF_IEEE802154;
	/* Used PAN ID is 0x23 here, adapt to your setup */
	dst.addr.pan_id = 0x0023;

#if EXTENDED /* IEEE 802.15.4 extended address usage */
	dst.addr.addr_type = IEEE802154_ADDR_LONG;
	memcpy(&dst.addr.hwaddr, long_addr, IEEE802154_ADDR_LEN);
#else
	dst.addr.addr_type = IEEE802154_ADDR_SHORT;
	dst.addr.short_addr = 0x6001;
#endif

	//sprintf(buf, "Hello world from IEEE 802.15.4 socket example!");

	/* sendto() is used for implicity in this example, bin()/send() would
	 * be an alternative */
	char bufa[] = {0x6a, 0x6f, 0x68, 0x61, 0x6e, 0x20, 0x73, 0x6b, 0x69, 0x63, 0x6b, 0x61, 0x72};

	len = sendto(sd, bufa, strlen(bufa), 0, (struct sockaddr *)&dst, sizeof(dst));
	if (len < 0) {
		perror("sendto");
	}

	shutdown(sd, SHUT_RDWR);
	close(sd);
	return 0;
}

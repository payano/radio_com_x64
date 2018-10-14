/*
 * Radio.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: johan
 */

#include "Radio.h"
#include <iostream>
#include <assert.h>
#include <string.h>
#include <unistd.h>
namespace radio {

Radio::Radio(std::unique_ptr<radio::RadioSettings>& radioSettings):
	radioSettings(std::move(radioSettings)),
	threadRecv(nullptr),
	threadSend(nullptr),
	runningStatus(common::Status::Stopped),
	radio_com(this->radioSettings),
	runningRecv(false),
	runningSend(false)
{}
Radio::Radio(std::unique_ptr<radio::RadioSettings>&& radioSettings):
	radioSettings(std::move(radioSettings)),
	threadRecv(nullptr),
	threadSend(nullptr),
	runningStatus(common::Status::Stopped),
	radio_com(this->radioSettings),
	runningRecv(false),
	runningSend(false)
{}


Radio::~Radio()
{
	// Stop thread
	stop();
}

const common::Status& Radio::getRadioStatus(){return radioSettings->status;}
const common::Status& Radio::getRunningStatus(){return runningStatus;}
const std::unique_ptr<RadioSettings>& Radio::getSettings(){
	return radioSettings;
}

void Radio::start(){
	assert(radioSettings != nullptr);
	assert(radioSettings->recieve != nullptr);
	assert(radioSettings->send != nullptr);

	std::cout << "FILE: " << __FILE__ << ", LINE: " << __LINE__ << std::endl;
	// Want to run Mqtt inside its own thread.
	if(radioThread.get() == nullptr){
		// start a new thread and return to the caller.
	std::cout << "FILE: " << __FILE__ << ", LINE: " << __LINE__ << std::endl;
		radioThread = std::make_unique<std::thread>(&Radio::start, this);
		radioThread->detach();
		return;
	}

	std::cout << "FILE: " << __FILE__ << ", LINE: " << __LINE__ << std::endl;
	if(threadRecv != nullptr){throw std::invalid_argument("Thread already started.");}
	std::cout << "FILE: " << __FILE__ << ", LINE: " << __LINE__ << std::endl;
	// Create a thread and start running the instance.
	threadRecv = std::make_unique<std::thread>(&Radio::runRecv, this);
	threadSend= std::make_unique<std::thread>(&Radio::runSend, this);
	// Detach it so it runs in background
	std::cout << "Started thread Radio.\n";
	threadRecv->join();
	threadSend->join();
	std::cout << "Closing thread Radio.\n";

//	pthread_cancel(runningThread->native_handle());

	// Make it runnable again
	threadRecv.reset();
	radioThread.reset();
}

void Radio::runRecv(){
	using namespace common;

	// From mqtt
//	if(runningStatus == Status::Runnning && runningRecv){return;}

	runningRecv = true;
	runningStatus = Status::Runnning;

	std::cout << "want to recv from radio.." << std::endl;

	ieee802154_recv();

//	while(runningStatus == Status::Runnning)
//	{
//		if(!radioSettings->recieve->isEmpty()){
//			MessagePkg::Message message;
//			radioSettings->recieve->pop(message);
//		}
//
//		std::this_thread::sleep_for(std::chrono::milliseconds(THREADDELAY));
//	}


	// Remove all connections to Mqtt broker server...
	//radio_com.disconnect();
	// Reset all statuses in settings.
	runningRecv = false;

	runningStatus = runningRecv && runningSend ? Status::Stopping : Status::Stopped;
//	runningThread.reset();
	std::cout << "Quit thread Radio!\n";
}

void Radio::runSend(){
	using namespace common;

	// to mqtt
//	if(runningStatus == Status::Runnning && runningSend){return;}

	runningSend = true;
	runningStatus = Status::Runnning;

	while(runningStatus == Status::Runnning)
	{
		// Recieved from radio will be sen to mqtt
//		ieee802154_recv();

		while(!radioSettings->recieve->isEmpty()){
			MessagePkg::Message outgoing;
			radioSettings->recieve->pop(outgoing);
			std::string messageSend = outgoing.base;
//			messageSend.append(" " + outgoing.topic);
			messageSend.append(" " + outgoing.value);
			ieee802154_send(messageSend.c_str());
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			ieee802154_send(messageSend.c_str());
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			ieee802154_send(messageSend.c_str());
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			std::cout << ">> this happens" << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	// Remove all connections to Mqtt broker server...
	//radio_com.disconnect();
	// Reset all statuses in settings.

	runningSend = false;

	runningStatus = runningRecv && runningSend ? Status::Stopping : Status::Stopped;

	std::cout << "Quit thread Radio!\n";
}

void Radio::stop(){
	using namespace common;
	runningStatus = Status::Stopping;
	// Give thread time to quit
//	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}


int Radio::ieee802154_recv() {
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

	std::cout << "will now wait for recv..." << std::endl;
	/* Infinite loop receiving 802.15.4 frames and print out */
	while (1) {
		ret = recvfrom(sd, buf, MAX_PACKET_LEN, 0, (struct sockaddr *)&dst, &addrlen);
		if (ret < 0) {
			perror("recvfrom");
			continue;
		}
		buf[ret] = '\0';
#if EXTENDED
		printf("<< Received (from %s): %s\n", dst.addr.hwaddr, buf);
#else
		printf("<< Received (from %x): %s\n", dst.addr.short_addr, buf);
#endif
		MessagePkg::Message message;
		std::string stringMessage(reinterpret_cast<char*>(buf));

		auto space = stringMessage.find(" ");
		message.base = stringMessage.substr(0, space);
		stringMessage = stringMessage.substr(space+1);
//		space = stringMessage.find(" ");
//		message.topic = stringMessage.substr(0, space);
//		stringMessage = stringMessage.substr(space+1);
		space = stringMessage.find(" ");
		message.value = stringMessage.substr(0,space);
		std::this_thread::sleep_for(std::chrono::milliseconds(80));

		std::cout << "<< base: " << message.base << std::endl;
		std::cout << "<< topic: " << message.topic << std::endl;
		std::cout << "<< value: " << message.value << std::endl;
		radioSettings->send->push(message);


//		radioSettings->send
	}

	shutdown(sd, SHUT_RDWR);
	close(sd);
	return 0;
}

int Radio::ieee802154_send(std::string message){
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
//	char bufa[] = {0x6a, 0x6f, 0x68, 0x61, 0x6e, 0x20, 0x73, 0x6b, 0x69, 0x63, 0x6b, 0x61, 0x72};
	if(message.length() > MAX_PACKET_LEN){message.substr(0,MAX_PACKET_LEN);}
	char bufa[] = "HEEEJJAAAAAAAA";

	std::cout << ">> sending radio" << std::endl;
	std::cout << ">> message: " << message.c_str() << std::endl;
	len = sendto(sd, message.c_str(), message.size(), 0, (struct sockaddr *)&dst, sizeof(dst));
	if (len < 0) {
		perror("sendto");
	}

	shutdown(sd, SHUT_RDWR);
	close(sd);
	return 0;
}

}




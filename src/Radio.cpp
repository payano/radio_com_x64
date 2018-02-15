/*
 * Radio.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: johan
 */

#include "Radio.h"
#include <iostream>
#include <assert.h>

namespace radio {

Radio::Radio(std::unique_ptr<radio::RadioSettings>& radioSettings):
	radioSettings(std::move(radioSettings)),
	threadRecv(nullptr),
	runningStatus(common::Status::Stopped),
	radio_com(this->radioSettings),
	runningRecv(false),
	runningSend(false)
{}
Radio::Radio(std::unique_ptr<radio::RadioSettings>&& radioSettings):
	radioSettings(std::move(radioSettings)),
	threadRecv(nullptr),
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
	assert(radioSettings->recv != nullptr);
	assert(radioSettings->send != nullptr);

	// Want to run Mqtt inside its own thread.
	if(radioThread.get() == nullptr){
		// start a new thread and return to the caller.
		radioThread = std::make_unique<std::thread>(&Radio::start, this);
		radioThread->detach();
		return;
	}

	if(threadRecv != nullptr){throw std::invalid_argument("Thread already started.");}
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

	if(runningStatus == Status::Runnning && runningRecv){return;}

	runningRecv = true;
	runningStatus = Status::Runnning;

	while(runningStatus == Status::Runnning)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(THREADDELAY));
	}

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

	if(runningStatus == Status::Runnning && runningSend){return;}

	runningSend = true;
	runningStatus = Status::Runnning;

	while(runningStatus == Status::Runnning)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(THREADDELAY));
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

}

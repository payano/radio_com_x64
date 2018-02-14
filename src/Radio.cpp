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
	runningThread(nullptr),
	runningStatus(common::Status::Stopped),
	radio_com(this->radioSettings)
{}
Radio::Radio(std::unique_ptr<radio::RadioSettings>&& radioSettings):
	radioSettings(std::move(radioSettings)),
	runningThread(nullptr),
	runningStatus(common::Status::Stopped),
	radio_com(this->radioSettings)
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

	if(runningThread != nullptr){throw std::invalid_argument("Thread already started.");}
	// Create a thread and start running the instance.
	runningThread = std::make_unique<std::thread>(&Radio::run, this);
	// Detach it so it runs in background
	runningThread->join();

//	pthread_cancel(runningThread->native_handle());

	// Make it runnable again
	runningThread.reset();
	radioThread.reset();
}

void Radio::run(){
	using namespace common;
	std::cout << "Started thread Radio.\n";

	if(runningStatus == Status::Runnning){return;}

	runningStatus = Status::Runnning;

	while(runningStatus == Status::Runnning)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(THREADDELAY));
	}
	std::cout << "Closing thread Radio.\n";

	// Remove all connections to Mqtt broker server...
	//radio_com.disconnect();
	// Reset all statuses in settings.

	runningStatus = Status::Stopped;
//	runningThread.reset();
	std::cout << "Quit thread Radio!\n";}

void Radio::stop(){
	using namespace common;
	runningStatus = Status::Stopping;
	// Give thread time to quit
//	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

}

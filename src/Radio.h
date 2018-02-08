/*
 * Radio.h
 *
 *  Created on: Feb 8, 2018
 *      Author: johan
 */

#pragma once
#include "runInstance.h"
#include <memory>
#include "RadioPkg.h"
#include "CommonPkg.h"
#include "Radio_com.h"
#include <thread>

namespace radio {

class Radio : public runInstance{
public:
	Radio(std::unique_ptr<RadioSettings>& radioSettings);
	Radio(std::unique_ptr<RadioSettings>&& radioSettings);
	virtual ~Radio();
	const std::unique_ptr<RadioSettings>& getSettings();
	const common::Status& getRadioStatus();
	const common::Status& getRunningStatus();
	void start() override;
	void stop() override;
private:
	void run();
	std::unique_ptr<RadioSettings> radioSettings;
	std::unique_ptr<std::thread> runningThread;
	std::unique_ptr<std::thread> radioThread;
	common::Status runningStatus;
	Radio_com radio_com;

};

}

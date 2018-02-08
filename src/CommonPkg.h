/*
 * CommonPkg.h
 *
 *  Created on: Feb 6, 2018
 *      Author: johan
 */

#pragma once

static constexpr unsigned int THREADDELAY = 100; // ms

namespace common {
enum Status {
	Disconnected = 0,
	Connnecting = 1,
	Connected = 2,
	Stopped = 3,
	Runnning = 4,
	Stopping = 5
};

}

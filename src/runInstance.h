/*
 * runInstance.h
 *
 *  Created on: Feb 8, 2018
 *      Author: johan
 */

#pragma once

class runInstance {
public:
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual ~runInstance(){};
};

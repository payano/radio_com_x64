/*
 * Radiocom.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: johan
 */

#include "Radio_com.h"

namespace radio {

Radio_com::Radio_com(std::unique_ptr<RadioSettings>& RadioSettings):
settings(RadioSettings.get())
{}

Radio_com::~Radio_com() {
	// TODO Auto-generated destructor stub
}

}

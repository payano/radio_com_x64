/*
 * Radiocom.h
 *
 *  Created on: Feb 8, 2018
 *      Author: johan
 */

#pragma once
#include "RadioPkg.h"
#include <memory>

namespace radio {
class Radio_com {
public:
	Radio_com(std::unique_ptr<RadioSettings>& radioSettings);
	virtual ~Radio_com();
private:
	RadioSettings* settings;
};
}

//============================================================================
// Name        : radio_com.cpp
// Author      : Johan Svensson
// Version     :
// Copyright   : Free For All
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "af_ieee802154.h"
#include "Mqtt.h"

using namespace std;

int main() {
	ieee802154_send();

	cout << "Hello!" << endl; // prints Hello!
	return 0;
}

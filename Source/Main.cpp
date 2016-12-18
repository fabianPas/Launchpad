
#include "../JuceLibraryCode/JuceHeader.h"
#include "Launchpad.h"

#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include "MidiCallback.h"

int main ()
{
	StringArray inputDevices = MidiInput::getDevices();
	StringArray outputDevices = MidiOutput::getDevices();

	std::vector<std::string> inputs, outputs;
	std::vector<int> inputPorts, outputPorts;

	std::cout << "Launchpad Interface V1.0.2\n";
	std::cout << "\n";

	for (int i = 0; i < inputDevices.size(); i++) {
		std::cout << "\tDevice #" << i << ": " << inputDevices[i] << "\n";
	}

	std::string ports;
	std::cout << "\nPlease select one or more (0, 1..) input devices: ";
	
	std::getline(std::cin, ports);
	boost::split(inputs, ports, boost::is_any_of(","));

	std::cout << "\n";

	for (size_t i = 0; i < inputs.size(); ++i)
	{
		boost::trim(inputs[i]);
		int port = std::stoi(inputs[i]);
		if (port <= inputDevices.size()) {// it's in there (probably)
			inputPorts.push_back(port);
		}
	}

	for (int i = 0; i < outputDevices.size(); i++) {
		std::cout << "\tDevice #" << i << ": " << outputDevices[i] << "\n";
	}

	std::cout << "\nPlease select one or more (0, 1..) output devices: ";
	std::getline(std::cin, ports);
	boost::split(outputs, ports, boost::is_any_of(","));

	for (size_t i = 0; i < outputs.size(); ++i) {
		boost::trim(outputs[i]);
		int port = std::stoi(outputs[i]);
		if (port <= outputDevices.size()) {// it's in there (probably)
			outputPorts.push_back(port);
		}
	}

	Launchpad *launchpad = new Launchpad(inputPorts, outputPorts);

	std::cout << "Press any button to exit" << "\n";
	std::cin.get();

	launchpad->clearController();


	delete launchpad;
	return 0;

}


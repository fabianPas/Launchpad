/*
  ==============================================================================

    Controller.h
    Created: 3 Jul 2015 8:16:51pm
    Author:  Fabian

  ==============================================================================
*/

#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MidiCallback;

class Controller {
protected:
	std::vector<MidiOutput*> outputs;
	std::vector<MidiInput*> inputs;
	std::vector<MidiOutput*> mainOutputs;

	virtual ~Controller();
public:
	Controller(std::vector<int> inputPorts, std::vector<int> outputPorts);

	virtual void initializeController() = 0;
	virtual void handleMessage(int byte1, int byte2, int byte3) = 0;
	virtual void sendMessage(int byte1, int byte2, int byte3, int channel = 1, bool controllerOnly = false);

	virtual std::string getControllerID() = 0;


	void addOutput(MidiOutput *output);
	MidiOutput* getOutput(int index);
	std::vector<MidiOutput*> getOutputs();
};



#endif  // CONTROLLER_H_INCLUDED

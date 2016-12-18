/*
  ==============================================================================

    Controller.cpp
    Created: 3 Jul 2015 8:16:51pm
    Author:  Fabian

  ==============================================================================
*/

#include "Controller.h"
#include "MidiCallback.h"
#include <memory>

#include <boost/shared_ptr.hpp>

Controller::Controller(std::vector<int> inputPorts, std::vector<int> outputPorts)
{
	for (size_t i = 0; i < inputPorts.size(); ++i) {
		MidiInput* input = MidiInput::openDevice(inputPorts[i], new MidiCallback(this));
		input->start();

		// We also have to keep a vector of the inputs we openened to close them again later on
		this->inputs.push_back(input);
	}

	for (size_t i = 0; i < outputPorts.size(); ++i) {
		MidiOutput *output = MidiOutput::openDevice(outputPorts[i]);
		this->outputs.push_back(output);
	}

}

void Controller::sendMessage(int byte1, int byte2, int byte3, int channel, bool controllerOnly)
{
	// we gonna add a channel to this so we can actually send channel 1-16 messages
	MidiMessage message = MidiMessage::MidiMessage(byte1, byte2, byte3);
	message.setChannel(channel);

	if (controllerOnly)
	{
		for (size_t i = 0; i < this->mainOutputs.size(); ++i)
			this->mainOutputs[i]->sendMessageNow(message);
	}
	else {
		for (size_t i = 0; i < this->outputs.size(); ++i) {
			this->outputs[i]->sendMessageNow(message);
		}
	}
}

void Controller::addOutput(MidiOutput* output)
{
	if (output) {
		this->outputs.push_back(output);
	}
}

std::vector<MidiOutput*> Controller::getOutputs()
{
	return this->outputs;
}

MidiOutput* Controller::getOutput(int index)
{
	return this->outputs[index];
}

Controller::~Controller()
{
	for (size_t i = 0; i < this->outputs.size(); ++i) {
		delete this->outputs[i];
	}

	for (size_t i = 0; i < this->inputs.size(); ++i) {
		delete this->inputs[i];
	}

	this->inputs.clear();
	this->outputs.clear();
}

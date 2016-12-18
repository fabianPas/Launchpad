#include "../JuceLibraryCode/JuceHeader.h"
#include <boost/lexical_cast.hpp>

#include "MidiCallback.h"

MidiCallback::MidiCallback(Controller *controller)// : controller(controller)
{
	this->controller = controller;
}

void MidiCallback::handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message) {

	if (static_cast<int>(*message.getRawData()) == 176) {
		onKeyUp(source, message);
		return;
	}

	float velocity = message.getFloatVelocity();
	if (velocity == 1)
		onKeyDown(source, message);
	else
		onKeyUp(source, message);

}

void MidiCallback::onKeyDown(MidiInput *source, const MidiMessage &message) {
	this->controller->handleMessage(static_cast<int>(*message.getRawData()), message.getNoteNumber(), 0);
}

void MidiCallback::onKeyUp(MidiInput *source, const MidiMessage &message) {
	this->controller->handleMessage(static_cast<int>(*message.getRawData()), message.getNoteNumber(), 1);
}
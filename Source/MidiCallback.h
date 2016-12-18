#include "../JuceLibraryCode/JuceHeader.h"
#include "Controller.h"

class MidiCallback: public MidiInputCallback {
	Controller *controller;

public:
	MidiCallback(Controller *controller);

	void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message) override;
	void onKeyDown(MidiInput *source, const MidiMessage &message);
	void onKeyUp(MidiInput *source, const MidiMessage &message);
};
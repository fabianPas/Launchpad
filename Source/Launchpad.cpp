/*
  ==============================================================================

    Launchpad.cpp
    Created: 30 Jun 2015 7:24:18am
    Author:  Fabian

  ==============================================================================
*/

#include "Launchpad.h"
#include "MidiCallback.h"

Launchpad::Launchpad(std::vector<int> inputPorts, std::vector<int> outputPorts) : Controller(inputPorts, outputPorts) {

	grid.resize(5);
	for (int i = 0; i < 5; ++i)
		grid[i].resize(9);

	StringArray outputDevices = MidiOutput::getDevices();
	for (size_t i = 0; i < outputPorts.size(); ++i) {

		MidiOutput *output = MidiOutput::openDevice(outputPorts[i]);

		// can we optimize this? we select the "correct" midi by using the controller's ID
		std::string portName = outputDevices[outputPorts[i]].toStdString();
		if (portName.find(Launchpad::getControllerID()) != std::string::npos)
			this->mainOutputs.push_back(output);
		
		this->outputs.push_back(output);
	}

	// We turn of all leds and we send 40 to automatically handle led flashing
	Launchpad::sendMessage(176, 0, 0);
	Launchpad::sendMessage(176, 0, 40);

	Launchpad::initializeController();
	initializeDecks();
}

std::string Launchpad::getControllerID()
{
	return "Launchpad";
}


void Launchpad::initializeController() 
{
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			grid[y][x] = false;
			if (x == 8)
				this->sendMessage(144, (16 * y) + x, 63, 1, true);
			else 
				this->sendMessage(144, (16 * y) + x, 15, 1, true);
		}
	}

	// Initialize the utility buttons
	for (int i = 80; i <= 103; i++) {
		this->sendMessage(144, i, 60, 1, true);
	}

	// We have the first control deck selected on the launchpad (layers, decks, clips)
	this->sendMessage(176, 104, 60, 1, true);
}

void Launchpad::clearController()
{
	Launchpad::sendMessage(176, 0, 0);
}

// @TODO make selected deck change channel
void Launchpad::initializeDecks()
{
	// up to 8 decks
	for (int i = 0; i < 8; ++i)
	{
		// no we have to use a map so we can identify the button ID's
		decks.insert(std::pair<int, bool>(112 + i, false));
		
		// set the decks initial leds 104... check programmer reference of launchpad
		this->sendMessage(144, 112 + i, 63, 1, true);
	}

	// we have the first led enabled
	decks[112] = true;

	selectedDeck = 0;

	this->sendMessage(144, 112, 60, true);

	initializeController();
}


void Launchpad::handleMessage(int byte1, int byte2, int byte3)
{
	if (byte2 >= 80 && byte2 <= 103) {
		if (byte3 == 0) {
			this->sendMessage(144, byte2, 127);
			this->sendMessage(144, byte2, 59, 1, true);
			return;
		}
		else {
			this->sendMessage(128, byte2, 0);
			this->sendMessage(144, byte2, 60, 1, true);
		}
	}

	// fix that byte3 shit make it an key up/down bool
	if (byte3 == 0) {
		// Note on
		if (byte1 == 144)
		{
			// Utility buttons available for effects, and whatnot.

			// its the bottom buttons and we want to switch decks!
			if (byte2 >= 112 && byte2 <= 119)
			{
				for (auto it : decks)
				{
					//@TODO change decks from 112 based to id based and add 112+id
					if (it.second == true) {
						this->sendMessage(144, it.first, 63, true);
						decks[it.first] = false;
					}
				}
				this->sendMessage(144, byte2, 60);
				decks[byte2] = true;

				// @TODO make this clearLayer() and only update the LED's that are actually TRUE thus flickering
				initializeController();

				return;
			}

			// we only have 4 layers so we don't want unused buttons to be pressable 
			if (byte2 / 16 >= 4)
				return;
				

			// @TODO clear up these calculations, so many put it in var
			if (byte2 == (16 * (byte2 / 16)) + 8)
			{
				this->sendMessage(144, byte2, 63, 1, true);
				// clear clip from layer
				this->sendMessage(144, byte2, 15, 15);
				//grid[byte2 / 16][byte2 % 8] = false;

				// We want to stop the leds that are blinking because we just cleared the fucking layer
				for (int x = 0; x < 8; ++x)
				{
					// if its TRUE thus blinking
					if (grid[byte2 / 16][x])
					{
						this->sendMessage(144, (16 * (byte2 / 16)) + x, 15, 1, true);
						grid[byte2 / 16][x] = false;
					}
					//this->sendMessage(144, (16 * (byte2 / 16)) + x, 15, true);
				}

				return;
			}

			// x = index mod width
			// y = index / 16
			// we will only send this message to the controller because else it will send some crazy amount of messages to your vj program (resolume)
			for (int x = 0; x < 8; ++x)
			{
				// if its TRUE thus blinking
				if (grid[byte2 / 16][x])
				{
					this->sendMessage(144, (16 * (byte2 / 16)) + x, 15, 1, true);
					grid[byte2 / 16][x] = false;
				}
				//this->sendMessage(144, (16 * (byte2 / 16)) + x, 15, true);
			}

			// update that shit, we use the second channel for resolume because for some reason the 1 channel is f*cked up
			this->sendMessage(144, byte2, 56, 2);
			this->sendMessage(144, byte2, 56, 1, true);
			grid[byte2 / 16][byte2 % 16] = true;
		}
	} else
	{
		if (byte1 == 176)
		{
			// dit zijn de knoppen  die boven aan zitten
			// het is 1 knop per deck -- max wss 4
			// allemaal rood (misschien wel oranje), geslecteerde wordt groen
			// bij een nieuw deck wordt het bordt weer geinitieerd
			/*for (auto it : decks)
			{
				if (it.second == true) {
					this->sendMessage(176, it.first, 63, true);
					decks[it.first] = false;
				}
			}
			std::cout << "wtf" << byte2 + 17;
			this->sendMessage(176, byte2, 60, true);
			this->sendMessage(176, byte2 + 17, 60, 15);
			decks[byte2] = true;*/
			//this->sendMessage(176, 105, 60);
		}
	}
}





Launchpad::~Launchpad() {
	//delete callback;
}


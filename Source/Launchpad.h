/*
  ==============================================================================

    Launchpad.h
    Created: 30 Jun 2015 7:24:33am
    Author:  Fabian

  ==============================================================================
*/
#ifndef LAUNCHPAD_H_INCLUDED
#define LAUNCHPAD_H_INCLUDED

#include "Controller.h"
#include <map>


class Launchpad : public Controller {
public:
	virtual void initializeController() override;
	virtual void handleMessage(int byte1, int byte2, int byte3) override;
	virtual std::string getControllerID() override;

	Launchpad(std::vector<int> inputPorts, std::vector<int> outputPorts);
	std::vector<std::vector<bool>> grid;
	std::map<int, bool> decks;
	int selectedDeck;

	void clearController();

	void initializeDecks();

	virtual ~Launchpad() override;

};


#endif  // LAUNCHPAD_H_INCLUDED

#pragma once

#include "pch.h"
#include "PacketHandler.h"

class Manager
{
public:
	Manager();
	~Manager();

	void openPort();

private:
	void showResult();
	void showFeedback(Feedback feedbackID);

	PacketHandler pktHandler;
	string data;
};


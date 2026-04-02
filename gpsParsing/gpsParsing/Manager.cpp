#include "pch.h"
#include "Manager.h"
#include <string>
#include <windows.h>
Manager::Manager(){}

Manager::~Manager(){}

void Manager::openPort()
{
    while (true)
    {
        cout << "\nEnter NMEA string (or 'exit'):\n";
        getline(cin, data);

        if (data == "exit")
            break;

        showResult();
    }
}

void Manager::showResult()
{
    PointInfo point = pktHandler.parsePacket(data);
    Feedback feedback = pktHandler.getFeedbackValue();

    if (feedback == OK)
    {
        cout << "\n\nGPS: " << point.lat << ' ' << point.latHem
            << ", " << point.lon << ' ' << point.lonHem << " | "
            << "Satelites: " << point.sats << "\n\n";
    }
    else
        showFeedback(feedback);
}

void Manager::showFeedback(Feedback feedbackID)
{
    switch (feedbackID)
    {
    case OK:
        break;
    case Error_CRC:
        cout << "\nBad CRC!\n\n";
        break;
    case ERROR_EMPTY_DATA:
        cout << "\nEmpty data!\n\n";
        break;
    case ERROR_UNKNOWN_FORMAT:
        cout << "\nUnknow format!\n\n";
        break;
    default:
        return;
    }
}


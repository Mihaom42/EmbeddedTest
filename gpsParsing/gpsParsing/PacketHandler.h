#pragma once

#include "pch.h"
#include <vector>
#include <cstdint>

struct PointInfo
{
    float lat = 0.0f;
    char latHem = ' ';
    float lon = 0.f;
    char lonHem = ' ';
    float sats = 0.0f;
};

enum Feedback
{
    OK,
    Error_CRC,
    ERROR_EMPTY_DATA,
    ERROR_UNKNOWN_FORMAT
};

enum Fields
{
    t = 0,
    lat = 1,
    latHem = 2,
    lon = 3,
    lonHem = 4,
    quality = 5,
    sats = 6
};

class PacketHandler
{
public:
	PacketHandler();
	~PacketHandler();
	
    PointInfo parsePacket(const string& data);
    Feedback getFeedbackValue() { return feedback; }

private:
    void readStreamData(const string& data, vector<int>& fieldStartIndex);
    void collectPointInfo(const vector<string>& parseData, PointInfo& p);
    float nmeaToDecimal(const string& val);

    uint8_t calcChecksum(const string& data);
    bool validateCRC(const string& data);

    bool checkData(const string& data);
    void setFeedback(Feedback feedbackID = OK);

    Feedback feedback = OK;
    vector<int> fieldsIndex;
    vector<string> parseData;
};


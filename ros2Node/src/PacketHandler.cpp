#include "PacketHandler.h"
#include <iostream>

PacketHandler::PacketHandler(){}

PacketHandler::~PacketHandler(){}

void PacketHandler::readStreamData(const std::string& data, std::vector<int>& fieldStartIndex)
{
    if (data.size() == 0)
        return;

    if (data[0] != '$')
        return;

    for (uint8_t i = 0; i < data.length(); i++)
    {
        if (data[i] == '*')
            break;

        if (data[i] == ',')
            fieldStartIndex.push_back(i + 1);
    }
}

PointInfo PacketHandler::parsePacket(const std::string& data)
{
    if (!checkData(data))
        return PointInfo();

    setFeedback(OK);
    fieldsIndex.clear();
    parseData.clear();

    readStreamData(data, fieldsIndex);

    if (fieldsIndex.empty())
        return PointInfo();

    uint8_t size = fieldsIndex.size() - 1;
    for (uint8_t i = 0; i < size; i++)
    {
        if (i + 1 < size)
            parseData.push_back(data.substr(fieldsIndex[i], fieldsIndex[i + 1] - fieldsIndex[i] - 1));
    }

    PointInfo p;
    collectPointInfo(parseData, p);

    return p;
}

void PacketHandler::collectPointInfo(const std::vector<std::string>& parseData, PointInfo& p)
{
    p.lat = nmeaToDecimal(parseData[lat]);
    p.latHem = parseData[latHem][0];
    p.lon = nmeaToDecimal(parseData[lon]);
    p.lonHem = parseData[lonHem][0];
    p.sats = stoi(parseData[sats]);
}

float PacketHandler::nmeaToDecimal(const std::string& val)
{
    float raw = stof(val);

    int degrees = (int)raw / 100;
    float mins = raw - (degrees * 100);

    return degrees + mins / 60.0f;
}

bool PacketHandler::validateCRC(const std::string& data)
{
    size_t starPos = data.find('*');

    if (starPos == std::string::npos)
        return false;

    if (starPos + 2 >= data.size())
        return false;

    std::string hexStr = data.substr(starPos + 1, 2);

    uint8_t expected = static_cast<uint8_t>(stoi(hexStr, nullptr, 16));
    uint8_t actual = calcChecksum(data);

    return expected == actual;
}

uint8_t PacketHandler::calcChecksum(const std::string& data)
{
    uint8_t cs = 0;
    size_t i = 1;

    while (i < data.size() && data[i] != '*')
    {
        cs ^= static_cast<uint8_t>(data[i]);
        i++;
    }

    return cs;
}

bool PacketHandler::checkData(const std::string& data)
{
    if (data.size() == 0)
    {
        setFeedback(ERROR_EMPTY_DATA);
        return false;
    }

    if (data.find("$GPGGA") != 0)
    {
        setFeedback(ERROR_UNKNOWN_FORMAT);
        return false;
    }

    if (!validateCRC(data))
    {
        setFeedback(Error_CRC);
        return false;
    }

    return true;
}

void PacketHandler::setFeedback(Feedback feedbackID)
{
    feedback = feedbackID;
}

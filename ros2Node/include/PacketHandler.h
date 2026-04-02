#pragma once

#include <vector>
#include <string>
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

    PointInfo parsePacket(const std::string& data);
    Feedback getFeedbackValue() { return feedback; }

private:
    void readStreamData(const std::string& data, std::vector<int>& fieldStartIndex);
    void collectPointInfo(const std::vector<std::string>& parseData, PointInfo& p);
    float nmeaToDecimal(const std::string& val);

    uint8_t calcChecksum(const std::string& data);
    bool validateCRC(const std::string& data);

    bool checkData(const std::string& data);
    void setFeedback(Feedback feedbackID = OK);

    Feedback feedback = OK;
    std::vector<int> fieldsIndex;
    std::vector<std::string> parseData;
};
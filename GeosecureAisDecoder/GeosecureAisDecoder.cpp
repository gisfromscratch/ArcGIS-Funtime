#include "GeosecureAisDecoder.h"

#include <QDateTime>
#include <QDebug>
#include <QThread>

GeosecureAisDecoder::GeosecureAisDecoder(QObject *parent) :
    QObject(parent)
{
    // Create the lookup once
    BuildNmeaLookup();
}

AisMessage GeosecureAisDecoder::createAisMessage(const char *nmea_payload, const size_t padding)
{
    Ais1_2_3 simpleAisMessage(nmea_payload, padding);
    int mmsi = simpleAisMessage.mmsi;
    EsriRuntimeQt::Point location(simpleAisMessage.x, simpleAisMessage.y);
    auto aisMessage = AisMessage(mmsi, location);
    aisMessage.setRotation(simpleAisMessage.rot);
    // TODO: Just a test
    QDateTime timestamp = QDateTime::currentDateTime();
    timestamp = timestamp.addSecs(simpleAisMessage.timestamp);
    aisMessage.setTimestamp(timestamp);
    return aisMessage;
}

#include "GeosecureAisDecoder.h"

#include <QThread>

GeosecureAisDecoder::GeosecureAisDecoder(QObject *parent) :
    QObject(parent)
{
    // Create the lookup once
    BuildNmeaLookup();
}

AisMessage *GeosecureAisDecoder::createAisMessage(const char *nmea_payload, const size_t padding)
{
    Ais1_2_3 simpleAisMessage(nmea_payload, padding);
    int mmsi = simpleAisMessage.mmsi;
    EsriRuntimeQt::Point location(simpleAisMessage.x, simpleAisMessage.y);
    return new AisMessage(mmsi, location, nullptr);
}

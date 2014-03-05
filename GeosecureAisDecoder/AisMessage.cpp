#include "AisMessage.h"

AisMessage::AisMessage(const int mmsi, const EsriRuntimeQt::Point &location, QObject *parent) :
    QObject(parent),
    _mmsi(mmsi),
    _location(location)
{
}

int AisMessage::mmsi() const
{
    return _mmsi;
}

const EsriRuntimeQt::Point &AisMessage::location() const
{
    return _location;
}

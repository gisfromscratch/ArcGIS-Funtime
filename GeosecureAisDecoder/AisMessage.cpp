#include "AisMessage.h"

AisMessage::AisMessage(const int mmsi, const EsriRuntimeQt::Point &location) :
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

float AisMessage::rotation() const
{
    return _rotation;
}

void AisMessage::setRotation(float rotation)
{
    _rotation = rotation;
}

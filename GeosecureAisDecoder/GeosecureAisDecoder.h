#ifndef GEOSECUREAISDECODER_H
#define GEOSECUREAISDECODER_H

#include "geosecureaisdecoder_global.h"
#include "ais.h"

class GEOSECUREAISDECODERSHARED_EXPORT GeosecureAisDecoder
{

public:
    GeosecureAisDecoder(const char *nmea_payload, const size_t padding);

private:
    Ais1_2_3 _decoder;
};

#endif // GEOSECUREAISDECODER_H

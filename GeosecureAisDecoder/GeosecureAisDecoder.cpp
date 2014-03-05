#include "GeosecureAisDecoder.h"


GeosecureAisDecoder::GeosecureAisDecoder(const char *nmea_payload, const size_t padding) :
    _decoder(Ais1_2_3(nmea_payload, padding))
{
}

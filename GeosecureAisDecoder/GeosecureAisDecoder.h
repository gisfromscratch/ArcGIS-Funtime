#ifndef GEOSECUREAISDECODER_H
#define GEOSECUREAISDECODER_H

#include "geosecureaisdecoder_global.h"
#include "ais.h"
#include "AisMessage.h"

#include <QObject>

/*!
 * \brief Simple AIS decoder.
 */
class GEOSECUREAISDECODERSHARED_EXPORT GeosecureAisDecoder : public QObject
{
    Q_OBJECT
public:
    GeosecureAisDecoder(QObject *parent = nullptr);

    /*!
     * \brief createAisMessage      Creates a new message.
     * \param nmea_payload          The payload for the NMEA message.
     * \param padding               The padding for the NMEA message.
     * \return                      A new message instance.
     */
    AisMessage createAisMessage(const char *nmea_payload, const size_t padding = 0);
};

#endif // GEOSECUREAISDECODER_H

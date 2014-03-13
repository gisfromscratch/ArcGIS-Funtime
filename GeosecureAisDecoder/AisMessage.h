#ifndef AISMESSAGE_H
#define AISMESSAGE_H

#include "geosecureaisdecoder_global.h"

#include <Point.h>

#include <QDateTime>

/*!
 * \brief Simple Ais message with location.
 */
class GEOSECUREAISDECODERSHARED_EXPORT AisMessage
{
public:
    explicit AisMessage(const int mmsi, const EsriRuntimeQt::Point &location);

    /*!
     * \brief mmsi  The unique identifier.
     * \return      Unique identifier
     */
    int mmsi() const;

    /*!
     * \brief location  WGS84 location of the Ais Vessel.
     * \return          WGS84 location
     */
    const EsriRuntimeQt::Point &location() const;

    /*!
     * \brief rotation  The rotation of the AIS Vessel.
     * \return          Approximated rotation of the AIS Vessel using deegree
     */
    float rotation() const;

    /*!
     * \brief setRotation   Sets the rotation of the AIS Vessel.
     * \param rotation      The rotation using deegree.
     */
    void setRotation(float rotation);

    /*!
     * \brief timestamp The UTC timestamp of the AIS message.
     * \return          UTC timestamp
     */
    QDateTime timestamp() const;

    /*!
     * \brief setTimestamp  Sets the UTC timestamp.
     * \param timestamp     The UTC timestamp
     */
    void setTimestamp(const QDateTime &timestamp);

private:
    const int _mmsi;
    const EsriRuntimeQt::Point _location;
    float _rotation;
    QDateTime _timestamp;
};

#endif // AISMESSAGE_H

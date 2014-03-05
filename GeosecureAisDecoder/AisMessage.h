#ifndef AISMESSAGE_H
#define AISMESSAGE_H

#include "geosecureaisdecoder_global.h"

#include <Point.h>

#include <QObject>

/*!
 * \brief Simple Ais message with location.
 */
class GEOSECUREAISDECODERSHARED_EXPORT AisMessage : public QObject
{
    Q_OBJECT
public:
    explicit AisMessage(const int mmsi, const EsriRuntimeQt::Point &location, QObject *parent = 0);

    /*!
     * \brief mmsi  The unique identifier.
     * \return      Unique identifier
     */
    int mmsi() const;

    /*!
     * \brief location  WGS84 location of the Ais object.
     * \return          WGS84 location
     */
    const EsriRuntimeQt::Point &location() const;

signals:

public slots:

private:
    const int _mmsi;
    const EsriRuntimeQt::Point _location;
};

#endif // AISMESSAGE_H

#ifndef AISREADER_H
#define AISREADER_H

#include "geosecureaisdecoder_global.h"
#include "AisMessage.h"
#include "GeosecureAisDecoder.h"

#include <QAtomicInt>
#include <QObject>

/*!
 * \brief Reader for NMEA AIS files. See http://www.aishub.net/
 */
class GEOSECUREAISDECODERSHARED_EXPORT AisReader : public QObject
{
    Q_OBJECT
public:
    explicit AisReader(QObject *parent = 0);
    ~AisReader();

    /*!
     * \brief readFileAsync  Reads the specified file.
     * \param filePath       The path to the NMEA AIS file.
     * \return               true if the file can be read
     */
    bool readFileAsync(const QString &filePath);

signals:
    /*!
     * \brief aisMessageVisited Signals that a new AIS message was obtained.
     * \param message           The AIS message
     */
    void aisMessageVisited(AisMessage *message);

public slots:

private:
    /*!
     * \brief readFile  Reads the specified file
     * \param filePath  The path to the NMEA AIS file.
     */
    void readFile(const QString &filePath);

    enum AisNmea { AisNmeaTokenIndex = 5, AisNmeaTokenCount = 7 };

    GeosecureAisDecoder *_decoder;
    QAtomicInt _cancel;
};

#endif // AISREADER_H

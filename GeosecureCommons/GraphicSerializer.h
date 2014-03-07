#ifndef GRAPHICSERIALIZER_H
#define GRAPHICSERIALIZER_H

#include "geosecurecommons_global.h"

#include <Graphic.h>

#include <QAtomicInt>
#include <QList>
#include <QObject>

/*!
 * \brief Serialize Graphics as JSON
 */
class GEOSECURECOMMONSSHARED_EXPORT GraphicSerializer : public QObject
{
    Q_OBJECT
public:
    explicit GraphicSerializer(QObject *parent = 0);

    /*!
     * \brief cancelSerialize   Cancels the serialization.
     */
    void cancelSerialize();

    /*!
     * \brief serializeGraphicsAsync    Serializes the graphics as JSON into the specified file asynchronously.
     * \param graphics                  The graphics to serialize.
     * \param filePath                  The path to the file.
     * \return                          true if the file can be written
     */
    bool serializeGraphicsAsync(QList<EsriRuntimeQt::Graphic> *graphics, const QString &filePath);

signals:
    /*!
     * \brief graphicsSerialized    Signals that the file was written.
     * \param filePath              The path to the file.
     */
    void graphicsSerialized(QString filePath);

public slots:

private:
    /*!
     * \brief serializeGraphics    Serializes the graphics as JSON into the specified file asynchronously.
     * \param graphics             The graphics to serialize.
     * \param filePath             The path to the file.
     */
    void serializeGraphics(QList<EsriRuntimeQt::Graphic> *graphics, const QString &filePath);

    QAtomicInt _cancel;
};

#endif // GRAPHICWRITER_H

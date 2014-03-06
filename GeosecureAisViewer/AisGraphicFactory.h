#ifndef AISGRAPHICFACTORY_H
#define AISGRAPHICFACTORY_H

#include <AisMessage.h>

#include <Graphic.h>
#include <SimpleMarkerSymbol.h>
#include <SpatialReference.h>

#include <QList>
#include <QObject>

/*!
 * \brief Creates AIS graphics using AIS messages.
 */
class AisGraphicFactory : public QObject
{
    Q_OBJECT
public:
    explicit AisGraphicFactory(const EsriRuntimeQt::SpatialReference &spatialReference, QObject *parent = 0);

    /*!
     * \brief createGraphic Creates a graphic instance using a AIS message.
     * \param aisMessage    The AIS message
     * \return              A new graphic instance.
     */
    EsriRuntimeQt::Graphic createGraphic(AisMessage *aisMessage);

    /*!
     * \brief createGraphicsAsync   Creates graphics asynchronously using the AIS messages.
     * \param aisMessages           The AIS messages
     */
    void createGraphicsAsync(QList<AisMessage*> *aisMessages);

    /*!
     * \brief createGraphics    Creates graphics using the AIS messages.
     * \param aisMessages       The AIS messages
     */
    QList<EsriRuntimeQt::Graphic*> *createGraphics(QList<AisMessage*> *aisMessages);

signals:
    /*!
     * \brief graphicsCreated   Signals that the AIS messages were created.
     * \param aisGraphics       The created AIS graphic instances
     */
    void graphicsCreated(QList<EsriRuntimeQt::Graphic*> *aisGraphics);

public slots:

private:
    static const EsriRuntimeQt::SpatialReference WGS84;
    static const EsriRuntimeQt::SimpleMarkerSymbol DefaultMarkerSymbol;

    enum SymbolSize { DefaultSymbolSize = 5 };

    enum AttributeIndex { MMSIAttributeIndex = 0 };
    static QMap<AttributeIndex, QString> AttributeNames;

    EsriRuntimeQt::SpatialReference _targetSpatialReference;
};

#endif // AISGRAPHICFACTORY_H

#ifndef AISGRAPHICFACTORY_H
#define AISGRAPHICFACTORY_H

#include <AisMessage.h>
#include <EsriProjectionEngine.h>
#include <ProjectionEngine.h>
#include <SimpleProjectionEngine.h>

#include <Graphic.h>
#include <PictureMarkerSymbol.h>
#include <SimpleMarkerSymbol.h>
#include <SpatialReference.h>

#include <QAtomicInt>
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
    ~AisGraphicFactory();

    /*!
     * \brief cancelCreate  Cancel the creation of AIS graphics.
     */
    void cancelCreate();

    /*!
     * \brief createGraphic Creates a graphic instance using a AIS message.
     * \param aisMessage    The AIS message
     * \return              A new graphic instance.
     */
    EsriRuntimeQt::Graphic createGraphic(const AisMessage &aisMessage);

    /*!
     * \brief createGraphicsAsync   Creates graphics asynchronously using the AIS messages.
     * \param aisMessages           The AIS messages
     */
    void createGraphicsAsync(QList<AisMessage> *aisMessages);

    /*!
     * \brief emitGraphics      Creates graphics using the AIS messages.
     * \param aisMessages       The AIS messages
     */
    void emitGraphics(QList<AisMessage> *aisMessages);

signals:
    /*!
     * \brief graphicsCreated   Signals that the AIS messages were created.
     * \param aisGraphics       The created AIS graphic instances
     */
    void graphicsCreated(QList<EsriRuntimeQt::Graphic> *aisGraphics);

public slots:

private:
    static const EsriRuntimeQt::SpatialReference WGS84;
    static const EsriRuntimeQt::SimpleMarkerSymbol DefaultMarkerSymbol;
    static EsriRuntimeQt::PictureMarkerSymbol ShipMarkerSymbol;

    enum SymbolSize { DefaultSymbolSize = 5 };

    enum AttributeIndex { MMSIAttributeIndex = 0, TimestampAttributeIndex = 1 };
    static QMap<AttributeIndex, QString> AttributeNames;

    ProjectionEngine *_projectionEngine;
    EsriRuntimeQt::SpatialReference _targetSpatialReference;
    QAtomicInt _cancel;
};

#endif // AISGRAPHICFACTORY_H

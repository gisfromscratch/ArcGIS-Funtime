#include "AisGraphicFactory.h"

#include <GeometryEngine.h>

#include <QVariant>

#include <QtConcurrent/QtConcurrent>

const EsriRuntimeQt::SpatialReference AisGraphicFactory::WGS84 = EsriRuntimeQt::SpatialReference(4326);
const EsriRuntimeQt::SimpleMarkerSymbol AisGraphicFactory::DefaultMarkerSymbol = EsriRuntimeQt::SimpleMarkerSymbol(Qt::green, AisGraphicFactory::DefaultSymbolSize);
QMap<AisGraphicFactory::AttributeIndex, QString> AisGraphicFactory::AttributeNames;

AisGraphicFactory::AisGraphicFactory(const EsriRuntimeQt::SpatialReference &spatialReference, QObject *parent) :
    QObject(parent),
    _targetSpatialReference(spatialReference)
{
    if (AttributeNames.isEmpty())
    {
        AttributeNames[MMSIAttributeIndex] = "MMSI";
    }
}

EsriRuntimeQt::Graphic AisGraphicFactory::createGraphic(AisMessage *aisMessage)
{
    auto location = aisMessage->location();
    if (!_targetSpatialReference.isWGS84())
    {
        // Project coordinates
        location = EsriRuntimeQt::GeometryEngine::project(location, WGS84, _targetSpatialReference);
    }

    // AIS attributes
    QVariantMap attributes;
    attributes[AttributeNames[MMSIAttributeIndex]] = QVariant(aisMessage->mmsi());

    auto aisGraphic = EsriRuntimeQt::Graphic(location, DefaultMarkerSymbol, attributes);
    return aisGraphic;
}

void AisGraphicFactory::createGraphicsAsync(QList<AisMessage*> *aisMessages)
{
    QtConcurrent::run(this, &AisGraphicFactory::createGraphics, aisMessages);
}

QList<EsriRuntimeQt::Graphic*> *AisGraphicFactory::createGraphics(QList<AisMessage*> *aisMessages)
{
    auto aisGraphics = new QList<EsriRuntimeQt::Graphic*>();
    foreach(AisMessage *aisMessage, *aisMessages)
    {
        auto aisGraphic = new EsriRuntimeQt::Graphic(createGraphic(aisMessage));
        aisGraphics->append(aisGraphic);
    }
    emit graphicsCreated(aisGraphics);
    return aisGraphics;
}

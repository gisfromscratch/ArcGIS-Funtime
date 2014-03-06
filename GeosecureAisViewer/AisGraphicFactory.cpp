#include "AisGraphicFactory.h"

#include <QDebug>
#include <QVariant>

#include <QtConcurrent/QtConcurrent>

const EsriRuntimeQt::SpatialReference AisGraphicFactory::WGS84 = EsriRuntimeQt::SpatialReference(4326);
const EsriRuntimeQt::SimpleMarkerSymbol AisGraphicFactory::DefaultMarkerSymbol = EsriRuntimeQt::SimpleMarkerSymbol(Qt::black, AisGraphicFactory::DefaultSymbolSize);
QMap<AisGraphicFactory::AttributeIndex, QString> AisGraphicFactory::AttributeNames;

AisGraphicFactory::AisGraphicFactory(const EsriRuntimeQt::SpatialReference &spatialReference, QObject *parent) :
    QObject(parent),
    _targetSpatialReference(spatialReference),
    _projectionEngine(new SimpleProjectionEngine)
{
    if (AttributeNames.isEmpty())
    {
        AttributeNames[MMSIAttributeIndex] = "MMSI";
    }
}

AisGraphicFactory::~AisGraphicFactory()
{
    delete _projectionEngine;
}

void AisGraphicFactory::cancelCreate()
{
    _cancel.store(1);
}

EsriRuntimeQt::Graphic AisGraphicFactory::createGraphic(AisMessage *aisMessage)
{
    auto location = aisMessage->location();
    if (!_targetSpatialReference.isWGS84())
    {
        // Project coordinates
        location = _projectionEngine->project(location, WGS84, _targetSpatialReference);
    }

    // AIS attributes
    QVariantMap attributes;
    attributes[AttributeNames[MMSIAttributeIndex]] = QVariant(aisMessage->mmsi());

    auto aisGraphic = EsriRuntimeQt::Graphic(location, DefaultMarkerSymbol, attributes);
    return aisGraphic;
}

void AisGraphicFactory::createGraphicsAsync(QList<AisMessage*> *aisMessages)
{
    _cancel.store(0);
    QtConcurrent::run(this, &AisGraphicFactory::createGraphics, aisMessages);
}

QList<EsriRuntimeQt::Graphic> *AisGraphicFactory::createGraphics(QList<AisMessage*> *aisMessages)
{
    qDebug() << "Trying to create" << aisMessages->size() << "AIS graphics";

    auto aisGraphics = new QList<EsriRuntimeQt::Graphic>();
    aisGraphics->reserve(aisMessages->size());
    foreach(AisMessage *aisMessage, *aisMessages)
    {
        if (_cancel.load())
        {
            return aisGraphics;
        }

        EsriRuntimeQt::Graphic aisGraphic = createGraphic(aisMessage);
        if (!aisGraphic.geometry().isEmpty())
        {
            aisGraphics->append(aisGraphic);
        }
    }

    qDebug() << aisGraphics->size() << "AIS graphics created";
    emit graphicsCreated(aisMessages, aisGraphics);
    return aisGraphics;
}

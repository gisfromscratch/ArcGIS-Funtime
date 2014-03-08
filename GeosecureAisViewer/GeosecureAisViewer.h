// Copyright 2013 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#ifndef GEOSECUREAISVIEWER_H
#define GEOSECUREAISVIEWER_H

namespace EsriRuntimeQt
{
class MapGraphicsView;
}

#include "AisGraphicFactory.h"

#include <AisReader.h>
#include <GraphicSerializer.h>

//Uncommented layer(s) needed
#include <ArcGISLocalTiledLayer.h>
#include <ArcGISTiledMapServiceLayer.h>
//#include <LocalMapService.h>
//#include "ArcGISDynamicMapServiceLayer.h"
//#include "LocalFeatureService.h"
//#include "ArcGISFeatureLayer.h"
#include <GraphicsLayer.h>
#include <Map.h>

#include <QList>
#include <QMainWindow>

class GeosecureAisViewer : public QMainWindow
{
    Q_OBJECT
public:
    GeosecureAisViewer (QWidget *parent = 0);
    ~GeosecureAisViewer ();

private slots:
    void addAisMessage(AisMessage aisMessage);
    void addAisMessages(QList<AisMessage> *aisMessage);
    void addAisGraphics(QList<EsriRuntimeQt::Graphic> *aisGraphics);
    void graphicsExported(QString filePath);
    void mapReady();
//    void localServiceCreationSuccess(const QString& url, const QString& name);
//    void localServiceCreationFailure(const QString& name);
    //  void onFeatureServiceCreationSuccess(const QString& url, const QString& name);
    //  void onFeatureServiceCreationFailure(const QString& name);

private:
    EsriRuntimeQt::Map m_map;
    EsriRuntimeQt::MapGraphicsView* m_mapGraphicsView;
    EsriRuntimeQt::ArcGISLocalTiledLayer m_tiledLayer;
//    EsriRuntimeQt::ArcGISTiledMapServiceLayer m_tiledServiceLayer;
    //  EsriRuntimeQt::ArcGISDynamicMapServiceLayer m_dynamicServiceLayer;
//    EsriRuntimeQt::LocalMapService m_localMapService;
    //  EsriRuntimeQt::ArcGISDynamicMapServiceLayer m_dynamicLocalServiceLayer;
    //  EsriRuntimeQt::LocalFeatureService m_localFeatureService;
    //  EsriRuntimeQt::ArcGISFeatureLayer m_localFeatureLayer;
    //  EsriRuntimeQt::GraphicsLayer m_graphicsLayer;
    //  EsriRuntimeQt::ArcGISFeatureLayer m_featureLayer;

    EsriRuntimeQt::ArcGISTiledMapServiceLayer _basemapLayer;
    EsriRuntimeQt::GraphicsLayer _aisLayer;
    AisReader *_aisReader;
    AisGraphicFactory *_aisGraphicFactory;
    GraphicSerializer *_graphicSerializer;
};

#endif // GEOSECUREAISVIEWER_H


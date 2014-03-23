#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ShapefileWorkspaceFactory.h"

#include <ArcGISDynamicMapServiceLayer.h>
#include <ArcGISLocalTiledLayer.h>
#include <Map.h>
#include <MapGraphicsView.h>
#include <LayerSource.h>

#include <QMainWindow>

using namespace EsriRuntimeQt;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void workspaceAdded(const QString &mapServiceUrl);

private:
    Ui::MainWindow *ui;

    MapGraphicsView *_graphicsView;
    Map _map;
    ArcGISLocalTiledLayer _basemapLayer;

    ShapefileWorkspaceFactory *_workspaceFactory;
    TableDataSource _shapefileDatasource;
    ArcGISDynamicMapServiceLayer _shapefileMapServiceLayer;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GraphicsFileReader.h"

#include <ArcGISLocalTiledLayer.h>
#include <Graphic.h>
#include <GraphicsLayer.h>
#include <Map.h>
#include <MapGraphicsView.h>

#include <QList>
#include <QMainWindow>

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
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void readGraphicsCompleted(QList<EsriRuntimeQt::Graphic> *graphics);

private:
    Ui::MainWindow *ui;

    EsriRuntimeQt::MapGraphicsView *_mapGraphicsView;
    EsriRuntimeQt::Map _map;
    EsriRuntimeQt::ArcGISLocalTiledLayer _basemapLayer;

    GraphicsFileReader *_graphicsFileReader;
};

#endif // MAINWINDOW_H

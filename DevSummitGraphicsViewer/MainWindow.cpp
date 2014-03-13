#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <SimpleMarkerSymbol.h>
#include <SimpleRenderer.h>

#include <QtConcurrent/QtConcurrent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _graphicsFileReader(new GraphicsFileReader)
{
    ui->setupUi(this);

    connect(_graphicsFileReader, SIGNAL(readComplete(QList<EsriRuntimeQt::Graphic>*)), this, SLOT(readGraphicsCompleted(QList<EsriRuntimeQt::Graphic>*)));

    // set to openGL rendering
    EsriRuntimeQt::ArcGISRuntime::setRenderEngine(EsriRuntimeQt::RenderEngine::OpenGL);

    _mapGraphicsView = EsriRuntimeQt::MapGraphicsView::create(_map, this);
    setCentralWidget(_mapGraphicsView);
    _map.setWrapAroundEnabled(false);

    // show Esri logo by default
    _map.setEsriLogoVisible(true);

    // Add the basemap layer
    _basemapLayer = EsriRuntimeQt::ArcGISLocalTiledLayer("Basemap.tpk");
    _map.addLayer(_basemapLayer);

    // Activate drag & drop
    setAcceptDrops(true);
    _mapGraphicsView->setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    disconnect(_graphicsFileReader, SIGNAL(readComplete(QList<EsriRuntimeQt::Graphic>*)), this, SLOT(readGraphicsCompleted(QList<EsriRuntimeQt::Graphic>*)));

    delete ui;

    _map.dispose();
    delete _mapGraphicsView;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    auto urls = event->mimeData()->urls();
    foreach (auto url, urls)
    {
        _graphicsFileReader->readGraphicsAsync(url.toLocalFile());
    }
}

void MainWindow::readGraphicsCompleted(QList<EsriRuntimeQt::Graphic> *graphics)
{
    auto graphicsLayer = EsriRuntimeQt::GraphicsLayer(EsriRuntimeQt::RenderingMode::Dynamic);
    auto renderer = EsriRuntimeQt::SimpleRenderer(EsriRuntimeQt::SimpleMarkerSymbol(Qt::blue, 5, EsriRuntimeQt::SimpleMarkerSymbolStyle::Diamond));
    graphicsLayer.setRenderer(renderer);
    qDebug() << "Adding" << graphics->size() << "graphics";

    _map.addLayer(graphicsLayer);
    graphicsLayer.addGraphics(*graphics);
    delete graphics;
}

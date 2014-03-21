#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "GraphicsCache.h"
#include "TimeFieldInfo.h"

#include <PictureMarkerSymbol.h>
#include <SimpleMarkerSymbol.h>
#include <SimpleRenderer.h>

#include <QtConcurrent/QtConcurrent>
#include <QDebug>

static const int TIMESPAN_INCREMENT_SECONDS = 2;
static const int MAX_TIMESPAN_SECONDS = 60;
static const int UPDATE_INTERVAL_SECONDS = 2;

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

    // Activate selection
    _activateSelectionAction = new QAction(QPixmap(":/toolbar/Icons/edit-select.png"), "Select", this);
    _activateSelectionAction->setCheckable(true);
    ui->mainToolBar->addAction(_activateSelectionAction);

    connect(&_map, SIGNAL(mousePress(QMouseEvent)), this, SLOT(mousePressed(QMouseEvent)));
    connect(&_animationTimer, SIGNAL(timeout()), this, SLOT(updateTimeExtent()));
}

MainWindow::~MainWindow()
{
    disconnect(_graphicsFileReader, SIGNAL(readComplete(QList<EsriRuntimeQt::Graphic>*)), this, SLOT(readGraphicsCompleted(QList<EsriRuntimeQt::Graphic>*)));
    disconnect(&_animationTimer, SIGNAL(timeout()), this, SLOT(updateTimeExtent()));
    _animationTimer.stop();

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

void MainWindow::mousePressed(QMouseEvent evt)
{
    if (_activateSelectionAction->isChecked())
    {
        if (Qt::RightButton == evt.button())
        {
            // Unselect all graphics
            foreach (EsriRuntimeQt::GraphicsLayer *layer, _timeLayers.keys())
            {
                layer->setSelectionIDs(QList<int>(), false);
            }
            return;
        }

        // Select graphics
        auto location = evt.pos();
        const int PixelTolerance = 5;
        foreach (EsriRuntimeQt::GraphicsLayer *layer, _timeLayers.keys())
        {
            auto graphicIds = layer->graphicIDs(location.x(), location.y(), PixelTolerance);
            foreach (auto graphicId, graphicIds)
            {
                if (layer->isGraphicSelected(graphicId))
                {
                    layer->unselect(graphicId);
                }
                else
                {
                    layer->select(graphicId);
                }
            }
        }
    }
}

void MainWindow::updateTimeExtent()
{
    foreach (TimeLayer *layer, _timeLayers.values())
    {
        EsriRuntimeQt::TimeExtent validTimeExtent = layer->timeExtent();
        QDateTime startDate = _currentTimeExtent.startDate().addSecs(TIMESPAN_INCREMENT_SECONDS);
        QDateTime endDate = _currentTimeExtent.endDate().addSecs(TIMESPAN_INCREMENT_SECONDS);
        _currentTimeExtent.setStartDate(startDate);
        _currentTimeExtent.setEndDate(endDate);
        //qDebug() << "Time extent updated" << startDate << "," << endDate;

        // Check time extent
        if (!validTimeExtent.intersects(_currentTimeExtent))
        {
            _animationTimer.stop();

            layer->setTimeInterval(validTimeExtent);
            return;
        }

        layer->setTimeInterval(_currentTimeExtent);
    }
}

void MainWindow::readGraphicsCompleted(QList<EsriRuntimeQt::Graphic> *graphics)
{
    // Create a new graphics layer
    auto graphicsLayer = new EsriRuntimeQt::GraphicsLayer(EsriRuntimeQt::RenderingMode::Dynamic);
//    const int SymbolSize = 5;
//    EsriRuntimeQt::SimpleMarkerSymbol markerSymbol(Qt::blue, SymbolSize, EsriRuntimeQt::SimpleMarkerSymbolStyle::Diamond);
    EsriRuntimeQt::PictureMarkerSymbol pictureSymbol(QPixmap(":/symbol/Icons/ship.png").toImage());
    auto renderer = EsriRuntimeQt::SimpleRenderer(pictureSymbol);
    graphicsLayer->setRenderer(renderer);

    // Add the graphics layer and the graphics
    qDebug() << "Adding" << graphics->size() << "graphics";
    _map.addLayer(*graphicsLayer);
    auto graphicIds = graphicsLayer->addGraphics(*graphics);

    // Create a new graphics cache
    auto graphicsCache = new GraphicsCache(this);
    graphicsCache->insert(graphicIds, *graphics);
    delete graphics;

    // Create a new time layer
    auto timeInfo = new SimpleTimeFieldInfo("TIMESTAMP", "", this);
    auto timeLayer = new TimeLayer(graphicsLayer, timeInfo);

    qDebug() << "Query time extent";
    _currentTimeExtent.setStartDate(timeLayer->timeExtent().startDate());
    _currentTimeExtent.setEndDate(_currentTimeExtent.startDate().addSecs(TIMESPAN_INCREMENT_SECONDS));
    qDebug() << _currentTimeExtent.startDate() << " - " << _currentTimeExtent.endDate();
    _timeLayers.insert(graphicsLayer, timeLayer);

    _animationTimer.start(UPDATE_INTERVAL_SECONDS * 1000);
}

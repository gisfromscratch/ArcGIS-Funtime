#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <DynamicLayerInfo.h>
#include <QFileInfo>
#include <SimpleMarkerSymbol.h>
#include <SimpleLineSymbol.h>
#include <SimpleRenderer.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _workspaceFactory(nullptr)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/mainwindow/Icons/qemu.ico"));

    // Create the map graphics view
    ArcGISRuntime::setRenderEngine(RenderEngine::OpenGL);
    _graphicsView = MapGraphicsView::create(_map, this);
    setCentralWidget(_graphicsView);
    _map.setWrapAroundEnabled(false);
    _map.setEsriLogoVisible(true);

    // Set the basemap layer
    auto applicationPath = QCoreApplication::applicationDirPath();
    auto tilePackageFilePath = applicationPath + QDir::separator() + "Basemap.tpk";
    if (QFileInfo::exists(tilePackageFilePath))
    {
        _basemapLayer = ArcGISLocalTiledLayer(tilePackageFilePath);
        _map.addLayer(_basemapLayer);

        // Create the workspace factory
        _workspaceFactory = new ShapefileWorkspaceFactory(this);
        connect(_workspaceFactory, SIGNAL(workspacesUpdated(QString)), this, SLOT(workspaceAdded(QString)));

        auto shapefilePath = QString("%1%2Shapefiles%2mautknoten_4326.shp").arg(applicationPath).arg(QDir::separator());
        try
        {
            // Open the workspace asynchronously
            _shapefileDatasource = _workspaceFactory->openWorkspaceAsync(QFileInfo(shapefilePath));
        }
        catch (Exception &ex)
        {
            qWarning() << "Shapefile" << shapefilePath << "could not be added!";
            qWarning() << ex.what();
        }
    }
    else
    {
        qWarning() << "Basemap layer" << tilePackageFilePath << "missing!";
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    // Release map resources
    _map.dispose();
    delete _graphicsView;
}

void MainWindow::workspaceAdded(const QString &url)
{
    // Add the shapefile as layer
    _shapefileMapServiceLayer = ArcGISDynamicMapServiceLayer(url);
    _map.addLayer(_shapefileMapServiceLayer);
    auto &dynamicLayers = _shapefileMapServiceLayer.dynamicLayerInfos();
    auto &dynamicLayer = dynamicLayers[0];

    // Update the datasource
    auto layerDatasource = LayerDataSource();
    layerDatasource.setDataSource(_shapefileDatasource);
    dynamicLayer.setLayerSource(layerDatasource);

    // Create a renderer
    const int SymbolSize = 5;
    const int OutlineWith = 1;
    const int Transparency = 0;
    auto outlineSymbol = SimpleLineSymbol(Qt::black, OutlineWith);
    auto markerSymbol = SimpleMarkerSymbol(QColor(255, 85, 0), SymbolSize, SimpleMarkerSymbolStyle::Circle, outlineSymbol);
    auto renderer = SimpleRenderer(markerSymbol);
    dynamicLayer.setDrawingInfo(DrawingInfo(renderer, Transparency));

    // Refresh the layer
    _shapefileMapServiceLayer.refresh();
}

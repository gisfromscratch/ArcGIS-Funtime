#include "MappingToolbar.h"

#include <GraphicsLayer.h>

#include <QDebug>

MappingToolbar::MappingToolbar(EsriRuntimeQt::Map *focusMap, QMainWindow *parent) :
    QToolBar(parent),
    _focusMap(focusMap),
    _mappingGroup(new QActionGroup(this)),
    _navigateAction(new QAction(QPixmap(":/symbols/Icons/toolbars/map.png"), tr("Navigate"), this)),
    _selectGraphicAction(new QAction(QPixmap(":/symbols/Icons/toolbars/edit-select.png"), tr("Select"), this))
{
    // Create and add the actions
    _navigateAction->setCheckable(true);
    _navigateAction->setChecked(true);
    _selectGraphicAction->setCheckable(true);
    _mappingGroup->addAction(_navigateAction);
    _mappingGroup->addAction(_selectGraphicAction);

    addMappingActions();
    parent->addToolBar(this);

    connect(_mappingGroup, SIGNAL(triggered(QAction*)), this, SLOT(updateToolSlots(QAction*)));
}

void MappingToolbar::addMappingActions()
{
    addActions(_mappingGroup->actions());
}

void MappingToolbar::addMappingAction(QAction *action)
{
    _mappingGroup->addAction(action);
    addAction(action);
}

void MappingToolbar::setSelectableLayers(const QList<EsriRuntimeQt::GraphicsLayer> &layers)
{
    _selectableLayers = layers;
}

void MappingToolbar::selectGraphics(QMouseEvent mouseEvent)
{
    const int PixelTolerance = 5;
    const int MaximumGraphicsCount = 10;

    qDebug() << "Selecting graphics";
    QPoint screenLocation = mouseEvent.pos();
    foreach (EsriRuntimeQt::GraphicsLayer layer, _selectableLayers)
    {
        auto graphicIds = layer.graphicIDs(screenLocation.x(), screenLocation.y(), PixelTolerance, MaximumGraphicsCount);
        layer.setSelectionIDs(graphicIds, false);
    }
    qDebug() << "Selection updated";
}

void MappingToolbar::updateToolSlots(QAction *action)
{
    Q_UNUSED(action);

    if (_selectGraphicAction->isChecked())
    {
        // Register for map events
        connect(_focusMap, SIGNAL(mousePress(QMouseEvent)), this, SLOT(selectGraphics(QMouseEvent)));
    }
    else
    {
        // Deregister from map events
        disconnect(_focusMap, SIGNAL(mousePress(QMouseEvent)), this, SLOT(selectGraphics(QMouseEvent)));
    }
}

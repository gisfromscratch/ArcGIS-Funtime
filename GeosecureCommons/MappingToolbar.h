#ifndef MAPPINGTOOLBAR_H
#define MAPPINGTOOLBAR_H

#include "geosecurecommons_global.h"

#include <GraphicsLayer.h>
#include <Map.h>

#include <QAction>
#include <QList>
#include <QMainWindow>
#include <QObject>
#include <QToolBar>

/*!
 * \brief Toolbar using an action group for the mapping tools.
 */
class GEOSECURECOMMONSSHARED_EXPORT MappingToolbar : public QToolBar
{
    Q_OBJECT
public:
    explicit MappingToolbar(EsriRuntimeQt::Map *focusMap, QMainWindow *parent = 0);

    /*!
     * \brief addMappingAction  Adds a new mapping action to the action group.
     * \param action            The action to add
     */
    void addMappingAction(QAction *action);

    /*!
     * \brief setSelectableLayers   Sets the selectable layers.
     * \param layers                List of selectable layers
     */
    void setSelectableLayers(const QList<EsriRuntimeQt::GraphicsLayer> &layers);

signals:

private slots:
    /*!
     * \brief selectGraphics    Selects the graphics of all graphics layer add the mouse position.
     * \param mouseEvent        The mouse event
     */
    void selectGraphics(QMouseEvent mouseEvent);

    /*!
     * \brief updateToolSlots   Updates the connection of the tool based slots.
     * \param action            The action which was triggered
     */
    void updateToolSlots(QAction *action);

private:
    void addMappingActions();

    QActionGroup *_mappingGroup;
    QAction *_navigateAction;
    QAction *_selectGraphicAction;

    EsriRuntimeQt::Map *_focusMap;
    QList<EsriRuntimeQt::GraphicsLayer> _selectableLayers;
};

#endif // MAPPINGTOOLBAR_H

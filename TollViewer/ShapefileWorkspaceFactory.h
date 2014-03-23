#ifndef SHAPEFILEWORKSPACEFACTORY_H
#define SHAPEFILEWORKSPACEFACTORY_H

#include <DynamicLayerInfo.h>
#include <LocalMapService.h>
#include <WorkspaceInfo.h>

#include <QFileInfo>
#include <QObject>
#include <QMap>

using namespace EsriRuntimeQt;

/*!
 * \brief Offers access to shapefiles by using the Local Server.
 */
class ShapefileWorkspaceFactory : public QObject
{
    Q_OBJECT
public:
    explicit ShapefileWorkspaceFactory(QObject *parent = 0);
    ~ShapefileWorkspaceFactory();

    /*!
     * \brief openWorkspaceAsync    Opens the workspace where the shapefile exists as a local datasource.
     *                              After the local server is updated, the workspacesUpdated() signal is emitted.
     *                              You have to wait for this signal after opening the workspace.
     *                              Throws EsriRuntimeQt::Exception if the workspace is not valid!
     * \param shapeFile             The shapefile *.shp.
     * \return                      If the shapefile can be read a valid datasource is returned.
     */
    TableDataSource openWorkspaceAsync(const QFileInfo &shapeFile);

signals:
    /*!
     * \brief workspacesUpdated Signals that the local server was updated.
     * \param mapServiceUrl     The url to the local map service.
     */
    void workspacesUpdated(const QString &mapServiceUrl);

private slots:
    void localServerStarted(const QString &, const QString &);
    void localServerFailed(const QString &errorMessage);

private:
    LocalMapService _localMapService;
    bool _basemapLayerIsValid;

    QMap<QString, QString> _openWorkspaces;
};

#endif // SHAPEFILEWORKSPACEFACTORY_H

#ifndef GRAPHICSCACHE_H
#define GRAPHICSCACHE_H

#include <Graphic.h>

#include <QList>
#include <QMap>
#include <QObject>

/*!
 * \brief A simple cache mapping graphics IDs to graphic instances.
 */
class GraphicsCache : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsCache(QObject *parent = 0);

    /*!
     * \brief insert        Inserts a graphic into this cache using a graphic ID as key.
     *                      If the key is already associated with a graphic, the graphic is replaced.
     * \param graphicId     The key for the graphic.
     * \param graphic       The graphic instance.
     */
    void insert(int graphicId, const EsriRuntimeQt::Graphic &graphic);

    /*!
     * \brief insert        Inserts graphics into this cache using graphic IDs as keys.
     * \param graphicIds    The keys for the graphics.
     * \param graphics      The graphic instances.
     */
    void insert(const QList<int> graphicIds, const QList<EsriRuntimeQt::Graphic> &graphics);

    /*!
     * \brief value         Returns the graphic associated with the graphic ID.
     * \param graphicId     The key for the graphic.
     * \return              The associated graphic.
     *                      If there is no associated graphic, a graphic with an empty geometry is returned.
     */
    EsriRuntimeQt::Graphic value(int graphicId);

signals:

public slots:

private:
    QMap<int, EsriRuntimeQt::Graphic> _graphics;

    EsriRuntimeQt::Graphic _defaultGraphic;
};

#endif // GRAPHICSCACHE_H

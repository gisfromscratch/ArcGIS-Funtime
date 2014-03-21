#ifndef GRAPHICSFILEREADER_H
#define GRAPHICSFILEREADER_H

#include <Graphic.h>

#include <QAtomicInt>
#include <QList>
#include <QObject>

/*!
 * \brief Simple reader for files containing the JSON representation of graphics linewise.
 */
class GraphicsFileReader : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsFileReader(QObject *parent = 0);

    void cancel();

    /*!
     * \brief readGraphicsAsync Reads the specified file asynchronously.
     * \param filePath          The path to the file.
     */
    void readGraphicsAsync(const QString &filePath);

signals:
    /*!
     * \brief readComplete  Signals the read complete event.
     * \param graphics      The graphics read from the file.
     */
    void readComplete(QList<EsriRuntimeQt::Graphic> *graphics);

public slots:

private:
    void readGraphics(const QString &filePath);

    QAtomicInt _cancel;
};

#endif // GRAPHICSFILEREADER_H

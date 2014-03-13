#ifndef GRAPHICSFILEREADER_H
#define GRAPHICSFILEREADER_H

#include <Graphic.h>

#include <QAtomicInt>
#include <QList>
#include <QObject>

class GraphicsFileReader : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsFileReader(QObject *parent = 0);

    void cancel();

    void readGraphicsAsync(const QString &filePath);

signals:
    void readComplete(QList<EsriRuntimeQt::Graphic> *graphics);

public slots:

private:
    void readGraphics(const QString &filePath);

    QAtomicInt _cancel;
};

#endif // GRAPHICSFILEREADER_H

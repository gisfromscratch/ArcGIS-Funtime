#include "GraphicsFileReader.h"

#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <QFile>
#include <QTextStream>

GraphicsFileReader::GraphicsFileReader(QObject *parent) :
    QObject(parent)
{
}

void GraphicsFileReader::cancel()
{
    _cancel.store(1);
}

void GraphicsFileReader::readGraphicsAsync(const QString &filePath)
{
    QtConcurrent::run(this, &GraphicsFileReader::readGraphics, filePath);
}

void GraphicsFileReader::readGraphics(const QString &filePath)
{
    QFile inputFile(filePath);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        qWarning() << filePath << "open failed";
        return;
    }
    qDebug() << "Reading" << filePath;

    auto *graphics = new QList<EsriRuntimeQt::Graphic>();
    QTextStream textStream(&inputFile);
    while (!textStream.atEnd() && !_cancel.load())
    {
        auto graphicAsJson = textStream.readLine();
        auto graphic = EsriRuntimeQt::Graphic();
        graphic.fromJson(graphicAsJson);
        graphics->append(graphic);
    }

    inputFile.close();

    if (_cancel.load())
    {
        return;
    }
    emit readComplete(graphics);
}

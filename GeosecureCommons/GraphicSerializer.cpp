#include "GraphicSerializer.h"

#include <QFile>
#include <QTextStream>

#include <QtConcurrent/QtConcurrent>

GraphicSerializer::GraphicSerializer(QObject *parent) :
    QObject(parent)
{
}

void GraphicSerializer::cancelSerialize()
{
    _cancel.store(1);
}

bool GraphicSerializer::serializeGraphicsAsync(QList<EsriRuntimeQt::Graphic> *graphics, const QString &filePath)
{
    QFile jsonFile(filePath);
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    jsonFile.close();

    _cancel.store(0);
    QtConcurrent::run(this, &GraphicSerializer::serializeGraphics, graphics, filePath);
    return true;
}

void GraphicSerializer::serializeGraphics(QList<EsriRuntimeQt::Graphic> *graphics, const QString &filePath)
{
    QFile jsonFile(filePath);
    if (jsonFile.open(QIODevice::WriteOnly))
    {
        QTextStream textStream(&jsonFile);
        foreach (EsriRuntimeQt::Graphic graphic, *graphics)
        {
            if (_cancel.load())
            {
                break;
            }
            textStream << graphic.toJson() << endl;
        }
        jsonFile.close();
    }

    emit graphicsSerialized(filePath);
    delete graphics;
}

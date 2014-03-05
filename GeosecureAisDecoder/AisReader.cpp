#include "AisReader.h"

#include <QFile>
#include <QTextStream>

#include <QtConcurrent/QtConcurrent>

AisReader::AisReader(QObject *parent) :
    QObject(parent),
    _decoder(new GeosecureAisDecoder)
{
}

AisReader::~AisReader()
{
    _cancel.store(1);
    _decoder->deleteLater();
}

bool AisReader::readFileAsync(const QString &filePath)
{
    QFile aisFile(filePath);
    if (!aisFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    aisFile.close();

    // Call asynchronously
    QtConcurrent::run(this, &AisReader::readFile, filePath);
    return true;
}

void AisReader::readFile(const QString &filePath)
{
    QFile aisFile(filePath);
    if (aisFile.open(QIODevice::ReadOnly))
    {
        QTextStream aisStream(&aisFile);
        while (!aisStream.atEnd())
        {
            QString aisLine = aisStream.readLine();
            QStringList aisTokens = aisLine.split(",");
            switch (aisTokens.length())
            {
                case AisNmeaTokenCount:
                {
                    QString aisNmeaToken = aisTokens.at(AisNmeaTokenIndex);
                    AisMessage *aisMessage;
                    try
                    {
                        if (_cancel.load())
                        {
                            // Destructor of AisFileReader was called!
                            return;
                        }
                        aisMessage = _decoder->createAisMessage(aisNmeaToken.toStdString().c_str());
                        emit aisMessageVisited(aisMessage);
                    }
                    catch (std::logic_error &err)
                    {
                        qWarning() << err.what();
                    }
                    break;
                }
            }
        }
        aisFile.close();
    }
}

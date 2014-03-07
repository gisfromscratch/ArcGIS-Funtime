#include "AisReader.h"

#include <QFile>
#include <QTextStream>

#include <QtConcurrent/QtConcurrent>

AisReader::AisReader(QObject *parent) :
    QObject(parent),
    _decoder(new GeosecureAisDecoder),
    AisNmeaTokenSeparator(",")
{
}

AisReader::~AisReader()
{
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
    _cancel.store(0);
    QtConcurrent::run(this, &AisReader::readFile, filePath);
    return true;
}

void AisReader::cancelRead()
{
    // Set the cancel flag
    _cancel.store(1);
}

void AisReader::readFile(const QString &filePath)
{
    QFile aisFile(filePath);
    if (aisFile.open(QIODevice::ReadOnly))
    {
        auto messages = new QList<AisMessage>();
        QTextStream aisStream(&aisFile);
        while (!aisStream.atEnd() && !_cancel.load())
        {
            QString aisLine = aisStream.readLine();
            QStringList aisTokens = aisLine.split(AisNmeaTokenSeparator);
            switch (aisTokens.length())
            {
                case AisNmeaTokenCount:
                {
                    QString aisNmeaToken = aisTokens.at(AisNmeaTokenIndex);
                    try
                    {
                        if (_cancel.load())
                        {
                            // Cancel read was called!
                            break;
                        }
                        AisMessage aisMessage = _decoder->createAisMessage(aisNmeaToken.toStdString().c_str());
                        messages->append(aisMessage);
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
        emit aisMessagesVisited(messages);
    }
}

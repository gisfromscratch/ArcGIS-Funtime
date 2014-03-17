#ifndef TIMEFIELDINFO_H
#define TIMEFIELDINFO_H

#include <QObject>

class TimeFieldInfo : public QObject
{
    Q_OBJECT
public:
    explicit TimeFieldInfo(QObject *parent = 0);
    virtual ~TimeFieldInfo();

    virtual QString startTimeField() = 0;
    virtual QString endTimeField() = 0;
};

class SimpleTimeFieldInfo : public TimeFieldInfo {
    Q_OBJECT
public:
    SimpleTimeFieldInfo(const QString &startTimeField, const QString &endTimeField, QObject *parent = 0) :
        TimeFieldInfo(parent),
        _startTimeField(startTimeField),
        _endTimeField(endTimeField)
    {
    }

    virtual QString startTimeField() override
    {
        return _startTimeField;
    }

    virtual QString endTimeField() override
    {
        return _endTimeField;
    }

private:
    QString _startTimeField;
    QString _endTimeField;
};

#endif // TIMEFIELDINFO_H

#ifndef GEOSECUREGEODATABASE_H
#define GEOSECUREGEODATABASE_H

#include "geosecuregeodatabase_global.h"

#include <FileGDBAPI.h>
#include <FileGDBCore.h>

#include <QDebug>

class GEOSECUREGEODATABASESHARED_EXPORT GeosecureGeodatabase
{

public:
    GeosecureGeodatabase();

    static bool Try(const fgdbError &error)
    {
        using namespace std;

        if (S_OK == error)
        {
            return true;
        }
        else
        {
            wstring errorMessage;
            FileGDBAPI::ErrorInfo::GetErrorDescription(error, errorMessage);
            qWarning() << QString::fromStdWString(errorMessage);
            return false;
        }
    }
};

#endif // GEOSECUREGEODATABASE_H

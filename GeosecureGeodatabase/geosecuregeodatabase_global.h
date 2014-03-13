#ifndef GEOSECUREGEODATABASE_GLOBAL_H
#define GEOSECUREGEODATABASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GEOSECUREGEODATABASE_LIBRARY)
#  define GEOSECUREGEODATABASESHARED_EXPORT Q_DECL_EXPORT
#else
#  define GEOSECUREGEODATABASESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GEOSECUREGEODATABASE_GLOBAL_H

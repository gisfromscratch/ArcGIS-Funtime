#ifndef GEOSECUREAISDECODER_GLOBAL_H
#define GEOSECUREAISDECODER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GEOSECUREAISDECODER_LIBRARY)
#  define GEOSECUREAISDECODERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GEOSECUREAISDECODERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GEOSECUREAISDECODER_GLOBAL_H

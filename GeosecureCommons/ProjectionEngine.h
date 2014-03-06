#ifndef PROJECTIONENGINE_H
#define PROJECTIONENGINE_H

#include "geosecurecommons_global.h"

#include <Geometry.h>
#include <SpatialReference.h>

using namespace EsriRuntimeQt;

class GEOSECURECOMMONSSHARED_EXPORT ProjectionEngine
{
public:
    ProjectionEngine();
    virtual ~ProjectionEngine();

    virtual Geometry project(const Geometry &geometry, const SpatialReference &sourceSpatialReference, const SpatialReference &targetSpatialReference) = 0;
};

#endif // PROJECTIONENGINE_H

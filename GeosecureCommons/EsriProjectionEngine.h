#ifndef ESRIPROJECTIONENGINE_H
#define ESRIPROJECTIONENGINE_H

#include "geosecurecommons_global.h"

#include "ProjectionEngine.h"

class GEOSECURECOMMONSSHARED_EXPORT EsriProjectionEngine : public ProjectionEngine
{
public:
    EsriProjectionEngine();

    virtual Geometry project(const Geometry &geometry, const SpatialReference &sourceSpatialReference, const SpatialReference &targetSpatialReference) override;
};

#endif // ESRIPROJECTIONENGINE_H

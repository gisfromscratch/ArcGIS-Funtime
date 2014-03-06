#ifndef SIMPLEPROJECTIONENGINE_H
#define SIMPLEPROJECTIONENGINE_H

#include "geosecurecommons_global.h"

#include "ProjectionEngine.h"

class GEOSECURECOMMONSSHARED_EXPORT SimpleProjectionEngine : public ProjectionEngine
{
public:
    SimpleProjectionEngine();

    virtual Geometry project(const Geometry &geometry, const SpatialReference &sourceSpatialReference, const SpatialReference &targetSpatialReference) override;
};

#endif // SIMPLEPROJECTIONENGINE_H

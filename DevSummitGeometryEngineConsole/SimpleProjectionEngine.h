#ifndef SIMPLEPROJECTIONENGINE_H
#define SIMPLEPROJECTIONENGINE_H

#include "ProjectionEngine.h"

class SimpleProjectionEngine : public ProjectionEngine
{
public:
    SimpleProjectionEngine();

    virtual Geometry project(const Geometry &geometry, const SpatialReference &sourceSpatialReference, const SpatialReference &targetSpatialReference) override;
};

#endif // SIMPLEPROJECTIONENGINE_H

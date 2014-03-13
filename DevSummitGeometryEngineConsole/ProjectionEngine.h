#ifndef PROJECTIONENGINE_H
#define PROJECTIONENGINE_H

#include <Geometry.h>
#include <SpatialReference.h>

using namespace EsriRuntimeQt;

class ProjectionEngine
{
public:
    ProjectionEngine();
    virtual ~ProjectionEngine();

    virtual Geometry project(const Geometry &geometry, const SpatialReference &sourceSpatialReference, const SpatialReference &targetSpatialReference) = 0;
};

#endif // PROJECTIONENGINE_H

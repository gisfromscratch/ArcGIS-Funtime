#include "EsriProjectionEngine.h"

#include <GeometryEngine.h>

EsriProjectionEngine::EsriProjectionEngine()
{
}

Geometry EsriProjectionEngine::project(const Geometry &geometry, const SpatialReference &sourceSpatialReference, const SpatialReference &targetSpatialReference)
{
    return GeometryEngine::project(geometry, sourceSpatialReference, targetSpatialReference);
}

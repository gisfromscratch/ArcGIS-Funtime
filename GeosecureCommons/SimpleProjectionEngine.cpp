#include "SimpleProjectionEngine.h"

#include <GeometryEngine.h>

#include <QDebug>

#include <math.h>

SimpleProjectionEngine::SimpleProjectionEngine()
{
}

Geometry SimpleProjectionEngine::project(const Geometry &geometry, const SpatialReference &sourceSpatialReference, const SpatialReference &targetSpatialReference)
{
    if (Geometry::isPoint(geometry.type()))
    {
        if (sourceSpatialReference.isWGS84() && targetSpatialReference.isAnyWebMercator())
        {
            const Point *geometryAsPoint = reinterpret_cast<const Point*>(&geometry);
            double mercatorX_lon = geometryAsPoint->x();
            double mercatorY_lat = geometryAsPoint->y();
            if (180 < fabs(mercatorX_lon) || 90 < fabs(mercatorY_lat))
            {
//                qWarning() << mercatorX_lon << "," << mercatorY_lat << "not valid for WGS84";
                return GeometryEngine::project(geometry, sourceSpatialReference, targetSpatialReference);
            }

            double num = mercatorX_lon * 0.017453292519943295;
            double x = 6378137.0 * num;
            double a = mercatorY_lat * 0.017453292519943295;

            mercatorX_lon = x;
            mercatorY_lat = 3189068.5 * log((1.0 + sin(a)) / (1.0 - sin(a)));
            return EsriRuntimeQt::Point(mercatorX_lon, mercatorY_lat);
        }
        else if (sourceSpatialReference.isAnyWebMercator() && targetSpatialReference.isWGS84())
        {
            const Point *geometryAsPoint = reinterpret_cast<const Point*>(&geometry);
            double mercatorX_lon = geometryAsPoint->x();
            double mercatorY_lat = geometryAsPoint->y();
            if ((20037508.3427892 < fabs(mercatorX_lon)) || 20037508.3427892 < (fabs(mercatorY_lat)))
            {
//                qWarning() << mercatorX_lon << "," << mercatorY_lat << "not valid for Web Mercator";
                return GeometryEngine::project(geometry, sourceSpatialReference, targetSpatialReference);
            }

            double x = mercatorX_lon;
            double y = mercatorY_lat;
            double num3 = x / 6378137.0;
            double num4 = num3 * 57.295779513082323;
            double num5 = floor((double)((num4 + 180.0) / 360.0));
            double num6 = num4 - (num5 * 360.0);
            double num7 = 1.5707963267948966 - (2.0 * atan(exp((-1.0 * y) / 6378137.0)));
            mercatorX_lon = num6;
            mercatorY_lat = num7 * 57.295779513082323;
            return EsriRuntimeQt::Point(mercatorX_lon, mercatorY_lat);
        }
    }
    return GeometryEngine::project(geometry, sourceSpatialReference, targetSpatialReference);
}

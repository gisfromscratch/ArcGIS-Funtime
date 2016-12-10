using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Runtime.Powershell.Modules
{
    /// <summary>
    /// Geometry having a spatial reference
    /// </summary>
    public interface IEsriGeometry
    {
        EsriSpatialReference SpatialReference { get; set; }
    }
}

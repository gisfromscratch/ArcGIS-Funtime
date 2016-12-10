using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Runtime.Powershell.Modules
{
    /// <summary>
    /// Two dimensional point.
    /// </summary>
    public class EsriPoint : IEsriGeometry
    {
        public double X { get; set; }

        public double Y { get; set; }

        public EsriSpatialReference SpatialReference { get; set; }
    }
}

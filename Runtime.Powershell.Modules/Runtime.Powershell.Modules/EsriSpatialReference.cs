using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Runtime.Powershell.Modules
{
    /// <summary>
    /// Spatial reference for any type of geometries.
    /// </summary>
    public class EsriSpatialReference
    {
        public int Wkid { get; set; }
    }
}

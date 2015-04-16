using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Runtime.Powershell.Modules
{
    /// <summary>
    /// In-Memory layer containing a bunch of features.
    /// </summary>
    interface IEsriInMemoryLayer
    {
        EsriSpatialReference SpatialReference { get; }

        EsriFeature CreateFeature();

        void InsertFeature(EsriFeature feature);

        void DeleteFeature(EsriFeature feature);

        void DeleteFeature(long objectId);

        EsriFeature GetFeature(long objectId);
    }
}

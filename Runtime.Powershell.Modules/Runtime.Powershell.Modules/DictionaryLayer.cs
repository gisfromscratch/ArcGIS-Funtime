using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Runtime.Powershell.Modules
{
    /// <summary>
    /// In-Memory layer using a dictionary for all of its features.
    /// </summary>
    public class DictionaryLayer : IEsriInMemoryLayer
    {
        private readonly EsriSpatialReference _spatialReference = new EsriSpatialReference { Wkid = 4326 };

        private readonly IDictionary<long, EsriFeature> _features = new Dictionary<long, EsriFeature>();

        private long _nextId = 0;

        public EsriSpatialReference SpatialReference
        {
            get
            {
                return _spatialReference;
            }
        }

        public EsriFeature CreateFeature()
        {
            return new EsriFeature { ObjectId = _nextId++, Shape = new EsriPoint { SpatialReference = _spatialReference }, Attributes = new Dictionary<string, object>() };
        }

        public void InsertFeature(EsriFeature feature)
        {
            _features.Add(feature.ObjectId, feature);
        }

        public void DeleteFeature(EsriFeature feature)
        {
            _features.Remove(feature.ObjectId);
        }

        public void DeleteFeature(long objectId)
        {
            _features.Remove(objectId);
        }

        public EsriFeature GetFeature(long objectId)
        {
            return _features[objectId];
        }
    }
}

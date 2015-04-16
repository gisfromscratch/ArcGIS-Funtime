using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Runtime.Powershell.Modules
{
    /// <summary>
    /// Feature having a geometry and a list of attributes.
    /// </summary>
    public class EsriFeature
    {
        public long ObjectId { get; set; }

        public IEsriGeometry Shape { get; set; }

        public IDictionary<string, object> Attributes { get; set; }
    }
}

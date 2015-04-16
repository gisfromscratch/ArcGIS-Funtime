using System;
using System.Collections.Generic;
using System.Linq;
using System.Management.Automation;
using System.Text;
using System.Threading.Tasks;

namespace Runtime.Powershell.Modules
{
    /// <summary>
    /// Creates in-memory layer.
    /// </summary>
    [Cmdlet(VerbsCommon.New, @"InMemoryLayer")]
    public class NewInMemoryLayerCmdLet : PSCmdlet
    {
        /// <summary>
        /// The type of the memory cache which should be used.
        /// </summary>
        [Parameter(Mandatory = true, Position = 0, ValueFromPipelineByPropertyName = true, ValueFromPipeline = true, HelpMessage = @"The type (hash, in) of the memory cache which should be used.")]
        public string MemoryCacheType { get; set; }

        /// <summary>
        /// The number of features.
        /// </summary>
        [Parameter(Mandatory = true, Position = 1, ValueFromPipelineByPropertyName = true, ValueFromPipeline = true, HelpMessage = @"The number of features.")]
        public int NumberOfFeatures { get; set; }

        protected override void BeginProcessing()
        {
            base.BeginProcessing();
        }

        protected override void ProcessRecord()
        {
            WriteDebug(string.Format(@"Creating a new in-memory layer using '{0}' cache having '{1}' features.", MemoryCacheType, NumberOfFeatures));
            var random = new Random();
            var layer = new DictionaryLayer();
            const int chunkSize = 50000;
            var progressRecord = new ProgressRecord(1, @"New-InMemoryLayer", string.Format(@"Creating a new in-memory layer using '{0}' cache having '{1}' features.", MemoryCacheType, NumberOfFeatures));
            for (var index = 0; index < NumberOfFeatures; index++)
            {
                var feature = layer.CreateFeature();
                feature.Shape = new EsriPoint { X = random.Next(-180, 180), Y = random.Next(-90, 90), SpatialReference = layer.SpatialReference };
                feature.Attributes.Add(@"CreationDate", DateTime.Now);
                layer.InsertFeature(feature);

                if (0 == index % chunkSize)
                {
                    WriteDebug(string.Format(@"'{0}' features inserted.", index));
                    progressRecord.PercentComplete = NumberOfFeatures / (index + 1);
                    WriteProgress(progressRecord);
                }
            }
            WriteObject(@"All features inserted.");
        }

        protected override void EndProcessing()
        {
            base.EndProcessing();
        }
    }
}

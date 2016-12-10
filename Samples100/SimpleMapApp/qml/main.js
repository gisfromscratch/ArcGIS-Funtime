.import Esri.ArcGISRuntime 100.0 as ArcRun
.import "PortalConnection.js" as Portal


function connect(url) {
    var connection = new Portal.PortalConnection();
    connection.connect(url);
    return connection;
}

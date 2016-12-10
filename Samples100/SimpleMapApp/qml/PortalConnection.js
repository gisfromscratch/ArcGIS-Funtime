.import Esri.ArcGISRuntime 100.0 as ArcRun

function PortalConnection() {
    this.loadStatus = "";
    this.portal = null;
}

PortalConnection.prototype._checkLoadStatus = function() {
    if (ArcRun.Enums.LoadStatusLoaded !== this.portal.loadStatus) {
       setTimeout(this._checkLoadStatus, 1000);
    } else {
        return;
    }
}

PortalConnection.prototype.connect = function(url) {
    var portal = ArcRun.ArcGISRuntimeEnvironment.createObject("Portal");
    console.log(portal.objectType);
    portal.url = url;
    portal.loadStatusChanged.connect(this.loadStatusChanged);
    portal.load();
    console.log(portal.loadStatus);
    this.portal = portal;
    this._checkLoadStatus();
}

PortalConnection.prototype.loadStatusChanged = function(loadStatus) {
    console.log("Load status changed.");
}

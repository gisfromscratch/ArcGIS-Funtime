
// Copyright 2016 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

import QtQuick 2.6
import QtQuick.Controls 1.4
import Esri.ArcGISRuntime 100.0

ApplicationWindow {
    id: appWindow
    width: 800
    height: 600
    title: "SimpleMapApp"

    // add a mapView component
    MapView {
        anchors.fill: parent
        wrapAroundMode: Enums.WrapAroundModeEnabledWhenSupported

        // add a map to the mapview
        Map {
            // add the BasemapTopographic basemap to the map
            BasemapTopographic {}
        }
    }
}

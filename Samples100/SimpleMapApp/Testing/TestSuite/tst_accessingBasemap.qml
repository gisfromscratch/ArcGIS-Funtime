import QtQuick 2.6
import QtTest 1.0
import Esri.ArcGISRuntime 100.0
import "../../qml/main.js" as Main

TestCase {
    name: "ArcGIS Online tests"

    function test_connection() {
        var connection = Main.connect("http://www.arcgis.com");
        compare(null !== connection, true, "No connection!");
    }
}

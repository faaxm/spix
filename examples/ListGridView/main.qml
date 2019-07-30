import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

Window {
    objectName: "mainWindow"
    visible: true
    width: 640
    height: 480
    title: qsTr("Spix Example")
    color: "#111111"

    ListView {
        objectName: "myListView"
        anchors {
            top: parent.top
            left: parent.left
            right: parent.horizontalCenter
            bottom: parent.verticalCenter
        }

        model: ["Hello", "world", "With", "More", "Buttons"]
        delegate: Button {
            objectName: "listItem_" + index
            text: "ListView Item: " + modelData
            width: 200
            onClicked: resultsView.appendText("List Button " + (index+1) + " clicked")
        }
    }

    GridView {
        objectName: "myGridView"
        anchors {
            top: parent.top
            left: parent.horizontalCenter
            right: parent.right
            bottom: parent.verticalCenter
        }

        model: ["Hello", "world", "With", "More", "Buttons"]
        delegate: Button {
            objectName: "gridItem_" + index
            text: "" + modelData
            onClicked: resultsView.appendText("Grid Button " + (index+1) + " clicked")
        }
    }

    ResultsView {
        id: resultsView
        anchors {
            top: parent.verticalCenter
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
}

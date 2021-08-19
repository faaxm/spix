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

    RowLayout {

        anchors {
            top: parent.top
            left: parent.left
            topMargin: 5
            leftMargin: 5
        }

        Button {
            objectName: "Button_1"
            text: "Press Me"
            onClicked: resultsView.appendText("Button 1 clicked")
        }
        Button {
            objectName: "Button_2"
            text: "Or Click Me"
            onClicked: resultsView.appendText("Button 2 clicked")
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

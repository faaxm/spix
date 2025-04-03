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

        
    GridLayout {
        objectName: "Grid_1"
        rowSpacing: 20
        columnSpacing: 20
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.verticalCenter
        }
        columns: 2

        Button {
            objectName: "Button_1"
            text: "Press Me"
            Layout.fillHeight: true
            Layout.fillWidth: true
            MouseArea {
                anchors.fill: parent
                acceptedButtons:  Qt.AllButtons
                
                onClicked:
                {
                        resultsView.appendText("Button 1 clicked")
                }
            }
        }
        Button {
            objectName: "Button_2"
            text: "Press Me"
            Layout.fillHeight: true
            Layout.fillWidth: true
            MouseArea {
                anchors.fill: parent
                acceptedButtons:  Qt.AllButtons
                
                onClicked:
                {
                        resultsView.appendText("Button 2 clicked")
                }
            }
        }
        Button {
            objectName: "Button_3"
            text: "Press Me"
            Layout.fillHeight: true
            Layout.fillWidth: true
            MouseArea {
                anchors.fill: parent
                acceptedButtons:  Qt.AllButtons
                
                onClicked:
                {
                        resultsView.appendText("Button 3 clicked")
                }
            }
        }
        Button {
            objectName: "Button_4"
            text: "Press Me"
            Layout.fillHeight: true
            Layout.fillWidth: true
            MouseArea {
                anchors.fill: parent
                acceptedButtons:  Qt.AllButtons
                
                onClicked:
                {
                        resultsView.appendText("Button 4 clicked")
                }
            }
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

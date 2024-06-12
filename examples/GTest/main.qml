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
            text: "Press Me"
            MouseArea {
                objectName: "Button_1"
                anchors.fill: parent
                acceptedButtons:  Qt.AllButtons

                onClicked:
                {
                    if(mouse.button & Qt.RightButton)
                        resultsView.appendText("Button 1 right clicked")
                    else
                        resultsView.appendText("Button 1 clicked")
                }
            }
        }
        Button {
            text: "Or Click Me"
            MouseArea {
                objectName: "Button_2"
                anchors.fill: parent
                acceptedButtons:  Qt.AllButtons
				
                onClicked:
                {
                    if(mouse.button & Qt.RightButton)
                        resultsView.appendText("Button 2 right clicked")
                    else
                        resultsView.appendText("Button 2 clicked")
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

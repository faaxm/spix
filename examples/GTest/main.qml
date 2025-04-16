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
            MouseArea {
                anchors.fill: parent
                acceptedButtons:  Qt.AllButtons

                onClicked: function (mouse) {
                    if(mouse.button & Qt.RightButton)
                        resultsView.appendText("Button 1 right clicked")
                    else if ((mouse.button == Qt.LeftButton) && (mouse.modifiers & Qt.ShiftModifier))
                        resultsView.appendText("Button 1 shift clicked")
                    else if ((mouse.button == Qt.LeftButton) && (mouse.modifiers & Qt.ControlModifier))
                        resultsView.appendText("Button 1 shift control clicked")
                    else
                        resultsView.appendText("Button 1 clicked")
                }
            }
        }
        Rectangle {
            width: 100
            height: 100
            color: "red"
            Button {
                objectName: "Button_2"
                text: "Or Click Me"
                width: 99
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons:  Qt.AllButtons
                    
                    onClicked: function (mouse) {
                        if(mouse.button & Qt.RightButton)
                            resultsView.appendText("Button 2 right clicked")
                        else if ((mouse.modifiers & Qt.ControlModifier) && (mouse.modifiers &Qt.ShiftModifier))
                            resultsView.appendText("Button 2 shift control clicked")
                        else if ((mouse.modifiers & Qt.ShiftModifier))
                            resultsView.appendText("Button 2 shift clicked")
                        else if ((mouse.button === Qt.LeftButton) && (mouse.modifiers & Qt.ControlModifier))
                            resultsView.appendText("Button 2 control clicked")
                        else
                            resultsView.appendText("Button 2 clicked")
                    }
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

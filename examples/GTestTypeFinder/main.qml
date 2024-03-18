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

        Rectangle {
            property string test_id: "red_rect"
            width: 200
            height: 200
            color: "red"

            Text {
                property string test_id: "button 1"
                objectName: "Button_1"
                text: "Click Me"
                anchors.right: parent.right
                anchors {
                    bottom: parent.bottom
                }
                    
                MouseArea {
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

            Text {
                property string test_id: "button_3"
                objectName: "Button_3"
                text: "Click You"
                anchors.left: parent.left
                anchors {
                    bottom: parent.bottom
                }
                
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons:  Qt.AllButtons
                    onClicked:
                        {
                            if(mouse.button & Qt.RightButton)
                                resultsView.appendText("Button 3 right clicked")
                            else
                                resultsView.appendText("Button 3 clicked")
                        }
                    }
            }

            Rectangle {
                property string test_id: "green_rect"
                width: 150
                height: 150
                color: "green"
                
                Rectangle {
                    property string test_id: "blue_rect"
                    width: 100
                    height: 100
                    color: "blue"
                    Text {
                        property string test_id: "button 2"
                        objectName: "Button_2"
                        text: "Click Me"
                        MouseArea {
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

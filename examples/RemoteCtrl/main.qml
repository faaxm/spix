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

    ColumnLayout {
        id: columnLayout

        RowLayout {
            id: rowLayout

            Layout.preferredHeight: 200

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

            Rectangle {
                id: rectangle
                color: "lightgray"

                Layout.preferredWidth: 200
                Layout.preferredHeight: 100

                MouseArea {
                    id: mouseArea_1
                    anchors.fill: parent

                    onClicked: resultsView.appendText("MouseArea 1 clicked")
                }

                RowLayout {
                    Button {
                        objectName: "Button_3"
                        text: "Press Me"
                        onClicked: resultsView.appendText("Button 3 clicked")
                    }
                    Button {
                        objectName: "Button_4"
                        text: "Or Click Me"
                        onClicked: resultsView.appendText("Button 4 clicked")
                    }
                }

                MouseArea {
                    id: mouseArea_2
                    anchors.fill: parent

                    onClicked: resultsView.appendText("MouseArea 2 clicked")
                }
            }
        }

        ResultsView {
            id: resultsView
        }
    }
}

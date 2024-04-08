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

        ColumnLayout {
            Repeater {
                objectName: "ItemButtons"
                model: ["tomato", "pear", "banana", "cucumber"]
                Rectangle {
                    objectName: "Item_" + index

                    color: "#AAAAFF"
                    width: 150
                    height: 30
                    border {
                        width: 2
                        color: "#5555FF"
                    }

                    Text {
                        anchors.centerIn: parent
                        text: "Show a " + modelData
                    }

                    MouseArea
                    {
                        id: mouseArea
                        anchors.fill: parent
                        onClicked: itemDisplayLoader.source = "show_" + modelData + ".qml"
                    }
                }
            }
        }

        Rectangle {
            color: "black"
            width: 130
            height: 130
            border {
                width: 2
                color: "red"
            }
            Loader {
                id: itemDisplayLoader
                objectName: "ItemDisplayLoader"

                anchors.centerIn: parent
            }
        }
        
        ComboBox {
            objectName: "itemCombo"
            width: 200
            model: ["tomato", "pear", "banana", "cucumber"]
            
            onActivated: itemDisplayLoader.source = "show_" + textAt(index) + ".qml"
        }
    }
}

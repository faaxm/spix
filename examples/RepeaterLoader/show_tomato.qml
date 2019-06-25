import QtQuick 2.11
import QtQuick.Controls 2.4

Rectangle {
    color: "#FF2222"
    width: 100
    height: 100

    Text {
        objectName: "textItem"
        anchors.fill: parent
        anchors.margins: 3
        wrapMode: Text.WordWrap
        font.bold: true
        text: "I am a view with a tomato. Trust me."
    }
}

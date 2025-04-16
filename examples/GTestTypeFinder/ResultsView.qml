import QtQuick 2.11
import QtQuick.Controls 2.4

Rectangle {
    id: resultsView
    color: "#222222"

    function appendText(text) {
        resultsArea.append(text);
    }

    Text {
        id: resultsTitle
        text: "Result: "
        color: "white"
        font.bold: true

        anchors {
            top: resultsView.top
            left: resultsView.left
            right: resultsView.right

            topMargin: 5
            leftMargin: 2
            bottomMargin: 2
        }
    }
    TextEdit {
        id: resultsArea
        objectName: "results"
        color: "white"
        anchors {
            top: resultsTitle.bottom
            left: resultsView.left
            right: resultsView.right
            bottom: resultsView.bottom

            leftMargin: 2
            rightMargin: 2
            bottomMargin: 2
        }
    }
}

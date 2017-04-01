import QtQuick 2.0

Rectangle {
    id: root
    signal clicked

    property alias image_src: image.source

    width: 40
    height: 40
    radius: 3
    smooth: true
    border.width: 2
    color: buttonColor
    border.color: borderColor

    Image {
        id: image
        anchors.fill:parent
        fillMode: Image.PreserveAspectFit
    }

    // Highlighting and ativation section
    property color buttonColor: "black"
    property color onHoverbuttonColor: "black"
    property color onHoverColor: "lightGrey"
    property color borderColor: "black"
    property color onSelectedColor: "#AA777777"

    MouseArea{
        id: mousearea
        anchors.fill: parent
        onClicked: {
            root.clicked()
        }
        hoverEnabled: true
        onEntered: {
            parent.border.color = onHoverColor
        }
        onExited: {
            parent.border.color = borderColor
        }
    }
}


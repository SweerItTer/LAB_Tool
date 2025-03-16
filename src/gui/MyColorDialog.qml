import QtQuick 2.12
import QtQml 2.12
import QtQuick.Window 2.12
import gui.controllers 1.0

Window {
    id: colorDialog
    visible: true
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint // 无边框窗口，保持在最顶层
    color: "transparent" // 透明
    property var rootWindow // 接收来自Loader的rootWindow

    width: Screen.width
    height: Screen.height

    signal closed()
    onClosing: closed()

    Image {
        id: screenshotImage
        anchors.fill: parent
        source: "image://screenshotprovider/screenshot"
    }

    DataProcess {
        id: dataProcessor
        onRgbDataProcessed: (l, a, b) => {
            rootWindow.currentLAB.lL = l;
            rootWindow.currentLAB.lA = a;
            rootWindow.currentLAB.lB = b;
            console.log("LAB: ", l, a, b);

            close();
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: containsMouse ? Qt.ArrowCursor : Qt.CrossCursor 
        onClicked: (mouse) => {
            var imageWidth = screenshotImage.sourceSize.width;
            var imageHeight = screenshotImage.sourceSize.height;
            var physicalX = mouse.x * (imageWidth / width);
            var physicalY = mouse.y * (imageHeight / height);
            var color = screenshotProvider.getPixelColor(physicalX, physicalY);
            dataProcessor.processRGB(color) // 调用C++处理数据
        }
    }
}

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
// import QtMultimedia 5.12

import gui.controllers 1.0

ApplicationWindow {
    id: root
    width: 680
    height: 420
    visible: true
    minimumWidth: 600
    minimumHeight: 400
    title: "LAB Color Threshold Configurator"

    // 选择摄像头
    Popup {
        id: cameraPopup
        width: 300
        height: 400
        modal: true
        focus: true
        closePolicy: Popup.NoAutoClose

        property var cList: []  // 存储摄像头名称
        
        Column {
            anchors.fill: parent
            spacing: 10

            Text {
                text: "Select Camera"
                font.bold: true
                font.pixelSize: 16
                anchors.horizontalCenter: parent.horizontalCenter
            }

            ListView {
                id: cameraListView
                width: parent.width
                height: parent.height - 40
                model: cameraPopup.cList

                delegate: ItemDelegate {
                    width: parent.width
                    text: modelData
                    onClicked: {
                        cameraProcess.startProcess(index)  // 发送信号
                        cameraPopup.close()  // 关闭弹窗
                    }
                }
            }
        }
        
        Component.onCompleted: console.log("QML Received:", cList);
        
    }

    // 错误提示
    Popup {
        id: errorPopup
        width: 300
        height: 150
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape

        Column {
            anchors.centerIn: parent
            spacing: 10
            Label {
                text: "Can't find camera."
                wrapMode: Text.Wrap
            }
            Button {
                text: "OK"
                onClicked: errorPopup.close()
            }
        }
    }

    CameraProcess {
        id: cameraProcess
        onCheckWarning: function(cameraList, i) {
            if (i === 1) {
                cameraPopup.cList = cameraList;  // 设置摄像头列表
                cameraPopup.open();
            } else {
                cameraProcess.startProcess(0);
            }
        }
        onCheckError: {
            errorPopup.open();
            console.log("camera error");
        }
    }

    property QtObject currentLAB: QtObject {
        property real lL: 0
        property real lA: 0
        property real lB: 0

        // 监听属性变化，自动触发滑条更新
        onLLChanged: updateSliders(lL, lA, lB)
        onLAChanged: updateSliders(lL, lA, lB)
        onLBChanged: updateSliders(lL, lA, lB)
    }

    // 滑条更新函数
    function updateSliders(l, a, b) {
        // 1. 定义通道索引映射
        let channels = ["L", "A", "B"];
        let values = [l, a, b];

        // 2. 遍历每个通道
        channels.forEach((channel, index) => {
            // 3. 获取对应的 Repeater 项
            let column = labChannelRepeater.itemAt(index);
            if (!column) return;

            // 4. 计算滑条范围
            let value = values[index];
            let min = Math.max(0, value - 10);
            let max = Math.min(255, value + 10);

            // 5. 动态调整范围宽度（确保至少20）
            if (max - min < 20) {
                if (min === 0) max = 20;
                else if (max === 255) min = 235;
                else {
                    min = value - 10;
                    max = value + 10;
                }
            }

            // 6. 更新滑条值
            column.children[1].value = min; // minSlider
            column.children[2].value = max; // maxSlider
        });
    }

    // 信号定义
    signal sliderChanged(string channel, real minValue, real maxValue)
    signal saveRequested()
    signal videoClicked(real x, real y)

    ColumnLayout {
        anchors.fill: parent
        spacing: 15
        anchors.margins: 20

        // 视频预览区域 40%
        RowLayout {
            Layout.preferredHeight: parent.height * 0.4
            Layout.fillWidth: true
            spacing: 15

            // VideoOutput {
            //     id:processVideo
            //     Layout.minimumHeight: 300
            //     Layout.fillWidth: true
            //     Layout.fillHeight: true
            //     // 处理后的视频预览
            // }

            // VideoOutput {
            //     id:rawVideo
            //     Layout.minimumHeight: 300
            //     Layout.fillWidth: true
            //     Layout.fillHeight: true
            // }
            Image {
                id:rawVideo
                Layout.minimumHeight: 300
                Layout.fillWidth: true
                Layout.fillHeight: true
                source: "image://frameprovider_raw/frame"
                // 处理后的视频预览
            }
        }
        

        Loader {
            id: loader
            // anchors.fill: parent
            onLoaded: {
                // 窗口关闭时清空 Loader
                item.closed.connect(() => loader.source = "");
            }
        }

        // 颜色选择控制区
        RowLayout {
            Layout.preferredHeight: parent.height * 0.1
            Layout.fillWidth: true
            
            Button {
                id : selcolor
                text: "pick up color"
                onClicked: {
                    screenshotProvider.captureScreenshot();
                    loader.setSource("MyColorDialog.qml", { rootWindow: root });
                    // loader.source = "MyColorDialog.qml"; // 直接加载 QML 文件
                }
            }
            
            // 在已有Loader下方添加LAB显示绑定
            Label {
                text: `Now LAB:  L${currentLAB.lL.toFixed(1)} A${currentLAB.lA.toFixed(1)} B${currentLAB.lB.toFixed(1)}`
                font.bold: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.minimumWidth: 20
            }
            // 颜色配置下拉框
            ComboBox {
                id: colorSelector
                Layout.preferredWidth: 240  // 固定推荐宽度
                Layout.minimumWidth: 180    // 最小宽度限制
                Layout.maximumWidth: 300   // 最大宽度限制
                Layout.preferredHeight: 40 // 固定高度
                model: ["Red", "Green", "Blue", "Black", "White"]
                currentIndex: 0

                // 比例控制样式
                font.pixelSize: 14

                // 精简样式
                palette {
                    text: "#000000"       // 浅灰文字
                    base: "#616161"      // 高级灰背景
                    highlight: "#616161" // 选中项背景
                }

                contentItem: Text {
                    text: "config: " + colorSelector.currentText
                    anchors.centerIn: parent
                    color: parent.palette.text
                    verticalAlignment: Text.AlignVCenter
                }
                // 下拉箭头样式
                indicator: Canvas {
                    x: parent.width - width - 8
                    y: (parent.height - height) / 2
                    width: 12
                    height: 8
                    contextType: "2d"

                    onPaint: {
                        context.reset();
                        context.moveTo(0, 0);
                        context.lineTo(width, 0);
                        context.lineTo(width/2, height);
                        context.closePath();
                        context.fillStyle = parent.palette.buttonText;
                        context.fill();
                    }
                }


                onActivated: colorConfigSelected(model[index])
            }
        }

        // LAB控制面板
        GridLayout {
            id: labControlGrid

            Layout.preferredHeight: parent.height * 0.5
            Layout.fillWidth: true
            columns: 2

            Repeater {
                id: labChannelRepeater
                model: ["L", "A", "B"]

                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    // 选项
                    Label {
                        text: modelData + ":"
                        font.bold: true
                    }
                    // 前一个滑条
                    Slider {
                        id: minSlider
                        from: 0
                        to: 255
                        value: 0
                        Layout.fillWidth: true
                        onValueChanged: sliderChanged(modelData + "_min", value, maxSlider.value)
                    }

                    // 后一个滑条
                    Slider {
                        id: maxSlider
                        from: 0
                        to: 255
                        value: 255
                        Layout.fillWidth: true
                        onValueChanged: sliderChanged(modelData + "_max", minSlider.value, value)
                    }
                }
            }
        }

        // 配置保存区
        Button {
            text: "save config"
            onClicked: saveRequested()
        }
    }

    
    Component.onCompleted: {
        cameraProcess.check();
    }
    
}

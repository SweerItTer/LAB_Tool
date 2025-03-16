
## 三、关键功能实现调整

### 1. 摄像头管理类（简化版）
```cpp:D:\Program\project\RedRobot\LAB_Tool\src\core\CameraManager.cpp
class CameraManager : public QObject {
  Q_OBJECT
public:
  Q_INVOKABLE void startCapture() {
    if(!cap.isOpened()) cap.open(0);
    
    // 简化帧传递逻辑
    QTimer::singleShot(33, this, [this]{
      cv::Mat frame;
      cap >> frame;
      QImage img(frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
      emit frameUpdated(img.rgbSwapped());
    });
  }

  Q_INVOKABLE void exportConfig(const QString& path) {
    // 调用ConfigExporter
    ConfigExporter::exportToFile(currentConfig, path);
  }
signals:
  void frameUpdated(const QImage &image);
};
```

### 2. QML界面关键组件
```qml:D:\Program\project\RedRobot\LAB_Tool\src\gui\main.qml
// 新增配置导出对话框
Popup {
  id: exportDialog
  FileDialog {
    id: fileDialog
    onAccepted: CameraManager.exportConfig(fileDialog.selectedFile)
  }

  Button {
    text: "选择保存路径"
    onClicked: fileDialog.open()
  }
}

// ROI选择区域增强
VideoOutput {
  ColorPicker {
    onRegionSelected: (x,y,w,h) => {
      CameraManager.setROI(x,y,w,h)
      labValues.update(x,y,w,h)
    }
  }
}
```

## 四、开发步骤建议

### 第一阶段：基础框架搭建（2天）
1. 实现摄像头画面到QML的基本显示
2. 完成颜色选取的鼠标交互逻辑
3. 建立简单的配置导出框架

### 第二阶段：核心功能实现（3天）
1. 完善ROI选择与LAB值实时显示
2. 实现配置文件的YAML/JSON导出
3. 添加摄像头参数调节界面

### 第三阶段：稳定性优化（2天）
1. 异常处理（摄像头断开、文件权限等）
2. 添加配置导入功能
3. 实现多摄像头切换支持

## 五、关键交互流程
```mermaid
sequenceDiagram
  QML界面->>C++核心: 启动摄像头
  C++核心->>OpenCV: 初始化VideoCapture
  OpenCV-->>C++核心: 返回视频帧
  C++核心->>QML界面: 发送RGB图像(QImage)
  QML界面->>用户: 显示实时画面
  用户->>QML界面: 选择ROI区域
  QML界面->>C++核心: 发送ROI坐标
  C++核心->>OpenCV: 计算LAB值
  OpenCV-->>QML界面: 返回LAB数据
  用户->>QML界面: 点击导出配置
  QML界面->>C++核心: 调用exportConfig
  C++核心->>文件系统: 生成YAML/JSON文件
```
import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtWebEngine 1.0
import QtWebView 1.1

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ToolBar {
        id: toolBar
        width: parent.width
        TabBar {
            id: tabBar
            //anchors.top: toolBar.bottom
            width: parent.width -addTabBtn.width
            TabButton {
                text: "0"
                onClicked: webView.url = "https://www.google.ru/#newwindow=1&q=" + text
                onDoubleClicked: tabBar.removeItem(tabBar.currentIndex);
            }
            TabButton {
                text: "1"
                onClicked: webView.url = "https://www.google.ru/#newwindow=1&q=" + text
                onDoubleClicked: tabBar.removeItem(tabBar.currentIndex);
            }
        }
    }

    Component {
        id: tabButton
        TabButton {
            text: "0"
            onClicked: webView.url = "https://www.google.ru/#newwindow=1&q=" + text
            onDoubleClicked: tabBar.removeItem(tabBar.currentIndex);
        }
    }

    WebEngineView {
        id: webView
        url: "http://google.ru"
        visible: true
        anchors.top: toolBar.bottom
        width: parent.width
        height: parent.height - toolBar.height
    }


    ToolBar {
        id: statusBar
        anchors.bottom: parent.bottom;
        width: parent.width
        Button {
            id: addTabBtn
            anchors.left: parent.left
            width: 50
            text: "+"
            onClicked: {
                var obj = tabButton.createObject(tabBar)
                obj.text = progressBar2.value
                progressBar2.value += 1
            }
        }
        ProgressBar {
            id: progressBar
            anchors.fill: parent
            from: 0
            to: 100
            value: webView.loadProgress
        }
    }

    ProgressBar {
        id: progressBar2
        visible: false
        from: 0
        to: 100
        value: 4
    }

}

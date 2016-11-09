import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
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
        anchors.top: parent.top
        width: parent.width

        TabBar {
            id: tabBar
            anchors.fill: parent
            property var tabCount: 1;

            TabButton {
                text: "1"
                onClicked: webView.url = "https://www.google.ru/#newwindow=1&q=" + text
                onDoubleClicked: {
                    tabBar.removeItem(tabBar.currentIndex);
                    tabBar.tabCount -= 1
                }
            }
        }
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
                var tab = tabButton.createObject(tabBar)
                tab.text = tabBar.tabCount + 1
                tabBar.tabCount += 1
            }
        }

        Button {
            id: showLeftPanelBtn
            anchors.left: addTabBtn.right
            width: 50
            text: "+"
            onClicked: {
                leftPanel.visible = true
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

    WebEngineView {
        id: webView
        url: "http://google.ru"
        visible: true
        anchors.top: toolBar.bottom
        anchors.bottom: statusBar.top
        width: parent.width
    }

    Component {
        id: tabButton

        TabButton {
            text: "0"
            onClicked: webView.url = "https://www.google.ru/#newwindow=1&q=" + text
            onDoubleClicked: {
                tabBar.removeItem(tabBar.currentIndex);
                tabBar.tabCount -= 1
            }
        }
    }

}



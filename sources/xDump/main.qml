import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtWebEngine 1.3


ApplicationWindow {
    id: root
    visible: true
    width: 1000
    height: 600
    title: "Hello World"

    menuBar: MenuBar {
        Menu {
            title: "App"
            MenuItem {
                text: "Exit"
                onTriggered: {
                    root.close()
                }
            }
        }

        Menu {
            title: "Actions"
            MenuItem {
                text: "Add tab"
                onTriggered: {
                    var tab = tabComponent.createObject(tabBar)
                    tab.title = "tab_" + (tabBar.count - 1)
                    tabBar.currentIndex = tabBar.count - 1
                }
            }
            MenuItem {
                text: "Delete tab"
                onTriggered: {
                    if (tabBar.count > 0) {
                        tabBar.removeTab(tabBar.currentIndex)
                        var index = tabBar.currentIndex
                        tabBar.currentIndex = 0
                        tabBar.currentIndex = index
                    }
                }
            }
            MenuItem {
                text: "Toggle left panel"
                onTriggered: {
                    leftPanel.visible = !leftPanel.visible
                }
            }
        }
    }

    /*************************************************************************************************/
    /*Top*/
    /*************************************************************************************************/

    ToolBar {
        id: toolBar
        height: 25

        TabView {
            id: tabBar
            anchors.fill: parent
            frameVisible: false
            tabsVisible: true
            height: tab0.implicitHeight


            onCurrentIndexChanged: {
                if (count > 0) {
                    var tab = getTab(currentIndex)
                    webView.url = "https://www.google.ru/#q=" + getTab(currentIndex).title
                } else {
                    webView.url = "https://www.google.ru/#q=null"
                }
            }

            Tab {
                id: tab0
                title: "Main"
            }

        }
    }

    /*************************************************************************************************/
    /*Center*/
    /*************************************************************************************************/

    SplitView {
        anchors.top: toolBar.bottom
        anchors.bottom: statusBar.top
        width: parent.width

        Rectangle {
            id: leftPanel
            visible: false
            width: 250
            Layout.maximumWidth: 250
            Layout.minimumWidth: 250
            color: "lightgrey"
            }

        WebEngineView {
            id: webView
            url: "https://www.google.ru/#q=" + parent.width / 2
            visible: true
            Layout.fillWidth: true
        }
    }

    /*************************************************************************************************/
    /*Bottom*/
    /*************************************************************************************************/

    StatusBar {
        id: statusBar
        anchors.bottom: parent.bottom;
        width: parent.width
        height: 25

        Button {
            id: addTabBtn
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.height
            height: parent.height
            text: " + "

            onClicked: {
                var tab = tabComponent.createObject(tabBar)
                tab.title = "tab_" + (tabBar.count - 1)
                tabBar.currentIndex = tabBar.count - 1
            }
        }

        /*temp buttons begin*/

        Button {
            id: delTabBtn
            anchors.top: parent.top
            anchors.left: addTabBtn.right
            width: parent.height
            height: parent.height
            text: " - "

            onClicked: {
                if (tabBar.count > 0) {
                    tabBar.removeTab(tabBar.currentIndex)
                    var index = tabBar.currentIndex
                    tabBar.currentIndex = 0
                    tabBar.currentIndex = index
                }
            }
        }

        Button {
            id: toggleLeftPanelBtn
            anchors.top: parent.top
            anchors.left: delTabBtn.right
            width: parent.height
            height: parent.height
            text: "LP"

            onClicked: {
                leftPanel.visible = !leftPanel.visible
            }
        }

        /*Temp buttons end*/

        ProgressBar {
            id: progressBar
            anchors.top: parent.top
            anchors.left: toggleLeftPanelBtn.right
            width: parent.width - addTabBtn.width - delTabBtn.width - toggleLeftPanelBtn.width
            height: parent.height
            minimumValue: 0
            maximumValue: 100
            value: webView.loadProgress
        }
    }


    /*************************************************************************************************/
    /*Components*/
    /*************************************************************************************************/

    Component {
        id: tabComponent
        Tab {
         }
    }

}



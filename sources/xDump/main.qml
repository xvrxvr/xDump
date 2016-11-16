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
                shortcut: "Ctrl+T"
                onTriggered: {
                    var tab = tabComponent.createObject(tabBar)
                    tab.title = "tab_" + (tabBar.count - 1)
                    tabBar.currentIndex = tabBar.count - 1
                    webView.url = "https://www.google.ru/#q=" + getTab(tabBar.currentIndex).title
                }
            }
            MenuItem {
                text: "Delete tab"
                shortcut: "Ctrl+W"
                onTriggered:
                    if (tabBar.count > 0) {
                        tabBar.removeTab(tabBar.currentIndex)
                        var index = tabBar.currentIndex
                        tabBar.currentIndex = 0
                        tabBar.currentIndex = index
                }
            }
            MenuItem {
                text: "Toggle search panel"
                shortcut: "Ctrl+F"
                onTriggered: {
                    searchPanel.visible = !searchPanel.visible
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
            id: searchPanel
            visible: false
            width: 250
            Layout.maximumWidth: 250
            Layout.minimumWidth: 250
            color: "lightgrey"
            }

        WebEngineView {
            id: webView
            url: "https://www.google.ru/#q=Main"
            visible: (tabBar.count != 0)
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

        ProgressBar {
            id: progressBar
            anchors.fill: parent
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



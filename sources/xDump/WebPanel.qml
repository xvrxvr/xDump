import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtWebEngine 1.0

Item {
    id: root
    ToolBar {
        id: toolBar
        height: 25

        TabView {
            id: tabBar
            anchors.fill: parent
            frameVisible: false
            tabsVisible: true

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
                title: "Main1"
            }
            Tab {
                id: tab1
                title: "Main2"
            }
        }
    }

    WebEngineView {
        id: webView
        anchors.bottom: parent.bottom
        anchors.top: toolBar.bottom
        anchors.left: parent.left
        width: parent.width
        url: "https://www.google.ru/#q=Main"
        visible: true
    }

    function openUrl(url) {
        webView.url = url;
    }

    function addTab() {
        var tab = tabComponent.createObject(tabBar)
        tab.title = "tab_" + (tabBar.count - 1)
        tabBar.currentIndex = tabBar.count - 1
        webView.url = "https://www.google.ru/#q=" + tabBar.getTab(tabBar.currentIndex).title
    }

    function deleteTab() {
        if (tabBar.count > 0) {
            tabBar.removeTab(tabBar.currentIndex)
            var index = tabBar.currentIndex
            tabBar.currentIndex = 0
            tabBar.currentIndex = index
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

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
                    if (tab.is_saved) {
                        webView.loadHtml(getFromBuffer(tab.bufferKey))
                    } else {
                        webView.url = "_" + getTab(currentIndex).title
                    }
                } else {
                    webView.url = "_Empty"
                }
            }
        }
    }

    WebEngineView {
        id: webView
        anchors.bottom: parent.bottom
        anchors.top: toolBar.bottom
        anchors.left: parent.left
        width: parent.width
        url: ""
        visible: true
    }

    function openUrl(url) {
        webView.url = url;
    }

    function addTab(name) {
        var tab = tabComponent.createObject(tabBar)
        tabBar.currentIndex = tabBar.count - 1
        if (name) {
            tab.title = name
        } else {
            tab.title = "Empty"
        }
    }

    function deleteTab() {
        if (tabBar.count > 0) {
            tabBar.removeTab(tabBar.currentIndex)
            var index = tabBar.currentIndex
            tabBar.currentIndex = 0
            tabBar.currentIndex = index
        }
    }

    function nextTab() {
        if (tabBar.currentIndex < (tabBar.count - 1)) {
            tabBar.currentIndex += 1
        }
    }

    function previousTab() {
        if (tabBar.currentIndex > 0) {
            tabBar.currentIndex -= 1
        }
    }

    function showHtml(html) {
        webView.loadHtml(html)
    }

    function getProgress() {
        return webView.loadProgress
    }

    function saveData(key, html) {
        var tab = tabBar.getTab(tabBar.currentIndex)
        tab.bufferKey = key
        addToBuffer(key, html)
        tab.is_saved = true
    }


    /*************************************************************************************************/
    /*Components*/
    /*************************************************************************************************/

    Component {
        id: tabComponent
        Tab {
            property var bufferKey: " "
            property var is_saved: false
        }
    }
}

import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtWebEngine 1.3
import QtQuick.Dialogs 1.2


ApplicationWindow {
    id: root
    visible: true
    width: 1000
    height: 600
    title: "Hello World"


    /*************************************************************************************************/
    /*Menu*/
    /*************************************************************************************************/

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
                    webPanel.addTab()
                }
            }

            MenuItem {
                text: "Next tab"
                shortcut: "Ctrl+Right"
                onTriggered: {
                    webPanel.nextTab()
                }
            }

            MenuItem {
                text: "Previous tab"
                shortcut: "Ctrl+Left"
                onTriggered: {
                    webPanel.previousTab()
                }
            }

            MenuItem {
                text: "Delete tab"
                shortcut: "Ctrl+W"
                onTriggered: {
                    webPanel.deleteTab()
                }
            }

            MenuItem {
                text: "Toggle search panel"
                shortcut: "Ctrl+F"
                onTriggered: {
                    searchPanel.visible = !searchPanel.visible
                }
            }


            MenuItem {
                text: "Open file"
                shortcut: "Ctrl+O"
                onTriggered: {
                    fileDialogComponent.createObject(root)
                }
            }

            MenuItem {
                text: "Split window"
                shortcut: "Ctrl+Z"
                onTriggered: {
                    var comp = webPanelComponent.createObject()
                    center.addItem(comp)
                }
            }
        }
    }


    /*************************************************************************************************/
    /*Center*/
    /*************************************************************************************************/

    SplitView {
        id: center
        anchors.top: parent.top//toolBar.bottom
        anchors.bottom: statusBar.top
        width: parent.width
        orientation: Qt.Horizontal

        GroupBox {
            id: searchPanel
            title: "Search panel"
            visible: false
            width: 250
            Layout.maximumWidth: 250
            Layout.minimumWidth: 250

            TextField {
                id: searchField
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 5
            }

            ListView {
                id: listView
                anchors.top: searchField.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 5
                highlight: Rectangle { color: "lightsteelblue"; radius: 2 }
                focus: true
                model: SearchModel {}
                delegate: Text {
                    text: name + ": " + number
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        listView.currentIndex = index
                    }
                }
            }
        }


        WebPanel {
            height: centerLeft.height
            visible: true
            id: webPanel
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
        id: webPanelComponent
        WebPanel {
            Layout.minimumWidth: 250
        }
    }


    Component {
        id: webPanelComponent2
        Rectangle {
            Layout.maximumWidth: 200
            width: 200
            color: "blue"
        }
    }


    Component {
        id: fileDialogComponent
        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            folder: "."
            onAccepted: {
                console.log("You chose: " + fileDialog.fileUrls)
            }
            onRejected: {
                console.log("Canceled")
            }
            Component.onCompleted: visible = true
        }
    }

}

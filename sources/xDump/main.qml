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
                    webPanel.openUrl("ya.ru")
                }
            }


            MenuItem {
                text: "Open file"
                shortcut: "Ctrl+O"
                onTriggered: {
                    fileDialogComponent.createObject()
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

        Rectangle {
            id: searchPanel
            visible: false
            width: 250
            Layout.maximumWidth: 250
            Layout.minimumWidth: 250
            color: "lightgrey"

            TextEdit {
                visible: true
                id: searchField
                anchors.fill: parent
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

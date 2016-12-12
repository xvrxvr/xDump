import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtWebEngine 1.3
import QtQuick.Dialogs 1.2
//import "jsdriver.js" as JSDriver

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
                text: "Create dynamic window"
                shortcut: "Ctrl+Z"
                onTriggered: {
                    var qmltext = "
                        import QtQuick 2.7
                        import QtQuick.Layouts 1.1
                        import QtQuick.Controls 1.4
                        import QtQuick.Controls.Styles 1.4
                        import QtWebEngine 1.3
                        import QtQuick.Dialogs 1.2

                        Dialog {
                            visible: true
                            standardButtons: StandardButton.Ok
                            ColumnLayout{
                                CheckBox {
                                    id: 'cbox_x'
                                    text: '-x'
                                    checked: true
                                }
                                CheckBox {
                                    id: 'cbox_D'
                                    text: '-D'
                                    checked: false
                                }
                            }
                            onAccepted: {
                                fileDialogComponent.createObject(root)
                            }
                        }
                    "
                    var component = Qt.createQmlObject(qmltext, root)

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
            height: parent.height
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
            height: center.height
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
            value: webPanel.getProgress()
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
            title: "Please choose a file to dump"
            folder: "."
            onAccepted: {
                console.log("File to dump: " + fileDialog.fileUrl)
                var path = decodeURIComponent(wrapFileUrl(fileDialog.fileUrl))
                dumpFile(path)
            }
            onRejected: {
                console.log("Canceled")
            }
            Component.onCompleted: visible = true
        }
    }

    function dumpFile(path) {
        env.addGlobObject('INP_FILE', path);
        env.loadConfig()
        for(var configName in viewConfigSet) {
            var sectionName = configName;
            if (viewConfigSet[sectionName].autoload) {
                dumpSection(path, sectionName);
            }
        }
    }

    function dumpSection(path, sectionName) {
        webPanel.addTab(sectionName);

        var lineStrm = executer.exec(sectionName);

        var vt = viewConfigSet[sectionName].getViewTranslator(lineStrm);

        var toLoad = vt.getHeader() +
                     vt.getBody() +
                     vt.getFooter();

        webPanel.showHtml(toLoad);
        webPanel.saveData(path + ":" + sectionName, toLoad);
    }

}

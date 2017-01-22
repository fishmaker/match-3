import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

import com.apps.gametable 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 800; height: 800

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "New game"
                shortcut: "Ctrl+N"
            }
        }
    }

    statusBar: StatusBar {
        RowLayout {
            anchors.fill: parent
            Label { text: game_table.points }
        }
    }

    Rectangle {
        id: root_wraper
        anchors.fill: parent
        color: "white"

        GridView {
            id: grid_view
            x: 60; y: -690
            width: 665; height: 1400
            anchors.margins: 10
            cellHeight: 74; cellWidth: 74
            interactive: false

            model: game_table
            delegate: game_delegate

            remove: { destroy_transition }
            move: { move_transition }
            displaced: { move_transition }

            flow: GridView.FlowTopToBottom
            layoutDirection: Qt.LeftToRight
            verticalLayoutDirection: GridView.BottomToTop

            Transition {
                id: move_transition
                NumberAnimation { easing.amplitude: 0.2; properties: "x,y"; duration: 800; easing.type: "Linear" }
            }

            Transition {
                id: destroy_transition
                NumberAnimation { property: "opacity"; to: 0; duration: 250 }
            }
        }

        GameTable {
            id: game_table
        }

        Component {
            id: game_delegate

            Rectangle {
                width: grid_view.cellWidth; height: grid_view.cellHeight

                color: "Transparent"

                Image {
                    width: grid_view.cellWidth - 10; height: grid_view.cellHeight - 10
                    anchors.centerIn: parent
                    source: r_image
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (!move_transition.running) {
                            game_table.handleMouse(index)
                        }
                    }
                }
            }
        }

        Rectangle {
            width: parent.width; height: 48
            color: "white"
        }

    }
}


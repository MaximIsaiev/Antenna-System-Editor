import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import DfPost.Controls.Stc 1.0
import DfPost.CoreQml 1.0
import DfPost.AntSystem 1.0
import QtQml 2.11

Frame {

    // модель таблицы
    property var bottomBoxModel

    // локальные свойства
    QtObject {
        id: settings
        // сколько знаков после запятой оставить
        property int roundCount: 2
    }

    id: leftFrame
    contentItem: ColumnLayout {

        Label {
            text: qsTr("Координаты антенных элементов: ")
        }

        TableView {
            id: tableView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: bottomBoxModel

            TableViewColumn {
                title: "№"
                role: "rowIndex"
                width: parent.width / 30


                delegate: Label {
                    text: styleData.value
                }

            }

            TableViewColumn {
                id: firstColumn
                title: "X"
                role: "x"
                width: parent.width / 3.1

                delegate: FloatEdit {
                    id: textInputX
                    value: styleData.value
                    round: settings.roundCount

                    onEditingFinished: {
                        var index = tableView.model.index(styleData.row, 0)
                        tableView.model.setData(index, value, AntSystemElementsModel.XRole)
                        textInputX.focus = false;
                    }

                }
            }

            TableViewColumn {
                title: "Y"
                role: "y"
                width: parent.width / 3.1

                delegate: FloatEdit {
                    id: textInputY
                    value: styleData.value
                    round: settings.roundCount

                    onEditingFinished: {
                        var index = tableView.model.index(styleData.row, 0)
                        tableView.model.setData(index, value, AntSystemElementsModel.YRole)
                        textInputY.focus = false;
                    }
                }
            }

            TableViewColumn {
                title: "Z"
                role: "z"
                width: parent.width / 3.5

                delegate: FloatEdit {
                    id: textInputZ
                    value: styleData.value
                    round: settings.roundCount

                    onEditingFinished: {
                        var index = tableView.model.index(styleData.row, 0)
                        tableView.model.setData(index, value, AntSystemElementsModel.ZRole)
                        textInputZ.focus = false;
                    }
                }
            }

        }
    }
}



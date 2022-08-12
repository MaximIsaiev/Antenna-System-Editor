import QtQuick 2.11
import DfPost.Controls.Stc 1.0
import QtQuick.Controls 2.4
import DfPost.CoreQml 1.0
import QtQuick.Layouts 1.11
import QtQml.Models 2.2
import DfPost.AntSystem 1.0


Frame {
    id: frameLeftBox

    // флаг отрисовки главного вида (true - нужно перерисовать главный вид, false - не нужно)
    property bool needRedrawMainChart: false

    // флаг отрисовки всех остальных видов
    property bool needRedrawRestChart: false

    // true - необходимо перерисовать все виды
    property bool needRecalculatedAxisMainChart: false

    // флаг типа задания ДН
    property bool isSector: false

    // модель с именами АС
    property var antSysNameModel

    // поля антенной системы
    property QtObject antSystemBackend
    property var maxAz
    property var maxAng

    // локальные свойства
    QtObject {
        id: settings

        // отступ
        property int defWidth: 1
        property int defHeight: 30
        property int defMargin: 20

        // диапазон ввода для FloatEdit
        property real minimumValueFloatEdit: -360.0
        property real maximumValueFloatEdit: 360.0
    }

    // сигнал выбора пользователя новой АС
    signal currAntennaNameChanged(string currName)

    // когда пользователь захотел переименовать систему
    signal indexChanged(var index, int role)

    // сигнал о том, что был нажат Switch сектор
    signal sectorChanged()

    // сигнал двойного щелчка мышью по TreeView
    signal doubleClickedTreeView()

    contentItem: ColumnLayout {

        Label {
            text: qsTr("Состав антенной системы:")
        }

        Frame {
            id: frameTree
            Layout.fillWidth: true
            Layout.fillHeight: true

            TreeView {
                id: tree
                height: frameTree.height - settings.defMargin
                width: frameTree.width - settings.defMargin
                headerVisible: false
                model: antSysNameModel

                TreeViewColumn {
                    id: treeColumn
                    role: "display"
                }

                itemDelegate: Label {
                    height: frameTree.height
                    width: frameTree.width
                    text: styleData.value
                }

                onClicked: {
                    // очистка всех графиков
                    needRedrawMainChart = true
                    needRedrawRestChart = true
                    needRecalculatedAxisMainChart = true
                    // говорим, какую именно АС выбрал пользователь
                    frameLeftBox.currAntennaNameChanged(antSysNameModel.data(index))
                    indexChanged(currentIndex, treeColumn.role)
                }

                onDoubleClicked: {
                    doubleClickedTreeView()
                }
            }
        }

        ColumnLayout {

            GridLayout {

                Label {
                    text: qsTr("Крен: ")
                    Layout.row: 1
                    Layout.column: 1

                }

                FloatEdit {
                    id: textRoll
                    Layout.row: 1
                    Layout.column: 2
                    Layout.fillWidth: true
                    Layout.minimumWidth: settings.defWidth
                    Layout.maximumHeight: settings.defHeight
                    value: antSystemBackend.roll
                    minValue: settings.minimumValueFloatEdit
                    maxValue: settings.maximumValueFloatEdit

                    onEditingFinished: {
                        needRedrawRestChart = true
                        needRecalculatedAxisMainChart = false

                        textRoll.focus = false;

                        // передача в с++ класс значение крена
                        antSystemBackend.roll = value
                    }
                }

                Label {
                    text: qsTr("Тангаж: ")
                    Layout.row: 1
                    Layout.column: 3
                }

                FloatEdit {
                    id: textPitch
                    Layout.row: 1
                    Layout.column: 4
                    Layout.fillWidth: true
                    Layout.minimumWidth: settings.defWidth
                    Layout.maximumHeight: settings.defHeight
                    value: antSystemBackend.pitch
                    minValue: settings.minimumValueFloatEdit
                    maxValue: settings.maximumValueFloatEdit

                    onEditingFinished: {
                        // очистка графиков (кроме главного)
                        needRedrawRestChart = true
                        needRecalculatedAxisMainChart = false

                        textPitch.focus = false;

                        // передача в с++ класс значение крена
                        antSystemBackend.pitch = value
                    }
                }

                Label {
                    Layout.row: 2
                    Layout.column: 1
                    text: qsTr("Рыскание: ")

                }

                FloatEdit {
                    id: textYaw
                    Layout.row: 2
                    Layout.column: 2
                    Layout.fillWidth: true
                    Layout.minimumWidth: settings.defWidth
                    Layout.maximumHeight: settings.defHeight
                    value: antSystemBackend.yaw
                    minValue: settings.minimumValueFloatEdit
                    maxValue: settings.maximumValueFloatEdit

                    onEditingFinished: {
                        // очистка графиков (кроме главного)
                        needRedrawRestChart = true
                        needRecalculatedAxisMainChart = false

                        textPitch.focus = false;

                        // передача в с++ класс значение тангажа
                        antSystemBackend.yaw = value
                    }
                }

                Label {
                    text: qsTr("Диапазон чаcтот, МГц: ")
                    font.bold: true
                    Layout.row: 4
                    Layout.column: 1
                    Layout.columnSpan: 4
                }

                Label {
                    text: qsTr("от: ")
                    Layout.row: 5
                    Layout.column: 1
                }

                FreqEdit {
                    id: textBegFreq
                    Layout.row: 5
                    Layout.column: 2
                    Layout.fillWidth: true
                    Layout.minimumWidth: settings.defWidth
                    Layout.maximumHeight: settings.defHeight
                    value: antSystemBackend.begFreq
                    onEditingFinished: {
                        // передача в с++ класс нижней границы частоты
                        antSystemBackend.begFreq = value;

                        textBegFreq.focus = false;
                    }
                }

                Label {
                    text: qsTr("до: ")
                    Layout.row:  5
                    Layout.column: 3
                }

                FreqEdit {
                    id: textEndFreq
                    Layout.row: 5
                    Layout.column: 4
                    Layout.fillWidth: true
                    Layout.minimumWidth: settings.defWidth
                    Layout.maximumHeight: settings.defHeight
                    value: antSystemBackend.endFreq
                    onEditingFinished: {
                        // передача в с++ класс верхней границы частоты
                        antSystemBackend.endFreq = value
                        textEndFreq.focus = false;
                    }
                }

                Item {
                    height: 10
                    Layout.row: 6
                }

                Label {
                    text: qsTr("Диаграмма направленности:")
                    font.bold: true
                    Layout.row: 7
                    Layout.column: 1
                    Layout.columnSpan: 4
                }

                Label {
                    text: qsTr("По горизонтали")
                    Layout.row: 8
                    Layout.column: 1
                    Layout.columnSpan: 4
                }

                Label {
                    text: qsTr("от: ")
                    Layout.row: 9
                    Layout.column: 1
                }

                FloatEdit {
                    id: textMinAz
                    Layout.row: 9
                    Layout.column: 2
                    Layout.fillWidth: true
                    Layout.maximumHeight: settings.defHeight
                    value: antSystemBackend.minAz
                    minValue: settings.minimumValueFloatEdit
                    maxValue: settings.maximumValueFloatEdit
                    onEditingFinished: {
                        // передача в с++ класс значение нижней границы азимута
                        antSystemBackend.minAz = value
                        textMinAz.focus = false;
                        textMaxAz.editingFinished()
                    }
                }

                Label {
                    id: textAz
                    text: qsTr("до: ")
                    Layout.row: 9
                    Layout.column: 3
                }

                FloatEdit {
                    id: textMaxAz
                    Layout.row: 9
                    Layout.column: 4
                    Layout.fillWidth: true
                    Layout.maximumHeight: settings.defHeight
                    value: maxAz
                    minValue: settings.minimumValueFloatEdit
                    maxValue: settings.maximumValueFloatEdit
                    onEditingFinished:{
                        // передача в с++ класс значение верхней границы азимута
                        if (isSector)
                            antSystemBackend.maxAz = value + antSystemBackend.minAz
                        else
                            antSystemBackend.maxAz = value
                        textMaxAz.focus = false;
                    }
                }

                Label {
                    text: qsTr("По вертикали")
                    Layout.row: 10
                    Layout.column: 1
                    Layout.columnSpan: 4
                }

                Label {
                    text: qsTr("от: ")
                    Layout.row: 11
                    Layout.column: 1
                }

                FloatEdit {
                    id: textMinAng
                    Layout.row: 11
                    Layout.column: 2
                    Layout.fillWidth: true
                    Layout.maximumHeight: settings.defHeight
                    value: antSystemBackend.minAng
                    minValue: settings.minimumValueFloatEdit
                    maxValue: settings.maximumValueFloatEdit
                    onEditingFinished: {
                        // передача в с++ класс значение нижней границы угла
                        antSystemBackend.minAng = value
                        textMinAng.focus = false;
                        textMaxAng.editingFinished()
                    }
                }

                Label {
                    id: textAng
                    text: qsTr("до: ")
                    Layout.row: 11
                    Layout.column: 3
                }

                FloatEdit {
                    id: textMaxAng
                    Layout.row: 11
                    Layout.column: 4
                    Layout.fillWidth: true
                    Layout.maximumHeight: settings.defHeight
                    value: maxAng
                    minValue: settings.minimumValueFloatEdit
                    maxValue: settings.maximumValueFloatEdit
                    onEditingFinished:{
                        // передача в с++ класс значение верхней границы угла
                        if (isSector)
                            antSystemBackend.maxAng = value + antSystemBackend.minAng
                        else
                            antSystemBackend.maxAng = value

                        textMaxAng.focus = false;
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignCenter


                Label {
                    id: textVariation
                    text: qsTr("Задать сектором")
                }

                Switch {
                    id: switchSector
                    onCheckedChanged: {
                        if (switchSector.checked) {
                            textAz.text = qsTr("сектор: ")
                            textAng.text = qsTr("сектор: ")
                            isSector = true
                        } else {
                            textAz.text = qsTr("до: ")
                            textAng.text = qsTr("до: ")
                            isSector = false
                        }
                        sectorChanged()
                    }
                }
            }
        }
    }
}

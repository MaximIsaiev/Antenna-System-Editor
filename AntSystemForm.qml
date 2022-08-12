import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.3
import QtQuick 2.10
import ru.stcspb 1.0

Page {
    id: mainframe
    title: qsTr("Редактор АС")

    // хранит в себе индекс выбранной АС
    property var index

    // хранит в себе роль listView
    property int role

    property int location
    property int direction

    Component.onCompleted: {
        disableButtons()
    }

    function disableButtons() {
        newSubPanelButton.enabled = false
        renamePanelButton.enabled = false
        deletePanelButton.enabled = false
        elementMenu.enabled = false
    }
    contentItem: ColumnLayout {
        MenuBar {

            Menu {
                title: qsTr("Файл")

                Action {
                    id: newFileButton
                    text: qsTr("Новый")
                    onTriggered: createNewFileDialog.open()
                }

                Action {
                    id: openFileButton
                    text: qsTr("Открыть")
                    onTriggered: loadFileDialog.open()
                }

                Action {
                    id: saveFileButton
                    text: qsTr("Сохранить как")
                    onTriggered: saveFileDialog.open()
                }

                MenuSeparator { }

                Action {
                    text: qsTr("Выход")
                    onTriggered: Qt.quit()
                }
            }
            Menu {
                id: systemMenu
                title: qsTr("Система")

                Action {
                    id: newPanelButton;
                    text: qsTr("Новая панель");
                    onTriggered: {
                        newAntSystemDialog.open()
                        newAntennaNameField.text = "Новая панель (" + Number(antsyseditor.antSystemCount() + 1).toLocaleString() + ")"
                    }
                }

                Action {
                    id: newSubPanelButton
                    text: qsTr("Новая подпанель");
                    onTriggered: {
                        newPanelDialog.open()
                        newPanelField.text = "Новая панель (" + Number(antsyseditor.antSystemCount() + 1).toLocaleString() + ")"
                    }
                }

                Action {
                    id: renamePanelButton
                    text: qsTr("Переименовать систему")
                    onTriggered: renameAntSystemDialog.open()
                }

                Action {
                    id: deletePanelButton
                    text: qsTr("Удалить систему")
                    onTriggered: {
                        deleteAntSystemDialog.open()
                        disableButtons()
                    }
                }
            }

            Menu {
                id: elementMenu
                title: qsTr("Элемент")

                Action {
                    id: addNewElementButton
                    text: qsTr("Добавить элемент")
                    onTriggered: {
                        // говорим о том, что нужно перестроить все графики
                        antsyseditor.needRecalculatedAxisMainChart = true

                        // вызываем функцию создания нового элемента
                        antsyseditor.createNewAntSystemElement()
                    }
                }

                Action {
                    id: addNewGroupButton
                    text: qsTr("Добавить группу")
                    onTriggered: addGroupElementsDialog.open()
                }

                Action {
                    id: deleteElementButton
                    text: qsTr("Удалить элемент")
                    onTriggered: {
                        // говорим о том, что нужно перестроить все графики
                        antsyseditor.needRecalculatedAxisMainChart = true

                        // вызываем функцию создания нового элемента
                        antsyseditor.deleteAntSystemElement()
                    }
                }

                Action {
                    id: deleteAllElementsButton
                    text: qsTr("Удалить все элементы")
                    onTriggered: deleteAntSystemElementsDialog.open()
                }
            }
        }

        AntSysEditorPage {
            id: antsyseditor
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        ToolBar {

            RowLayout {

                Label {
                    id: footerText
                    // вызываем функцию обработки файла
                    text: loadFileDialog.fileUrl
                }
            }
        }
    }

    Dialog {
        id: createNewFileDialog
        title: qsTr("Новый файл")
        standardButtons: Dialog.Ok | Dialog.Cancel

        Label {
            text: qsTr("Внимание! Данное действие приведёт к потере всех несохраненные данных.\nУбедитесь, что все важные файлы сохранены.");
            Layout.margins: 5
        }
        onAccepted: {
            antsyseditor.createNewFile()
            disableButtons()
        }
    }

    Dialog {
        id: newPanelDialog
        title: qsTr("Новая подпанель")
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            anchors.fill: parent

            RowLayout {

                Label {
                    text: qsTr("Имя")
                    Layout.margins: 5
                }

                TextField {
                    id: newPanelField
                    Layout.fillWidth: true
                    Layout.margins: 5
                }
            }
        }
        onAccepted: {
            // передаем в бэкенд имя новой АС

            antsyseditor.setNewPanel(newPanelField.text, index)
            newPanelField.text = "Новая панель (" + Number(antsyseditor.antSystemCount() + 1).toLocaleString() + ")"
            newAntSystemDialog.close()
        }
    }

    Dialog {
        id: deleteAntSystemDialog
        title: qsTr("Удаление АС")
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            anchors.fill: parent

            Label {
                text: qsTr("Вы действительно хотите удалить АС?")
                Layout.alignment: Qt.AlignHCenter
            }
        }
        onAccepted: {
            //вызываем функцию удаление АС
            antsyseditor.deleteAntSystem(index)
        }
    }

    Dialog {
        id: deleteAntSystemElementsDialog
        title: qsTr("Удаление всех элементов")
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            anchors.fill: parent

            Label {
                text: qsTr("Вы действительно хотите удалить все элементы АС?")
                Layout.alignment: Qt.AlignHCenter
            }
        }
        onAccepted: {
            antsyseditor.deleteAllAntSystemElements()
        }
    }

    Dialog {
        id: newAntSystemDialog
        title: qsTr("Новая АС")
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            anchors.fill: parent

            RowLayout {

                Label {
                    text: qsTr("Имя")
                    Layout.margins: 5
                }

                TextField {
                    id: newAntennaNameField
                    Layout.fillWidth: true
                    Layout.margins: 5
                }
            }
        }
        onAccepted: {
            // передаем в бэкенд имя новой АС

            antsyseditor.setNewAntSystem(newAntennaNameField.text)
            newAntSystemDialog.close()
            newAntennaNameField.text = "Новая панель (" + Number(antsyseditor.antSystemCount() + 1).toLocaleString() + ")"
        }
    }

    Dialog {
        id: addGroupElementsDialog
        title: qsTr("Группа элементов")
        standardButtons: Dialog.Ok | Dialog.Cancel

        GridLayout {
            columns: 2
            rows: 4

            Label {
                text: qsTr("Расположение")
                Layout.column: 1
                Layout.row: 1
            }

            ComboBox {
                id: cbLocation
                Layout.column: 2
                Layout.row: 1
                Layout.fillWidth: true
                textRole: "text"

                model: ListModel {
                    id: locationModel
                    ListElement { text: qsTr("По кругу") }
                    ListElement { text: qsTr("По кругу (первый в центре)") }
                    ListElement { text: qsTr("По кругу (последний в центре)") }
                }
                onActivated: {
                    switch (currentIndex) {
                    case 0:
                        location = AntSystemEditorBackend.RoundLocation;
                        break;
                    case 1:
                        location = AntSystemEditorBackend.FirstInCenter;
                        break;
                    case 2:
                        location = AntSystemEditorBackend.LastInCenter;
                        break;
                    }
                }
            }

            Label {
                Layout.column: 1
                Layout.row: 2
                text: qsTr("Направление")
            }

            ComboBox {
                id: cbDirection
                Layout.column: 2
                Layout.row: 2
                Layout.preferredWidth: 200
                textRole: "text"

                model: ListModel {
                    id: directionModel
                    ListElement { text: qsTr("По часовой стрелке") }
                    ListElement { text: qsTr("Против часовой стрелки") }
                }
                onActivated: {
                    switch (currentIndex) {
                    case 0:
                        direction = AntSystemEditorBackend.Clockwise;
                        break;
                    case 1:
                        direction = AntSystemEditorBackend.CounterClockwise;
                        break;
                    }
                }
            }

            Label {
                Layout.column: 1
                Layout.row: 3
                text: qsTr("Число элементов")
            }

            SpinBox {
                Layout.column: 2
                Layout.row: 3
                id: countSpin
                value: 8
                editable: true
            }

            Label {
                Layout.column: 1
                Layout.row: 4
                text: qsTr("Диаметр, м")
            }

            SpinBox {
                Layout.column: 2
                Layout.row: 4
                id: diameterSpin
                value: 5
                editable: true
            }
        }
        onAccepted: {
            antsyseditor.addAntSystemGroupElements(location, direction, countSpin.value, diameterSpin.value)
        }
        onRejected: {
            addGroupElementsDialog.close()
        }
    }

    Dialog {
        id: renameAntSystemDialog
        title: qsTr("Переименовать АС")
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            anchors.fill: parent

            RowLayout {

                Label {
                    text: qsTr("Имя")
                    Layout.margins: 5
                }

                TextField {
                    id: renameAntennaNameField
                    Layout.fillWidth: true
                    Layout.margins: 5
                }
            }
        }
        onAccepted: {
            // передаем в бэкенд имя новой АС
            antsyseditor.renameAntSystem(index, renameAntennaNameField.text)
        }
    }

    FileDialog {
        id: loadFileDialog
        folder: "."
        title: qsTr("Выберите файл антенной системы")
        selectMultiple: false
        nameFilters: [ "Документ XML (*.xml)" ]
        onAccepted: {
            // передаем в бэкенд имя пути к файлу
            antsyseditor.loadAntSystemFile(loadFileDialog.fileUrl)
            footerText.text = loadFileDialog.fileUrl
        }
    }

    FileDialog {
        id: saveFileDialog
        folder: "."
        title: qsTr("Сохранить как")
        selectExisting: false
        selectMultiple: false
        nameFilters: [ "Документ XML (*.xml)" ]
        onAccepted: {
            // вызываем функцию сохранения файла
            antsyseditor.saveAntSystemFile(saveFileDialog.fileUrl);
        }
    }

    Connections {
        target: antsyseditor
        onIndexChanged: {
            index = getIndex
            role = getRole
            newSubPanelButton.enabled = true
            renamePanelButton.enabled = true
            deletePanelButton.enabled = true
            elementMenu.enabled = true
            deleteElementButton.enabled = false;
        }
        onErrorLogged: {
            footerText.text = error
        }
        onInfoLogged: {
            footerText.text = info
        }
        onDoubleClicked: {
            renameAntSystemDialog.open()
        }
    }
}

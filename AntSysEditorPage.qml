import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as Q1
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.3
import DfPost.AntSystem 1.0

Page {
    id: antSystemPage
    property double space: 0.1
    property double xMax
    property double xMin
    property double yMax
    property double yMin
    property double zMax
    property double zMin
    property double azimut: 0
    property double angle: 0
    property alias needRecalculatedAxisMainChart: leftbox.needRecalculatedAxisMainChart
    property bool antSystemTopologyEmpty: false
    property bool firstLapOfCycle: true

    // сигнал говорит о том, что пользователь выбрал АС (соотвественно изменился индекс listview)
    signal indexChanged(var getIndex, int getRole)
    signal errorLogged(var error)
    signal infoLogged(var info)
    signal doubleClicked()

    AntSystemEditorBackend {
        id: backend
    }

    function saveAntSystemFile(filePath) {
        backend.saveAntSystemFile(filePath)
    }

    function loadAntSystemFile(filePath) {
        backend.loadAntSystemFile(filePath)
    }

    function setNewAntSystem(systemName) {
        backend.createNewAntSystem(systemName)
    }

    function setNewPanel(panelName, index) {
        backend.createNewSubPanel(panelName, index)
    }

    function createNewAntSystemElement() {
        backend.createNewAntSystemElement()
    }

    function deleteAntSystemElement() {
        backend.deleteAntSystemElement(centerBox.currElement)
    }

    function deleteAllAntSystemElements() {
        backend.deleteAllAntSystemElements()
    }

    function deleteAntSystem(index) {
        backend.deleteAntSystem(index)
    }

    function renameAntSystem(index, newName) {
        backend.updateAntSystemName(index, newName)
    }

    function addAntSystemGroupElements (location, direction, count, diameter) {
        backend.drawGroupElements(location, direction, count, diameter)
    }

    function antSystemCount () {
        return backend.countAntSystems()
    }

    function createNewFile () {
        backend.createNewFile()
    }

    Connections {
        target: backend
        onPointXYZChanged: {
            if (leftbox.needRedrawRestChart) {

                //  очистка старых графиков при запросе новых
                centerBox.clearAllProjections()

                // говорим о том, что сейчас будет построена первая точка
                centerBox.isFirstPointDrawn = true

                // говорим о том, что это первый круг цикла
                centerBox.isFirstLapOfCycle = true

                leftbox.needRedrawRestChart = false
            }
            // поиск нового максимума и минимума осей
            centerBox.searchNewMaxAndMinForAllChartsExceptMain(backend.pointX, backend.pointY, backend.pointZ, space)

            // добавление нужных точек на графики
            centerBox.addPointsXYZ(backend.pointX, backend.pointY, backend.pointZ)

        }
        onPointXYZBackupChanged: {
            if (leftbox.needRedrawMainChart) {

                // очистка главного графика
                centerBox.clearMainChart()

                // говорим о том, что сейчас будет построена первая точка
                centerBox.isFirstPointDrawn = true

                // говорим о том, что это первый круг цикла
                centerBox.isFirstLapOfCycle = true

                leftbox.needRedrawMainChart = false


            }
            // поиск нового максимума и минимума осей
            centerBox.searchNewMaxAndMinForMainChart(backend.pointX, backend.pointY, needRecalculatedAxisMainChart, space)

            centerBox.addPointsXYMainChart(backend.pointX, backend.pointY)

        }
        onChartsNeedsClear: {
            // полная очистка всех графиков
            centerBox.clearAllProjections()
            centerBox.clearMainChart()

            leftbox.needRedrawRestChart = true
            leftbox.needRedrawMainChart = true
            leftbox.needRecalculatedAxisMainChart = true
        }
        onMaxAzChanged: {
            if (leftbox.isSector)
                azimut = backend.maxAz - backend.minAz
            else
                azimut = backend.maxAz
        }
        onMaxAngChanged: {
            if (leftbox.isSector)
                angle = backend.maxAng - backend.minAng
            else
                angle = backend.maxAng
        }
        onErrorLogged: {
            errorLogged(backend.error)
        }
        onInformationLogged: {
            infoLogged(backend.information)
        }
    }
    Connections {
        target: leftbox
        onCurrAntennaNameChanged: {
            // обновляем элементы в таблице координат
            backend.currAntennaName = currName
        }

        onIndexChanged: {
            indexChanged(index, role)
        }
        onSectorChanged: {
            if (leftbox.isSector) {
                azimut = backend.maxAz - backend.minAz
                angle = backend.maxAng - backend.minAng
            } else {
                azimut = backend.maxAz
                angle = backend.maxAng
            }
        }
        onDoubleClickedTreeView: {
            doubleClicked()
        }
    }
    Connections {
        target: centerBox
        onPressedMouse: {
            leftbox.needRedrawMainChart  = true
            backend.drawPointXYZBackup()
        }
    }

    contentItem: RowLayout {
        anchors.fill: parent

        Q1.SplitView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: Qt.Horizontal

            LeftBox {
                id: leftbox
                width: 325
                Layout.minimumWidth: 325
                Layout.maximumWidth: 650
                antSysNameModel: backend.antSystemName
                antSystemBackend: backend
                maxAz: azimut
                maxAng: angle
            }

            Q1.SplitView {
                orientation: Qt.Vertical

                CenterBox {
                    id: centerBox
                    Layout.fillHeight: true
                    Layout.minimumHeight: parent.height * 1 / 2
                    Layout.maximumHeight: parent.height * 3 / 4
                }

                BottomBox {
                    id: bottomBox
                    bottomBoxModel: backend.elementsModel
                    Layout.minimumHeight: parent.height * 1 / 4
                    Layout.maximumHeight: parent.height * 1 / 2
                }
            }
        }
    }
}





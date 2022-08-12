import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQml 2.11
import QtCharts 2.2
import QtQuick 2.11


Frame {
    property bool isFirstPointDrawn: true
    property bool isFirstLapOfCycle: true

    //  локальные свойства
    QtObject {
        id: settings

        property double firstPointX: 0.0
        property double firstPointY: 0.0
        property double firstPointZ: 0.0

        // переменные необходимые для отслеживания выбранной АС
        property int currElement: 0
        property var currPoint: undefined

        // размер графиков
        property int defSize: 350
        property double chartMargin: 0.1
        readonly property int chartHeight: settings.defSize

        // ограничения по осям
        property double xMax: 0.0
        property double xMin: 0.0
        property double yMax: 0.0
        property double yMin: 0.0
        property double zMax: 0.0
        property double zMin: 0.0

        // тема графиков
        property int mainTheme: ChartView.ChartThemeDark
    }

    // сигнал выбора пользователя элемента АС
    signal currAntennaElementChanged()
    signal pressedMouse()

    ValueAxis {
        id: axisMainProjectionY
    }
    ValueAxis {
        id: axisMainProjectionX
    }
    ValueAxis {
        id: axisRightProjectionY
    }
    ValueAxis {
        id: axisRightProjectionX
    }
    ValueAxis {
        id: axisBackProjectionY
    }
    ValueAxis {
        id: axisBackProjectionX
    }
    ValueAxis {
        id: axisMainProjectionNoTransformationY
    }
    ValueAxis {
        id: axisMainProjectionNoTransformationX
    }
    contentItem: RowLayout {
            ChartView {
                Layout.alignment: Qt.AlignRight
                id: mainChart
                title: qsTr("Вид сверху без трансформаций")
                theme: settings.mainTheme

                // отвечают за перетаскивание точек
                property var selectedPoint: undefined
                property real toleranceX: 0.05
                property real toleranceY: 0.05

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.preferredWidth:  700
                Layout.preferredHeight: 700
                Layout.maximumWidth: height
                Layout.minimumWidth: height
                antialiasing: true
                legend.visible: false
                LineSeries {
                    id: connectingLineBetweenPoints
                    axisX: axisMainProjectionX
                    axisY: axisMainProjectionY
                }
                ScatterSeries {
                    id: mainProjection
                    axisX: axisMainProjectionX
                    axisY: axisMainProjectionY
                }
                ScatterSeries {
                    id: highlightSelectedElement
                    axisX: axisMainProjectionX
                    axisY: axisMainProjectionY
                }
                ScatterSeries {
                    id: firstElementIsRedMainProjection
                    axisX: axisMainProjectionX
                    axisY: axisMainProjectionY
                    color: "red"
                }


                // отслеживаем нажатие мыши
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        // при нажатии мыши на график его необходимо перестроить, так как у нас либо должна
                        // выделиться какая-то точка, либо наоборот перестать выделяться
                        pressedMouse()

                        var cp = mainChart.mapToValue(Qt.point(mouse.x, mouse.y))
                        for (var i = 0; i < mainProjection.count; ++i) {
                            var p = mainProjection.at(i)
                            if ((Math.abs(cp.x - p.x) <= mainChart.toleranceX) && (Math.abs(cp.y - p.y) <= mainChart.toleranceY)) {
                                settings.currElement = i + 1
                                mainChart.selectedPoint = p
                                settings.currPoint = p
                                highlightSelectedElement.append(p.x, p.y)
                                mainProjection.remove(p.x, p.y)
                                break;
                            }
                        }
                        for (var j = 0; j < firstElementIsRedMainProjection.count; j++) {
                            p = firstElementIsRedMainProjection.at(j)
                            if ((Math.abs(cp.x - p.x) <= mainChart.toleranceX) && (Math.abs(cp.y - p.y) <= mainChart.toleranceY)) {
                                settings.currElement = j
                                mainChart.selectedPoint = p
                                settings.currPoint = p
                                highlightSelectedElement.append(p.x, p.y)
                                firstElementIsRedMainProjection.remove(p.x, p.y)
                                break;
                            }
                        }
                    }
                }
            }
            GridLayout {
                id: grid


                Layout.fillHeight: true
                Layout.fillWidth: true
                rows: 2
                columns: 2
                ChartView {
                    title: qsTr("Вид справа")
                    theme: settings.mainTheme
                    Layout.preferredWidth: settings.defSize
                    Layout.preferredHeight:  settings.chartHeight
                    Layout.maximumWidth: height
                    Layout.minimumWidth: height
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.row: 1
                    Layout.column: 1
                    antialiasing: true
                    legend.visible: false
                    ScatterSeries {
                        id: rightProjection
                        axisX: axisRightProjectionX
                        axisY: axisRightProjectionY
                    }
                    ScatterSeries {
                        id: firstElementIsRedRightProjection
                        axisX: axisRightProjectionX
                        axisY: axisRightProjectionY
                        color: "red"
                    }
                }
                ChartView {
                    title: qsTr("Вид сзади")
                    theme: settings.mainTheme
                    Layout.preferredWidth: settings.defSize
                    Layout.preferredHeight: settings.chartHeight
                    Layout.maximumWidth: height
                    Layout.minimumWidth: height
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.row: 2
                    Layout.column: 1
                    //                Layout.alignment: Qt.AlignBottom
                    antialiasing: true
                    legend.visible: false
                    ScatterSeries {
                        id: backProjection
                        axisX: axisBackProjectionX
                        axisY: axisBackProjectionY
                    }
                    ScatterSeries {
                        id: firstElementIsRedBackProjection
                        axisX: axisBackProjectionX
                        axisY: axisBackProjectionY
                        color: "red"
                    }
                }
                ChartView {
                    title: qsTr("3D-вид")
                    theme: settings.mainTheme
                    Layout.preferredWidth: settings.defSize
                    Layout.maximumWidth: height
                    Layout.minimumWidth: height
                    Layout.preferredHeight: settings.chartHeight
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.row: 1
                    Layout.column: 2
                    antialiasing: true
                    legend.visible: false
                    LineSeries {
                        XYPoint { x: 0; y: 0 }
                        XYPoint { x: 1; y: 1 }
                        XYPoint { x: 3; y: 7 }
                        XYPoint { x: 9; y: 11 }
                    }
                }
                ChartView {
                    title: qsTr("Вид сверху")
                    theme: settings.mainTheme
                    Layout.preferredWidth: settings.defSize
                    Layout.maximumWidth: height
                    Layout.minimumWidth: height
                    Layout.preferredHeight: settings.chartHeight
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.row: 2
                    Layout.column: 2
                    antialiasing: true
                    legend.visible: false
                    ScatterSeries {
                        id: mainProjectionNoTransformation
                        axisX: axisMainProjectionNoTransformationX
                        axisY: axisMainProjectionNoTransformationY
                    }
                    ScatterSeries {
                        id: firstElementIsRedMainProjectionNoTransformation
                        axisX: axisMainProjectionNoTransformationX
                        axisY: axisMainProjectionNoTransformationY
                        color: "red"
                    }
                }
            }
        }


    function clearAllProjections () {
        rightProjection.clear()
        backProjection.clear()
        mainProjectionNoTransformation.clear()

        //очищаем график от красных точек
        firstElementIsRedRightProjection.clear()
        firstElementIsRedBackProjection.clear()
        firstElementIsRedMainProjectionNoTransformation.clear()
    }

    function clearMainChart () {
        mainProjection.clear()
        connectingLineBetweenPoints.clear()

        // "забываем" значение выбранного элемента
        mainChart.selectedPoint = undefined

        // удаляем с графика подсвеченный элемент
        highlightSelectedElement.clear()

        // удаляем с графика красную точку
        firstElementIsRedMainProjection.clear()
    }

    function addPointsXYZ (pointX, pointY, pointZ) {
        if (isFirstPointDrawn) {
            settings.firstPointX = pointX
            settings.firstPointY = pointY
            settings.firstPointZ = pointZ
            firstElementIsRedRightProjection.append(pointY, pointZ)
            firstElementIsRedBackProjection.append(pointX, pointZ)
            firstElementIsRedMainProjectionNoTransformation.append(pointX, pointY)
            isFirstPointDrawn = false
        } else {
            // добавление нужных точек на графики
            mainProjectionNoTransformation.append(pointX, pointY)
            rightProjection.append(pointY, pointZ)
            backProjection.append(pointX, pointZ)
        }
    }

    function addPointsXYMainChart (pointX, pointY) {
        if (isFirstPointDrawn) {
            settings.firstPointX = pointX
            settings.firstPointY = pointY
            firstElementIsRedMainProjection.append(pointX, pointY)
            isFirstPointDrawn = false
        } else {
            // добавление нужных точек на графики
            mainProjection.append(pointX, pointY)
            connectingLineBetweenPoints.append(settings.firstPointX, settings.firstPointY)
            connectingLineBetweenPoints.append(pointX, pointY)
        }
    }

    function searchNewMaxAndMinForAllChartsExceptMain (pointX, pointY, pointZ, space) {

        if (isFirstLapOfCycle) {
            settings.xMax = pointX
            settings.xMin = pointX
            settings.yMax = pointY
            settings.yMin = pointY
            settings.zMax = pointZ
            settings.zMin = pointZ
            isFirstLapOfCycle = false
        }

        if (pointX >= settings.xMax)
            settings.xMax = pointX

        if (pointX <= settings.xMin)
            settings.xMin = pointX

        if (pointY >= settings.yMax)
            settings.yMax = pointY

        if (pointY <= settings.yMin)
            settings.yMin = pointY

        if (pointZ >= settings.zMax)
            settings.zMax = pointZ

        if (pointZ <= settings.zMin)
            settings.zMin = pointZ

        var differenceX = Math.abs(settings.xMax - settings.xMin)
        var differenceY = Math.abs(settings.yMax - settings.yMin)
        var differenceZ = Math.abs(settings.zMax - settings.zMin)
        var maxDifference = Math.max(differenceX, differenceY, differenceZ)
        var margin = maxDifference * settings.chartMargin
        if (margin == 0)
            margin = space

        axisMainProjectionNoTransformationX.max = settings.xMax + (Math.abs(differenceX - maxDifference) / 2 + margin)
        axisMainProjectionNoTransformationX.min = settings.xMin - (Math.abs(differenceX - maxDifference) / 2 + margin)
        axisMainProjectionNoTransformationY.max = settings.yMax + (Math.abs(differenceY - maxDifference) / 2 + margin)
        axisMainProjectionNoTransformationY.min = settings.yMin - (Math.abs(differenceY - maxDifference) / 2 + margin)

        axisBackProjectionX.max = settings.xMax + (Math.abs(differenceX - maxDifference) / 2 + margin)
        axisBackProjectionX.min = settings.xMin - (Math.abs(differenceX - maxDifference) / 2 + margin)
        axisBackProjectionY.max = settings.zMax + (Math.abs(differenceZ - maxDifference) / 2 + margin)
        axisBackProjectionY.min = settings.zMin - (Math.abs(differenceZ - maxDifference) / 2 + margin)

        axisRightProjectionX.max = settings.yMax + (Math.abs(differenceY - maxDifference) / 2 + margin)
        axisRightProjectionX.min = settings.yMin - (Math.abs(differenceY - maxDifference) / 2 + margin)
        axisRightProjectionY.max = settings.zMax + (Math.abs(differenceZ - maxDifference) / 2 + margin)
        axisRightProjectionY.min = settings.zMin - (Math.abs(differenceZ - maxDifference) / 2 + margin)
    }

    function searchNewMaxAndMinForMainChart (pointX, pointY, needRecalculatedAxisMainChart, space) {

        if (isFirstLapOfCycle) {
            settings.xMax = pointX
            settings.xMin = pointX
            settings.yMax = pointY
            settings.yMin = pointY
            isFirstLapOfCycle = false
        }

        if (pointX >= settings.xMax)
            settings.xMax = pointX

        if (pointX <= settings.xMin)
            settings.xMin = pointX

        if (pointY >= settings.yMax)
            settings.yMax = pointY

        if (pointY <= settings.yMin)
            settings.yMin = pointY

        var differenceY = Math.abs(settings.yMax - settings.yMin)
        var differenceX = Math.abs(settings.xMax - settings.xMin)
        var maxDifferenceXY = Math.max(differenceX, differenceY)
        var margin = maxDifferenceXY * settings.chartMargin
        if (margin == 0)
            margin = space

        if (needRecalculatedAxisMainChart) {
            axisMainProjectionX.max = settings.xMax + (Math.abs(differenceX - maxDifferenceXY) / 2 + margin)
            axisMainProjectionX.min = settings.xMin - (Math.abs(differenceX - maxDifferenceXY) / 2 + margin)
            axisMainProjectionY.max = settings.yMax + (Math.abs(differenceY - maxDifferenceXY) / 2 + margin)
            axisMainProjectionY.min = settings.yMin - (Math.abs(differenceY - maxDifferenceXY) / 2 + margin)
        }
    }
}

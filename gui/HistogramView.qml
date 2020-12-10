import "colorizer.js" as Colorizer
import QtCharts 2.3

ChartView {
    property var barText

    id: histogramView
    margins.top: 0
    margins.left: 0
    margins.right: 0

    HorizontalBarSeries {
        id: barSeries
        useOpenGL: true

        onHovered: {
            if (status == true) {
                barset.color = Qt.darker(barset.color, 3)
                barText = qsTr("<b>" + barset.label + "</b> : <i>" + barset.values[0] + "</i>")
            } else {
                barset.color = Qt.lighter(barset.color, 3)
                barText = qsTr("")
            }
        }
        barWidth: 1
        axisY: BarCategoryAxis { categories: [" "] }
        axisX: ValueAxis { id: valueAxis; labelFormat: "%d";}
    }

    function changeWordCountbyMap(map) {
        barSeries.clear()
        var maxValue = 0
        for (var prop in map) {
            var value = map[prop]
            if (value > maxValue)
                maxValue = value
            barSeries.insert(0, prop, [value]).color = Colorizer.colorizer(barSeries.count - 1)
        }
        if (valueAxis.max !== maxValue)
            valueAxis.max = maxValue
    }

    function clearAllWords() {
        barSeries.clear()
    }
}

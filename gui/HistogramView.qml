import "colorizer.js" as Colorizer
import QtCharts 2.2

ChartView {
    property int countBar: 15
    property var barText

    id: histogramView

    HorizontalBarSeries {
        id: barSeries

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
        axisX: ValueAxis { id: valueAxis; labelFormat: "%d"; max: 5000}
    }

    function changeWordCount(label, value) {
        if (barSeries.count < countBar) {
            barSeries.append(label, [value])
            return
        }
        var less = [0, barSeries.at(0).values[0]]
        for (var i = 0; i < barSeries.count; i++) {
            if (barSeries.at(i).label === label) {
                barSeries.at(i).remove(0);
                barSeries.at(i).append(value);
                return
            }
            if (barSeries.at(i).values[0] < less[1])
                less = [i, barSeries.at(i).values[0]]
        }
        if (less[1] < value) {
            barSeries.remove(barSeries.at(less[0]))
            for (var j = 0; j < barSeries.count; j++) {
                if(barSeries.at(j).label > label) {
                    barSeries.insert(j, label, [value])
                    return
                }
            }
            barSeries.append(label, [value])
        }
    }

    function changeWordCountbyMap(map) {

        console.log("before = ", barSeries.count)

//        for (var i = 0; i < barSeries.count; i++) {
//            barSeries.clear()
//        }
        console.log("after = ", barSeries.count)

        for (var prop in map) {
            var value = map[prop]
            barSeries.insert(barSeries.count, prop, [value])
//            console.log("Object item:", prop, "=", map[prop])
        }
    }

    function rescale() {
        var max = 0
        for (var i = 0; i < barSeries.count; i++) {
            var count = barSeries.at(i).values[0]
            if (count > max)
                max = count
        }


        if (valueAxis.max !== max)
            valueAxis.max = max;
    }

    function colorize() {
        for (var i = 0; i < barSeries.count; i++) {
            barSeries.at(i).color = Colorizer.colorizer(i)
        }
    }

    function clearAllWords() {
        barSeries.clear()
    }
}

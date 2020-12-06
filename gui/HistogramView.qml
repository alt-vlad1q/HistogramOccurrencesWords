import QtQml 2.11
import QtQuick 2.11
import QtCharts 2.2
import QtQuick.Controls 2.3

ChartView {
    property int countBar: 15

    id: histogramView
    anchors.fill: parent
    antialiasing: false


    HorizontalBarSeries {
        id: barSeries
        barWidth: 1
        axisY: BarCategoryAxis { categories: [" "] }
        axisX: ValueAxis { id: valueAxis; }
    }

    function changeWordCount(label, value) {
        if (barSeries.count < countBar)
            barSeries.insert(barSeries.count + 1, label, [value])
        for (var i = 0; i < barSeries.count; i++) {
            if (barSeries.at(i).label === label) {
                barSeries.at(i).remove(0);
                barSeries.at(i).append(value);
                break
            }
        }
        rescale();
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
}

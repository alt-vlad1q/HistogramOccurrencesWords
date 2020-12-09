function makeStringByInterval(interval) {
    return "<i>" + (interval[0] ? interval[0] + qsTr("h ") : "")
            + (interval[1] ? interval[1] + qsTr("m ") : "")
            + (interval[2] ? interval[2] + qsTr("s ") : "")
            + (interval[3] ? interval[3] + qsTr("ms") : "") + "</i>"
}

function convertMs (timeMs) {
    var ms = timeMs % 1000
    var i = Math.floor(timeMs / 1000)
    var hours = Math.floor(i / 60 / 60)
    var minutes = Math.floor(i / 60) - (hours * 60)
    var seconds = i % 60
    return [hours, minutes, seconds, ms]
}

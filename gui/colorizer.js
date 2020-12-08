function colorizer(count) {
    var colors = [
                    Qt.rgba(0.98, 0.25, 0.27, 1),
                    Qt.rgba(0.34, 0.46, 0.56, 1),
                    Qt.rgba(0.95, 0.45, 0.17, 1),
                    Qt.rgba(0.26, 0.67, 0.55, 1),
                    Qt.rgba(0.97, 0.59, 0.12, 1),
                    Qt.rgba(0.56, 0.75, 0.43, 1),
                    Qt.rgba(0.13, 0.34, 0.48, 1),
                    Qt.rgba(0.98, 0.78, 0.31, 1),
                    Qt.rgba(0.31, 0.66, 0.87, 1),
                    Qt.rgba(0.74, 0.28, 0.29, 1),
                    Qt.rgba(0.95, 0.33, 0.18, 1),
                    Qt.rgba(0.33, 0.28, 0.55, 1),
                    Qt.rgba(0.61, 0.40, 0.27, 1),
                    Qt.rgba(0.07, 0.45, 0.46, 1),
                    Qt.rgba(0.98, 0.86, 0.36, 1),
                 ];
    return colors[count % colors.length]
}

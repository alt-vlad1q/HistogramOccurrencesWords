set(TARGET HistogramOccurrencesWordsCore)

add_library(${TARGET} SHARED
    file-separator.cpp
    worker-pool.cpp
    accumulator.cpp
    facrory-task/factory-pure-task.cpp
    facrory-task/factory-grammar-task.cpp
)

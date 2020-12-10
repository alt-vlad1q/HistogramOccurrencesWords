set(TARGET HistogramOccurrencesWordsCore)

find_package(Boost COMPONENTS iostreams REQUIRED)

add_library(${TARGET} SHARED
    file-separator.cpp
    worker-pool.cpp
    accumulator.cpp
    facrory-task/factory-pure-task.cpp
    facrory-task/factory-grammar-task.cpp
)

target_link_libraries(${TARGET} PRIVATE ${Boost_LIBRARIES})

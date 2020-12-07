#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

namespace multi_index {

struct Element {
    std::string word;
    size_t count = 0;

    Element(const std::string &word_)
        : word(word_)
    {};

    struct ByWord{};
    struct ByCount{};
};

using Container =
    boost::multi_index_container<Element, boost::multi_index::indexed_by<
    boost::multi_index::hashed_unique<boost::multi_index::tag<Element::ByWord>, boost::multi_index::member<Element,std::string, &Element::word>>,
    boost::multi_index::ordered_non_unique<boost::multi_index::tag<Element::ByCount>, boost::multi_index::member<Element,size_t, &Element::count>>
    >
>;

} // multi_index


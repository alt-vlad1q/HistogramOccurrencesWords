#include "factory-pure-task.hpp"

#include <sstream>

namespace core { namespace factory {

FactoryTaskBase::task_type FactoryPureTask::generateTask(const std::string &chunk,
                                                         const acc_type &accumulator) const
{
    auto task = [chunk, accumulator](){
        result_type words;
        std::istringstream stream (chunk);
        for(std::string word; stream >> word;) {
            auto result = words.insert({word, {1}});
            if (!result.second) {
                ++(result.first->second);
            }
        }
        accumulator(words);
    };

    return task;
}

const std::string FactoryPureTask::separator() const
{
    return {" "};
}

}} // core // factory

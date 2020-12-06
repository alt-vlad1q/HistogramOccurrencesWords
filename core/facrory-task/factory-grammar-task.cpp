#include "factory-grammar-task.hpp"

#include <sstream>

namespace core { namespace factory {

FactoryTaskBase::task_type FactoryGrammarTask::generateTask(const std::string &chunk,
                                                            const acc_type &accumulator) const
{
    auto task = [chunk, accumulator](){
        result_type words;
        std::istringstream stream (chunk);
        for (std::string word; std::getline(stream, word, '\n');) {
            for (auto c : word) {
                if (ispunct(c)) {
                    word.erase(std::remove(word.begin(), word.end(), c), word.end());
                }
            }
            auto result = words.insert({word, {1}});
            if (!result.second) {
                ++(result.first->second);
            }
        }
        accumulator(words);
    };

    return task;
}

const std::string FactoryGrammarTask::separator() const
{
    return {"\n"};
}

}} // core // factory

#pragma once

#include <string>
#include <functional>

namespace core { namespace factory {

//! \brief The FactoryTaskBase class - Интерфейс генерации задач (для WorkerPool)
//! с последующей отправкой на слияние (в Accumulator)
class FactoryTaskBase {
public:
    using task_type = std::function<void()>;
    using result_type = std::unordered_map<std::string, size_t>;
    using acc_type = std::function<void(result_type)>;

    FactoryTaskBase () = default;
    virtual ~FactoryTaskBase() = default;

    virtual task_type generateTask(const std::string &chunk,
                                   const acc_type &accumulator) const = 0;
    virtual const std::string separator() const = 0;
};

}} // core // factory

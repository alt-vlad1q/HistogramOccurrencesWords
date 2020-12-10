#pragma once

#include "factory-task-base.hpp"

namespace core { namespace factory {

//! \brief The FactoryGrammarTask final class
//! \details Делит блок на строки и удаляет символы пунктуации
class FactoryGrammarTask final : public FactoryTaskBase {
public:
    FactoryGrammarTask() = default;
    ~FactoryGrammarTask() = default;

    task_type generateTask(const std::string &chunk,
                           const acc_type &accumulator) const override;
    const std::string separator() const override;;
};

}} // core // factory

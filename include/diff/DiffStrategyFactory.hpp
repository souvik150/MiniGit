//
// Created by souvik on 11/1/25.
//

#ifndef MINIGIT_DIFFSTRATEGYFACTORY_HPP
#define MINIGIT_DIFFSTRATEGYFACTORY_HPP

#include "DiffStrategy.hpp"
#include "DiffStrategyType.hpp"

class DiffStrategyFactory {
public:
    static std::shared_ptr<IDiffStrategy> getStrategy(DiffStrategyType type);
};
#endif //MINIGIT_DIFFSTRATEGYFACTORY_HPP
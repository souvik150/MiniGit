//
// Created by souvik on 11/1/25.
//
#include "DiffStrategyFactory.hpp"

#include <stdexcept>

std::shared_ptr<IDiffStrategy> DiffStrategyFactory::getStrategy(DiffStrategyType type) {
    switch(type) {
        case DiffStrategyType::Line:
            return std::make_shared<LineDiffStrategy>();
        case DiffStrategyType::Word:
            return std::make_shared<WordDiffStrategy>();
        default:
            throw std::invalid_argument("Unknown diff strategy type");
    }
}

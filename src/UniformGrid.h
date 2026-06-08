#pragma once

#include "Container.h"

namespace fc {
class UniformGrid : public Container {
public:
    UniformGrid(alignment::ElementAlignment alignment, unsigned int columns, unsigned int rows) : Container(alignment) {
        for (unsigned int y = 0; y < rows; ++y) {
            _cells.emplace_back();
            for (unsigned int x = 0; x < columns; ++x) {
                auto child = createIDedChild<Container>(alignment::ElementAlignment(
                    alignment::Relative(x * 1.0f / columns), alignment::Relative(y * 1.0f / rows),
                    alignment::Relative(1.0f / columns), alignment::Relative(1.0f / rows)));
                
                _cells[y].push_back(child.second);
            }
        }
    }

    Container& at(unsigned int column, unsigned int row) const {
        if (row >= _cells.size() || column >= _cells[row].size()) {
            throw std::out_of_range("Column or row index out of range");
        }
        return static_cast<Container&>(*children()[_cells[row][column]]);   
    }

private:
    std::vector<std::vector<fiv::ID>> _cells;
};
} // namespace fc
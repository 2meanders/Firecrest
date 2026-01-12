#pragma once

#include "Container.h"

namespace fc {
    class HorisontalPositioning : public Container {
    public:
        enum class Direction {
            LeftToRight,
            RightToLeft,
        };
        Direction direction;

        float spacing;

        HorisontalPositioning(alignment::ElementAlignment alignment) : Container(alignment), spacing(0.0f), direction(Direction::LeftToRight) {}
        
        void childCreated(fiv::ID id) override {
			children()[id]->alignment.x = alignment::AlignmentFunction(
				[id, this](float parent1, float parent2) -> float {
					float xOffset = 0.0f;
					auto index = children().indexOf(id);
					for (size_t i = 0; i < index; i++) {
						const float childWidth = children().dataAt(i)->getPixelSize().x;
						xOffset += childWidth + (childWidth > 0 ? spacing : 0);
					}

					if (direction == Direction::LeftToRight) {
						return xOffset;
					} else {
						return parent1 - xOffset - children()[id]->getPixelSize().x;
					}
				}
			);
		}
    };
}
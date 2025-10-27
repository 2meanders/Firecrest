#pragma once
#include "Container.h"

namespace fc {
	class VerticalPositioning : public Container {
	public:
		// Enum to specify the direction of vertical positioning
		// TopToBottom: The first child is positioned at the top and the last child at the bottom.
		// BottomToTop: The first child is positioned at the bottom and the last child at the top.
		enum class Direction {
			TopToBottom,
			BottomToTop
		};
		Direction m_Direction;

		float m_Spacing;

		VerticalPositioning(alignment::ElementAlignment alignment, Direction direction) : Container(alignment), m_Direction(direction), m_Spacing(0.0f) {}
		VerticalPositioning(alignment::ElementAlignment alignment) : VerticalPositioning(alignment, Direction::TopToBottom) {}

		void setSpacing(float spacing) {
			m_Spacing = spacing;
		}

		void childCreated(fiv::ID id) override {
			children()[id]->alignment.y = alignment::AlignmentFunction(
				[id, this](float parent1, float parent2) -> float {
					float yOffset = 0.0f;
					auto index = children().indexOf(id);
					for (size_t i = 0; i < index; i++) {
						float childHeight = children().dataAt(i)->getPixelSize().y;
						yOffset += childHeight + (childHeight > 0 ? m_Spacing : 0);
					}

					if (m_Direction == Direction::BottomToTop) {
						return yOffset;
					} else {
						return parent1 - yOffset - children()[id]->getPixelSize().y;
					}
				}
			);
		}
	};
}
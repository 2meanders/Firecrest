#pragma once
#include "VerticalPositioning.h"
#include "alignment/Alignment.h"
#include <vector>
#include <iterator>
#include <algorithm>

namespace fc {
	class VerticalContainer : public VerticalPositioning {
	public:
		VerticalContainer(alignment::ElementAlignment alignment) : VerticalPositioning(alignment) {}
    public:
        void setFlexibleChildren(std::vector<fiv::ID> flexibleChildren) {
            alignment::AlignmentFunction alignment(
                [flexibleChildren, this](float parent1, float parent2) {
                    float totalSize = 0.0f;
                    size_t index = 0;
                    for (int i = 0; i < children().size(); i++) {
                        const auto& child = children().dataAt(i);
                        if (std::find(flexibleChildren.begin(), flexibleChildren.end(), children().idAt(i)) != flexibleChildren.end()) {
                            index++;
                            continue;
                        }

                        float childSize = child->getPixelSize().y;
                        totalSize += childSize;
                        if (childSize > 0) {
                            totalSize += m_Spacing;
                        }
                        index++;
                    }
                    float remainingSize = parent1 - totalSize;
                    float sizePerChild = remainingSize / flexibleChildren.size();

                    return sizePerChild;
                }
            );

            // Apply the alignment function
            for (fiv::ID id : flexibleChildren) {
                children()[id]->alignment.height = alignment;
            }
        }

	};
}
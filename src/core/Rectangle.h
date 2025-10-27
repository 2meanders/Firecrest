#pragma once

#include "glm/glm.hpp"

namespace fc {

	struct Rectangle {
		union {
			struct {
				float x;
				float y;
			};
			glm::vec2 position;
		};
		union {
			struct {
				float width;
				float height;
			};
			glm::vec2 size;
		};
	
		Rectangle(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}
		Rectangle(const glm::vec2& position, const glm::vec2& size) : position(position), size(size) {}
		
		bool contains(const glm::vec2& point) const {
			return point.x >= x && point.x <= x + width &&
			point.y >= y && point.y <= y + height;
		}
		
		// If they intersect, return the intersection rectangle, otherwise return a rectangle with zero size
		Rectangle intersection(const Rectangle& other) const {
			float x1 = std::max(x, other.x);
			float y1 = std::max(y, other.y);
			float x2 = std::min(x + width, other.x + other.width);
			float y2 = std::min(y + height, other.y + other.height);
			
			if (x1 < x2 && y1 < y2) {
				return Rectangle(x1, y1, x2 - x1, y2 - y1);
			}
			return Rectangle(0, 0, 0, 0); // No intersection
		}
		
		// Check if this rectangle intersects with another rectangle
		bool intersects(const Rectangle& other) const {
			return !(other.x > x + width || 
					other.x + other.width < x || 
					other.y > y + height || 
					other.y + other.height < y);
		}
	};
}

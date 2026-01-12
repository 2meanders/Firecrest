#pragma once
#include "Element.h"
#include <memory>
#include <utility>
#include "fiv.hpp"

namespace fc {

    // An element that can contain other elements.
    class Container : public Element {
    private:
        fiv::Vector<std::unique_ptr<Element>> _children;
    protected:
		// This function is called immediately after a child is created.
		virtual void childCreated(fiv::ID id) {}
    public:
        Container(alignment::ElementAlignment alignment) : Element(alignment), _children(true) {}
        
        template<typename T, typename... Args>
        T& createChild(Args&&... args) {
            static_assert(std::is_base_of<Element, T>::value, "T must derive from Element");
            fiv::ID id = _children.emplace(std::make_unique<T>(std::forward<Args>(args)...));
            _children[id]->_parent = this;
            childCreated(id);
            return static_cast<T&>(*_children[id]);
        }

        template<typename T, typename... Args>
        std::pair<T&, fiv::ID> createIDedChild(Args&&... args) {
            static_assert(std::is_base_of<Element, T>::value, "T must derive from Element");
            fiv::ID id = _children.emplace(std::make_unique<T>(std::forward<Args>(args)...));
            _children[id]->_parent = this;
            childCreated(id);
            return { static_cast<T&>(*_children[id]), id };
        }
        
        const fiv::Vector<std::unique_ptr<Element>>& children() const {
            return _children;
        }

        virtual void render(const Window& window, time::Duration delta) override {
            for (const auto& child : children()) {
                child->render(window, delta);
            }
        }

        virtual void onMouseMotionEvent(Input& input, input::MouseMotionEvent event) override {
            for (auto& child : children()) {
                const Rectangle childRect = child->getPixelRectangle();

                const bool wasInside = childRect.contains(event.lastPosition);
                const bool isInside = childRect.contains(event.position);

                if (wasInside && isInside)
                    child->onMouseMotionEvent(input, {input::MouseMotionAction::Move, event.position, event.lastPosition});
                else if (wasInside && !isInside)
                    child->onMouseMotionEvent(input, {input::MouseMotionAction::Exit, event.position, event.lastPosition});
                else if (!wasInside && isInside)
                    child->onMouseMotionEvent(input, {input::MouseMotionAction::Enter, event.position, event.lastPosition});
            }
        }
        
        virtual void onMouseButtonEvent(Input& input, input::MouseButtonEvent event) override {
            const glm::vec2 mousePos = input.mouse();
            for (auto& child : children())
            {
                const Rectangle childRect = child->getPixelRectangle();
                
                if (!childRect.contains(mousePos))
                    continue;
                
                child->onMouseButtonEvent(input, event);
            }
        }

        virtual void onScroll(Input& input, input::ScrollEvent event) override {
            const glm::vec2 mousePos = input.mouse();
            for (auto& child : children()) {
                const Rectangle childRect = child->getPixelRectangle();
                
                if(!childRect.contains(mousePos)) continue;

                child->onScroll(input, event);
            }
        }

        virtual Element* findFocusedElement(glm::vec2 mousePosition) override {
            for (auto& child : _children) {
                Rectangle childRect = child->getPixelRectangle();
                if (childRect.contains(mousePosition)) {
                    Element* focusedChild = child->findFocusedElement(mousePosition);
                    if (focusedChild != nullptr) {
                        return focusedChild;
                    }
                }
            }
            return Element::findFocusedElement(mousePosition); // No child is focused, so check self
        }

        virtual int32_t maxDepth() const override {
            int32_t depth = calculateDepth();
            for(const auto& child : _children) {
                depth = std::min(depth, child->maxDepth());
			}
			return depth;
        }

        virtual bool lockMouse(Element* requestingElement) {
            return parent().lockMouse(requestingElement);
        }

        virtual bool unlockMouse(Element* requestingElement) {
            return parent().unlockMouse(requestingElement);
        }

        virtual glm::vec2 calculateChildPixelPosition(const alignment::ElementAlignment& childAlignment) const {
            return getPixelPosition() + childAlignment.getPixelPosition(getPixelSize());
        }

        virtual glm::vec2 calculateChildPixelSize(const alignment::ElementAlignment& childAlignment) const {
            return childAlignment.getPixelSize(getPixelSize());
        }
    };
}

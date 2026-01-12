#pragma once
#include "Window.h"
#include "glm/glm.hpp"
#include "alignment/ElementAlignment.h"
#include "input/ElementEvents.h"
#include "core/Rectangle.h"
#include "core/Time.h"

namespace fc {
    class Container;

    // The base class for all UI elements.
    class Element {
    public:
    
        virtual void render(const Window& window, time::Duration delta) {}
        
        // Input
        virtual void onMouseMotionEvent(Input& input, input::MouseMotionEvent event) {}
        virtual void onMouseButtonEvent(Input& input, input::MouseButtonEvent event) {}
        virtual void onKeyboardEvent(Input& input, input::KeyboardEvent event) {}
        virtual void onScroll(Input& input, input::ScrollEvent event) {} 
        virtual void onLetterTyped(Input& input, input::UnicodeCodePoint letter) {}

        virtual void onFocusAquired() {}
        virtual void onFocusLost() {}
    protected:
        // Lock the mouse to this element, so that it will not move outside of this element.
        // Returns true if the mouse was successfully locked, false otherwise.
        // Note: only the focused element can lock the mouse.
        bool lockMouse();
        // Unlock the mouse from this element, allowing it to move freely again.
        // Returns true if the mouse was successfully unlocked, false otherwise.
        // Note: only the focused element can unlock the mouse.
        bool unlockMouse();
    
    protected:
        Container* _parent;
    
        const Container& parent() const { return *_parent; }
        Container& parent() { return *_parent; }

        // Whether this element can be focused or not.
        bool focusable;
        
    public:
        alignment::ElementAlignment alignment;
        
        Element(alignment::ElementAlignment alignment);
        
        virtual glm::vec2 getPixelPosition() const;
        virtual glm::vec2 getPixelSize() const;
        
        Rectangle getPixelRectangle() const;
        
        // This is only called when the mouse is over this element, 
        // so there is no need to check if the mouse position is within the element's rectangle.
        virtual Element* findFocusedElement(glm::vec2 mousePosition) {
            if(focusable) {
                return this;
            } else {
                return nullptr;
            }
        }
        
        // Call when an element wishes to stop being focused
        virtual void unFocus();
        
        // Note! the returned number is negative
        virtual int32_t calculateDepth() const;
        
        bool hasFocus() const {
            return _hasFocus;
        }

    private:
        bool _hasFocus;

        virtual int32_t maxDepth() const {
			return calculateDepth();
        }

        friend class Container;
        friend class Display;
    };

}

#pragma once
#include "Element.h"
#include "Container.h"
#include "Renderer.h"
#include <vector>
#include "gl/RenderRegion.h"
#include "core/Time.h"

namespace fc {

    // The root element of the UI tree.
    class Display : public Container {
    private:
        Element* _focusedElement;
        Window& _window;
        // vector<renderPassNumber, renderer>
		std::vector<std::unique_ptr<Renderer>> _renderers;

		fiv::ID _keySubscription;
		fiv::ID _charSubscription;
		fiv::ID _mouseMotionSubscription;
		fiv::ID _mouseButtonSubscription;
        fiv::ID _scrollSubscription;

        time::Moment _lastRenderTime;
    public:
        Display(Window& window) : 
            Container(alignment::ElementAlignment()), 
            _window(window),
            _focusedElement(nullptr),
            _lastRenderTime(time::now())
        {
            glEnable(GL_SCISSOR_TEST);

            _keySubscription = _window.getInput().subscripeKeyEvent([this](input::RawKeyboardEvent event) { keyCallback(event); });
            _charSubscription = _window.getInput().subscribeCharEvent([this](input::UnicodeCodePoint letter) { charCallback(letter); });
            _mouseMotionSubscription = _window.getInput().subscribeMouseMotionEvent([this](input::RawMouseMotionEvent event) { mouseMotionCallback(event); });
            _mouseButtonSubscription = _window.getInput().subscribeMouseButtonEvent([this](input::RawMouseButtonEvent event) { mouseButtonCallback(event); });
            _scrollSubscription = _window.getInput().subscribeScrollEvent([this](input::RawScrollEvent event) { scrollCallback(event); });
        }

        ~Display() {
            _window.getInput().unsubscribeKeyEvent(_keySubscription);
            _window.getInput().unsubscribeCharEvent(_charSubscription);
            _window.getInput().unsubscribeMouseMotionEvent(_mouseMotionSubscription);
			_window.getInput().unsubscribeMouseButtonEvent(_mouseButtonSubscription);
            _window.getInput().unsubscribeScrollEvent(_scrollSubscription);
        }

        template<typename T, typename... Args>
        T& createRenderer(Args&&... args) {
            static_assert(std::is_base_of<Renderer, T>::value, "T must derive from Renderer");
            _renderers.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			return static_cast<T&>(*_renderers.back().get());
        }
        
        glm::vec2 getPixelPosition() const override {
            return alignment.getPixelPosition(static_cast<glm::vec2>(_window.dimensions()));
        }
        glm::vec2 getPixelSize() const override {
            return alignment.getPixelSize(static_cast<glm::vec2>(_window.dimensions()));
        }
        
        void render() {
            time::Moment now = time::now();
            time::Duration delta = now - _lastRenderTime;
            _lastRenderTime = now;

            render(_window, delta);
		}

        void render(const Window& window, time::Duration delta) override {
            glDisable(GL_DEPTH_TEST);

            // Reset the viewport and scissor
            gl::RenderRegion::push({ getPixelPosition(), getPixelSize() });

            for(uint32_t i = 0; i < _renderers.size(); i++) {
                _renderers[i]->beforeRender(window);
			}

            Container::render(window, delta);

            for (uint32_t i = 0; i < _renderers.size(); i++) {
                _renderers[i]->afterRender(window);
            }
            
            gl::RenderRegion::pop();
        }

        virtual void unFocus() override {
            if(_focusedElement == nullptr) return;

            _focusedElement->_hasFocus = false;
            _focusedElement->onFocusLost();
            _focusedElement = nullptr;
        }

        virtual int32_t calculateDepth() const override {
            return 0;
        }
    private:

        // Unfocuses the current focused element, if any.
        // Then focuses on the given element, if it is not null.
        void focusOn(Element* element) {
            unFocus();
            
            if(element == nullptr) return;
            
            _focusedElement = element;
            _focusedElement->_hasFocus = true;
            _focusedElement->onFocusAquired();
        }

        void keyCallback(input::RawKeyboardEvent event) {
            if (_focusedElement == nullptr) return;
            input::KeyboardEvent e = static_cast<input::KeyboardEvent>(event);
            _focusedElement->onKeyboardEvent(_window.getInput(), e);
        }

        void charCallback(input::UnicodeCodePoint letter) {
            if (_focusedElement == nullptr) return;
            _focusedElement->onLetterTyped(_window.getInput(), letter);
        }
        
        void mouseButtonCallback(input::RawMouseButtonEvent event) {
            const bool mouseIsLocked = _window.isMouseLocked();
            const glm::vec2 mousePos = _window.getInput().mouse();
            
            if(!mouseIsLocked) {
                for (auto& child : children()) {
                    const Rectangle childRect = child->getPixelRectangle();

                    if (!childRect.contains(mousePos))
                        continue;
                    
                    input::MouseButtonEvent e {
                        static_cast<input::MouseButton>(event.button), 
                        static_cast<input::MouseButtonAction>(event.action), 
                        event.mods
                    };
                    child->onMouseButtonEvent(_window.getInput(), e);
                }
            } else if (_focusedElement != nullptr) {
                // If the mouse is locked, we only send the event to the focused element
                input::MouseButtonEvent e {
                    static_cast<input::MouseButton>(event.button), 
                    static_cast<input::MouseButtonAction>(event.action), 
                    event.mods
                };
                _focusedElement->onMouseButtonEvent(_window.getInput(), e);

            }
                
            // Find the focused element
            if (event.button == input::RawMouseButton::Left && event.action == input::RawMouseButtonAction::Up) {
                Element* newFocus = findFocusedElement(mousePos);
                focusOn(newFocus);
            }
		}
        
        void mouseMotionCallback(input::RawMouseMotionEvent event) {
            const bool mouseIsLocked = _window.isMouseLocked();

            if (!mouseIsLocked) {
                for (auto& child : children()) {
                    const Rectangle childRect = child->getPixelRectangle();
                    
                    const bool wasInside = childRect.contains(event.lastPosition);
                    const bool isInside = childRect.contains(event.position);
                    
                    if (wasInside && isInside)
                        child->onMouseMotionEvent(_window.getInput(), {input::MouseMotionAction::Move, event.position, event.lastPosition});
                    else if (wasInside && !isInside)
                        child->onMouseMotionEvent(_window.getInput(), {input::MouseMotionAction::Exit, event.position, event.lastPosition});
                    else if (!wasInside && isInside)
                       child->onMouseMotionEvent(_window.getInput(), {input::MouseMotionAction::Enter, event.position, event.lastPosition});
                }
            } else if (_focusedElement != nullptr) {
                // If the mouse is locked, we only send the event to the focused element
                _focusedElement->onMouseMotionEvent(_window.getInput(), {input::MouseMotionAction::Move, event.position, event.lastPosition});
            }
        }

        void scrollCallback(input::RawScrollEvent event) {
            const bool mouseIsLocked = _window.isMouseLocked();
            if(!mouseIsLocked) {
                input::ScrollEvent e = static_cast<input::ScrollEvent>(event);
                Container::onScroll(_window.getInput(), e);
            } else if (_focusedElement != nullptr) {
                // If the mouse is locked, we only send the event to the focused element
                input::ScrollEvent e = static_cast<input::ScrollEvent>(event);
                _focusedElement->onScroll(_window.getInput(), e);
            }
        }
    public:
        virtual bool lockMouse(Element* requestingElement) override {
            if (_focusedElement == nullptr || _focusedElement != requestingElement || _window.isMouseLocked()) {
                return false;
            }
            _window.lockMouse();
            return true;
        }

        virtual bool unlockMouse(Element* requestingElement) override {
            if (_focusedElement == nullptr || _focusedElement != requestingElement || _window.isMouseFree()) {
                return false;
            }
            _window.freeMouse();
            return true;
        }

    };
}

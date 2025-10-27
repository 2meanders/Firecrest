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
        Element* m_FocusedElement;
        Window& m_Window;
        // vector<renderPassNumber, renderer>
		std::vector<std::unique_ptr<Renderer>> m_Renderers;

		fiv::ID keySubscription;
		fiv::ID charSubscription;
		fiv::ID mouseMotionSubscription;
		fiv::ID mouseButtonSubscription;
        fiv::ID scrollSubscription;

        time::Moment lastRenderTime;
    public:
        Display(Window& window) : 
            Container(alignment::ElementAlignment()), 
            m_Window(window),
            m_FocusedElement(nullptr),
            lastRenderTime(time::now())
        {
            glEnable(GL_SCISSOR_TEST);

            keySubscription = m_Window.getInput().subscripeKeyEvent([this](input::RawKeyboardEvent event) { keyCallback(event); });
            charSubscription = m_Window.getInput().subscribeCharEvent([this](input::UnicodeCodePoint letter) { charCallback(letter); });
            mouseMotionSubscription = m_Window.getInput().subscribeMouseMotionEvent([this](input::RawMouseMotionEvent event) { mouseMotionCallback(event); });
            mouseButtonSubscription = m_Window.getInput().subscribeMouseButtonEvent([this](input::RawMouseButtonEvent event) { mouseButtonCallback(event); });
            scrollSubscription = m_Window.getInput().subscribeScrollEvent([this](input::RawScrollEvent event) { scrollCallback(event); });
        }

        ~Display() {
            m_Window.getInput().unsubscribeKeyEvent(keySubscription);
            m_Window.getInput().unsubscribeCharEvent(charSubscription);
            m_Window.getInput().unsubscribeMouseMotionEvent(mouseMotionSubscription);
			m_Window.getInput().unsubscribeMouseButtonEvent(mouseButtonSubscription);
            m_Window.getInput().unsubscribeScrollEvent(scrollSubscription);
        }

        template<typename T, typename... Args>
        T& createRenderer(Args&&... args) {
            static_assert(std::is_base_of<Renderer, T>::value, "T must derive from Renderer");
            m_Renderers.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			return static_cast<T&>(*m_Renderers.back().get());
        }
        
        glm::vec2 getPixelPosition() const override {
            return alignment.getPixelPosition(static_cast<glm::vec2>(m_Window.dimensions()));
        }
        glm::vec2 getPixelSize() const override {
            return alignment.getPixelSize(static_cast<glm::vec2>(m_Window.dimensions()));
        }
        
        void render() {
            time::Moment now = time::now();
            time::Duration delta = now - lastRenderTime;
            lastRenderTime = now;

            render(m_Window, delta);
		}

        void render(const Window& window, time::Duration delta) override {
            glDisable(GL_DEPTH_TEST);

            // Reset the viewport and scissor
            gl::RenderRegion::push({ getPixelPosition(), getPixelSize() });

            for(uint32_t i = 0; i < m_Renderers.size(); i++) {
                m_Renderers[i]->beforeRender(window);
			}

            Container::render(window, delta);

            for (uint32_t i = 0; i < m_Renderers.size(); i++) {
                m_Renderers[i]->afterRender(window);
            }
            
            gl::RenderRegion::pop();
        }

        virtual void unFocus() override {
            if(m_FocusedElement == nullptr) return;

            m_FocusedElement->_hasFocus = false;
            m_FocusedElement->onFocusLost();
            m_FocusedElement = nullptr;
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
            
            m_FocusedElement = element;
            m_FocusedElement->_hasFocus = true;
            m_FocusedElement->onFocusAquired();
        }

        void keyCallback(input::RawKeyboardEvent event) {
            if (m_FocusedElement == nullptr) return;
            input::KeyboardEvent e = static_cast<input::KeyboardEvent>(event);
            m_FocusedElement->onKeyboardEvent(m_Window.getInput(), e);
        }

        void charCallback(input::UnicodeCodePoint letter) {
            if (m_FocusedElement == nullptr) return;
            m_FocusedElement->onLetterTyped(m_Window.getInput(), letter);
        }
        
        void mouseButtonCallback(input::RawMouseButtonEvent event) {
            const bool mouseIsLocked = m_Window.isMouseLocked();
            const glm::vec2 mousePos = m_Window.getInput().mouse();
            
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
                    child->onMouseButtonEvent(m_Window.getInput(), e);
                }
            } else if (m_FocusedElement != nullptr) {
                // If the mouse is locked, we only send the event to the focused element
                input::MouseButtonEvent e {
                    static_cast<input::MouseButton>(event.button), 
                    static_cast<input::MouseButtonAction>(event.action), 
                    event.mods
                };
                m_FocusedElement->onMouseButtonEvent(m_Window.getInput(), e);

            }
                
            // Find the focused element
            if (event.button == input::RawMouseButton::Left && event.action == input::RawMouseButtonAction::Up) {
                Element* newFocus = findFocusedElement(mousePos);
                focusOn(newFocus);
            }
		}
        
        void mouseMotionCallback(input::RawMouseMotionEvent event) {
            const bool mouseIsLocked = m_Window.isMouseLocked();

            if (!mouseIsLocked) {
                for (auto& child : children()) {
                    const Rectangle childRect = child->getPixelRectangle();
                    
                    const bool wasInside = childRect.contains(event.lastPosition);
                    const bool isInside = childRect.contains(event.position);
                    
                    if (wasInside && isInside)
                        child->onMouseMotionEvent(m_Window.getInput(), {input::MouseMotionAction::Move, event.position, event.lastPosition});
                    else if (wasInside && !isInside)
                        child->onMouseMotionEvent(m_Window.getInput(), {input::MouseMotionAction::Exit, event.position, event.lastPosition});
                    else if (!wasInside && isInside)
                       child->onMouseMotionEvent(m_Window.getInput(), {input::MouseMotionAction::Enter, event.position, event.lastPosition});
                }
            } else if (m_FocusedElement != nullptr) {
                // If the mouse is locked, we only send the event to the focused element
                m_FocusedElement->onMouseMotionEvent(m_Window.getInput(), {input::MouseMotionAction::Move, event.position, event.lastPosition});
            }
        }

        void scrollCallback(input::RawScrollEvent event) {
            const bool mouseIsLocked = m_Window.isMouseLocked();
            if(!mouseIsLocked) {
                input::ScrollEvent e = static_cast<input::ScrollEvent>(event);
                Container::onScroll(m_Window.getInput(), e);
            } else if (m_FocusedElement != nullptr) {
                // If the mouse is locked, we only send the event to the focused element
                input::ScrollEvent e = static_cast<input::ScrollEvent>(event);
                m_FocusedElement->onScroll(m_Window.getInput(), e);
            }
        }
    public:
        virtual bool lockMouse(Element* requestingElement) override {
            if (m_FocusedElement == nullptr || m_FocusedElement != requestingElement || m_Window.isMouseLocked()) {
                return false;
            }
            m_Window.lockMouse();
            return true;
        }

        virtual bool unlockMouse(Element* requestingElement) override {
            if (m_FocusedElement == nullptr || m_FocusedElement != requestingElement || m_Window.isMouseFree()) {
                return false;
            }
            m_Window.freeMouse();
            return true;
        }

    };
}

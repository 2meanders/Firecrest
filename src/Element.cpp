#include "Element.h"
#include "Container.h"

fc::Element::Element(alignment::ElementAlignment alignment) 
    : alignment(alignment)
    , m_Parent(nullptr)
    , _hasFocus(false) 
    , focusable(false)
{}

glm::vec2 fc::Element::getPixelPosition() const {
    return parent().calculateChildPixelPosition(alignment);
}

glm::vec2 fc::Element::getPixelSize() const {
    return parent().calculateChildPixelSize(alignment);
}

fc::Rectangle fc::Element::getPixelRectangle() const {
    return { getPixelPosition(), getPixelSize() };
}

void fc::Element::unFocus() {
    m_Parent->unFocus();
}

int32_t fc::Element::calculateDepth() const {
    return parent().calculateDepth() - 1;
}

bool fc::Element::lockMouse() {
    return parent().lockMouse(this);
}

bool fc::Element::unlockMouse() {
    return parent().unlockMouse(this);
}
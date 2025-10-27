#include "RenderRegion.h"
#include "OpenGL.h"

void fc::gl::RenderRegion::pushAbsolute(const Rectangle& region) {
    pushAbsolute(region, Mode::All);
}

void fc::gl::RenderRegion::pushAbsolute(const Rectangle& region, Mode mode) {
    stack.emplace_back(region, mode);

    if(static_cast<uint8_t>(mode) & static_cast<uint8_t>(Mode::Viewport)) {
        glViewport( static_cast<GLint>(region.x), static_cast<GLint>(region.y),
                    static_cast<GLsizei>(region.width), static_cast<GLsizei>(region.height));
    }

    if(static_cast<uint8_t>(mode) & static_cast<uint8_t>(Mode::Scissor)) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(  static_cast<GLint>(region.x), static_cast<GLint>(region.y),
                    static_cast<GLsizei>(region.width), static_cast<GLsizei>(region.height));
    }
}

void fc::gl::RenderRegion::push(const Rectangle &region) {
    push(region, Mode::All);
}

void fc::gl::RenderRegion::push(const Rectangle &region, Mode mode) {
    Rectangle region_ = region;

    // Intersect with current viewport if mode includes Viewport
    if (static_cast<uint8_t>(mode) & static_cast<uint8_t>(Mode::Viewport)) {
        region_ = region_.intersection(currentViewport());
    }

    // Intersect with current scissor if mode includes Scissor
    if (static_cast<uint8_t>(mode) & static_cast<uint8_t>(Mode::Scissor)) {
        region_ = region_.intersection(currentScissor());
    }

    stack.emplace_back(region_, mode);

    if (static_cast<uint8_t>(mode) & static_cast<uint8_t>(Mode::Viewport)) {
        glViewport( static_cast<GLint>(region_.x), static_cast<GLint>(region_.y),
                    static_cast<GLsizei>(region_.width), static_cast<GLsizei>(region_.height));
    }

    if (static_cast<uint8_t>(mode) & static_cast<uint8_t>(Mode::Scissor)) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(  static_cast<GLint>(region_.x), static_cast<GLint>(region_.y),
                    static_cast<GLsizei>(region_.width), static_cast<GLsizei>(region_.height));
    }
}

void fc::gl::RenderRegion::pop() {
    if(stack.empty()) {
        return;
    }

    stack.pop_back();

    const Rectangle viewport = currentViewport();
    const Rectangle scissor = currentScissor();

    glViewport( static_cast<GLint>(viewport.x), static_cast<GLint>(viewport.y),
                static_cast<GLsizei>(viewport.width), static_cast<GLsizei>(viewport.height));
    glScissor(  static_cast<GLint>(scissor.x), static_cast<GLint>(scissor.y),
                static_cast<GLsizei>(scissor.width), static_cast<GLsizei>(scissor.height));

}

void fc::gl::RenderRegion::applyBase() {
    glEnable(GL_SCISSOR_TEST);
    glViewport( static_cast<GLint>(base.x), static_cast<GLint>(base.y),
                static_cast<GLsizei>(base.width), static_cast<GLsizei>(base.height));
    glScissor(  static_cast<GLint>(base.x), static_cast<GLint>(base.y),
                static_cast<GLsizei>(base.width), static_cast<GLsizei>(base.height));
}

fc::Rectangle fc::gl::RenderRegion::currentViewport() {
    if(stack.empty()) {
        return base;
    }

    for (int32_t i = stack.size()-1; i >= 0; i--) {
        const auto& [region, mode] = stack[i];

        if(static_cast<uint8_t>(mode) & static_cast<uint8_t>(Mode::Viewport)) {
            return region;
        }
    }
    return base; // If no viewport found in the stack, return base viewport
}

fc::Rectangle fc::gl::RenderRegion::currentScissor() {
    if(stack.empty()) {
        return base;
    }

    for (int32_t i = stack.size()-1; i >= 0; i--) {
        const auto& [region, mode] = stack[i];

        if(static_cast<uint8_t>(mode) & static_cast<uint8_t>(Mode::Scissor)) {
            return region;
        }
    }
    return base; // If no scissor found in the stack, return base scissor
}


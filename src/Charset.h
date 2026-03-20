#include "gl/Texture2D.h"
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

namespace fc {
class Charset {
public:
    struct Glyph {
        glm::vec2 uvMin;
        glm::vec2 uvMax;

        glm::vec2 size;
        glm::vec2 bearing;
        float advance;
    };

    Charset(const std::string& fontFile);

    const Glyph& glyph(char c) const;

    const gl::Texture2D& atlas() const;

    float ascender() const;
    float descender() const;
    float lineHeight() const;

private:
    std::unordered_map<char, Glyph> _glyphs;

    gl::Texture2D _atlas;

    float _ascender;
    float _descender;
    float _lineHeight;
};
} // namespace fc
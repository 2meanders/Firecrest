#pragma once
#include "OpenGL.h"
#include <algorithm>

namespace fc::gl {

struct BufferHandle {
    GLuint id = 0;

    BufferHandle() { glGenBuffers(1, &id); }

    ~BufferHandle()
    {
        if (id) {
            glDeleteBuffers(1, &id);
        }
    }

    BufferHandle(const BufferHandle&) = delete;
    BufferHandle& operator=(const BufferHandle&) = delete;

    BufferHandle(BufferHandle&& o) noexcept : id(o.id) { o.id = 0; }

    BufferHandle& operator=(BufferHandle&& o) noexcept
    {
        if (this != &o) {
            if (id)
                glDeleteBuffers(1, &id);
            id = o.id;
            o.id = 0;
        }
        return *this;
    }

    bool valid() const { return id != 0; }
};

template <GLenum t_Type>
class Buffer {
protected:
    BufferHandle _handle;

    GLsizeiptr m_Size;

public:
    Buffer() = default;

    Buffer& operator=(Buffer&& other) noexcept = default;
    Buffer(Buffer&& other) noexcept = default;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    inline void bind() const { glBindBuffer(t_Type, _handle.id); }

    inline void unbind() const { glBindBuffer(t_Type, 0); }

    void setData(const void* data) { setData(data, 0, m_Size); }

    void editData(const void* data, GLintptr offset, GLsizeiptr size)
    {
        bind();
        glBufferSubData(t_Type, offset, size, data);
        unbind();
    }

    void setData(const void* data, GLsizeiptr size, GLenum usage)
    {
        bind();
        glBufferData(t_Type, size, data, usage);
        m_Size = size;
        unbind();
    }

    void getData(void* dest) const { getData(dest, 0, m_Size); }

    void getData(void* dest, GLintptr offset, GLsizeiptr size) const
    {
        bind();
        glGetBufferSubData(t_Type, offset, size, dest);
        unbind();
    }

    void* dataPointer(GLbitfield access) { return dataPointer(0, m_Size, access); }

    void* dataPointer(GLintptr offset, GLsizeiptr size, GLbitfield access)
    {
        bind();
        void* ptr = glMapBufferRange(t_Type, offset, size, access);
        return ptr;
    }

    void close()
    {
        glUnmapBuffer(t_Type);
        unbind();
    }

    inline BufferHandle getHandle() const { return _handle; }
    inline GLsizeiptr getSize() const { return m_Size; }
};
} // namespace fc::gl
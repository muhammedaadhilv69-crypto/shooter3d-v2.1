#pragma once

#include <string>
#include <glad/glad.h>

namespace fps::renderer {

enum class TextureFormat {
    RGB8,
    RGBA8,
    RGB16F,
    RGBA16F,
    Depth16,
    Depth24,
    Depth32F,
    Depth24Stencil8,
    R8,
    RG8
};

enum class TextureWrap {
    Repeat = GL_REPEAT,
    ClampToEdge = GL_CLAMP_TO_EDGE,
    ClampToBorder = GL_CLAMP_TO_BORDER,
    MirroredRepeat = GL_MIRRORED_REPEAT
};

enum class TextureFilter {
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
    NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
    LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
};

struct TextureConfig {
    TextureFormat format = TextureFormat::RGBA8;
    TextureWrap wrapS = TextureWrap::Repeat;
    TextureWrap wrapT = TextureWrap::Repeat;
    TextureFilter minFilter = TextureFilter::LinearMipmapLinear;
    TextureFilter magFilter = TextureFilter::Linear;
    bool generateMipmaps = true;
    bool anisotropic = true;
    float anisotropyLevel = 4.0f;
};

class Texture2D {
public:
    Texture2D();
    ~Texture2D();
    
    // Non-copyable
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    
    // Movable
    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;
    
    bool LoadFromFile(const std::string& filepath, const TextureConfig& config = TextureConfig{});
    bool LoadFromMemory(const unsigned char* data, int width, int height, int channels, const TextureConfig& config = TextureConfig{});
    void CreateEmpty(int width, int height, const TextureConfig& config = TextureConfig{});
    
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
    void Delete();
    
    void SetWrap(TextureWrap wrapS, TextureWrap wrapT);
    void SetFilter(TextureFilter minFilter, TextureFilter magFilter);
    void GenerateMipmaps();
    
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    GLuint GetID() const { return m_textureID; }
    bool IsValid() const { return m_textureID != 0; }
    
    static void UnbindAll();
    
private:
    GLuint m_textureID;
    int m_width;
    int m_height;
    int m_channels;
    TextureConfig m_config;
    
    GLint GetInternalFormat(TextureFormat format);
    GLenum GetDataFormat(int channels);
    GLenum GetDataType(TextureFormat format);
};

class Cubemap {
public:
    Cubemap();
    ~Cubemap();
    
    Cubemap(const Cubemap&) = delete;
    Cubemap& operator=(const Cubemap&) = delete;
    
    Cubemap(Cubemap&& other) noexcept;
    Cubemap& operator=(Cubemap&& other) noexcept;
    
    bool LoadFromFiles(const std::string& right, const std::string& left,
                       const std::string& top, const std::string& bottom,
                       const std::string& front, const std::string& back);
    
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
    void Delete();
    
    GLuint GetID() const { return m_textureID; }
    bool IsValid() const { return m_textureID != 0; }
    
private:
    GLuint m_textureID;
};

} // namespace fps::renderer

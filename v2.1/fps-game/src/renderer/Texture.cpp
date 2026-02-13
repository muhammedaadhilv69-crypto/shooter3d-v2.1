#include "renderer/Texture.hpp"
#include "core/Logger.hpp"
#include <stb_image.h>

namespace fps::renderer {

Texture2D::Texture2D() : m_textureID(0), m_width(0), m_height(0), m_channels(0) {
}

Texture2D::~Texture2D() {
    Delete();
}

Texture2D::Texture2D(Texture2D&& other) noexcept 
    : m_textureID(other.m_textureID)
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_channels(other.m_channels)
    , m_config(other.m_config) {
    other.m_textureID = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept {
    if (this != &other) {
        Delete();
        m_textureID = other.m_textureID;
        m_width = other.m_width;
        m_height = other.m_height;
        m_channels = other.m_channels;
        m_config = other.m_config;
        other.m_textureID = 0;
    }
    return *this;
}

void Texture2D::Delete() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
    }
}

GLint Texture2D::GetInternalFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB8: return GL_RGB8;
        case TextureFormat::RGBA8: return GL_RGBA8;
        case TextureFormat::RGB16F: return GL_RGB16F;
        case TextureFormat::RGBA16F: return GL_RGBA16F;
        case TextureFormat::Depth16: return GL_DEPTH_COMPONENT16;
        case TextureFormat::Depth24: return GL_DEPTH_COMPONENT24;
        case TextureFormat::Depth32F: return GL_DEPTH_COMPONENT32F;
        case TextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
        case TextureFormat::R8: return GL_R8;
        case TextureFormat::RG8: return GL_RG8;
        default: return GL_RGBA8;
    }
}

GLenum Texture2D::GetDataFormat(int channels) {
    switch (channels) {
        case 1: return GL_RED;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return GL_RGBA;
    }
}

GLenum Texture2D::GetDataType(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB16F:
        case TextureFormat::RGBA16F:
        case TextureFormat::Depth32F:
            return GL_FLOAT;
        default:
            return GL_UNSIGNED_BYTE;
    }
}

bool Texture2D::LoadFromFile(const std::string& filepath, const TextureConfig& config) {
    stbi_set_flip_vertically_on_load(true);
    
    unsigned char* data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channels, 0);
    
    if (!data) {
        LOG_ERROR("Failed to load texture: " + filepath);
        return false;
    }
    
    bool result = LoadFromMemory(data, m_width, m_height, m_channels, config);
    stbi_image_free(data);
    
    if (result) {
        LOG_DEBUG("Texture loaded: " + filepath + " (" + std::to_string(m_width) + "x" + std::to_string(m_height) + ")");
    }
    
    return result;
}

bool Texture2D::LoadFromMemory(const unsigned char* data, int width, int height, int channels, const TextureConfig& config) {
    m_width = width;
    m_height = height;
    m_channels = channels;
    m_config = config;
    
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
    }
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(config.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(config.wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(config.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(config.magFilter));
    
    // Anisotropic filtering
    if (config.anisotropic) {
        GLfloat maxAnisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, std::min(config.anisotropyLevel, maxAnisotropy));
    }
    
    // Upload texture data
    GLenum dataFormat = GetDataFormat(channels);
    GLint internalFormat = GetInternalFormat(config.format);
    GLenum dataType = GetDataType(config.format);
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, dataType, data);
    
    if (config.generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return true;
}

void Texture2D::CreateEmpty(int width, int height, const TextureConfig& config) {
    m_width = width;
    m_height = height;
    m_channels = 4;
    m_config = config;
    
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
    }
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(config.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(config.wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(config.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(config.magFilter));
    
    GLint internalFormat = GetInternalFormat(config.format);
    GLenum format = (config.format == TextureFormat::Depth16 || 
                     config.format == TextureFormat::Depth24 || 
                     config.format == TextureFormat::Depth32F) ? GL_DEPTH_COMPONENT : GL_RGBA;
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture2D::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::UnbindAll() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SetWrap(TextureWrap wrapS, TextureWrap wrapT) {
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapT));
    Unbind();
}

void Texture2D::SetFilter(TextureFilter minFilter, TextureFilter magFilter) {
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
    Unbind();
}

void Texture2D::GenerateMipmaps() {
    Bind();
    glGenerateMipmap(GL_TEXTURE_2D);
    Unbind();
}

// Cubemap Implementation
Cubemap::Cubemap() : m_textureID(0) {
}

Cubemap::~Cubemap() {
    Delete();
}

Cubemap::Cubemap(Cubemap&& other) noexcept : m_textureID(other.m_textureID) {
    other.m_textureID = 0;
}

Cubemap& Cubemap::operator=(Cubemap&& other) noexcept {
    if (this != &other) {
        Delete();
        m_textureID = other.m_textureID;
        other.m_textureID = 0;
    }
    return *this;
}

void Cubemap::Delete() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
    }
}

bool Cubemap::LoadFromFiles(const std::string& right, const std::string& left,
                            const std::string& top, const std::string& bottom,
                            const std::string& front, const std::string& back) {
    std::string faces[] = { right, left, top, bottom, front, back };
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
    
    int width, height, channels;
    stbi_set_flip_vertically_on_load(false);
    
    for (unsigned int i = 0; i < 6; i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        
        if (data) {
            GLenum format = GetDataFormat(channels);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            LOG_ERROR("Failed to load cubemap face: " + faces[i]);
            stbi_image_free(data);
            return false;
        }
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    LOG_DEBUG("Cubemap loaded successfully");
    return true;
}

void Cubemap::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
}

void Cubemap::Unbind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

} // namespace fps::renderer

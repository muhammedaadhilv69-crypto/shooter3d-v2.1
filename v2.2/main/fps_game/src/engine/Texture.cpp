#include "Texture.h"
#include <iostream>

namespace Engine {

Texture::Texture() : m_textureID(0) {
}

Texture::~Texture() {
    if (m_textureID) {
        glDeleteTextures(1, &m_textureID);
    }
}

bool Texture::loadFromFile(const std::string& path) {
    // Placeholder - creates a simple procedural texture
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Create a simple 2x2 checkerboard pattern
    unsigned char data[] = {
        255, 255, 255, 255,  128, 128, 128, 255,
        128, 128, 128, 255,  255, 255, 255, 255
    };

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return true;
}

void Texture::bind(unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace Engine

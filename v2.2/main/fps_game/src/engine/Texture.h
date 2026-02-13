#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include <string>

namespace Engine {

class Texture {
public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string& path);
    void bind(unsigned int unit = 0);
    void unbind();

    GLuint getID() const { return m_textureID; }

private:
    GLuint m_textureID;
};

} // namespace Engine

#endif // TEXTURE_H

#pragma once
#include <string>
#include <vector>
#include <GL\glew.h>

class TextureManager
{
public:
	TextureManager(const std::string& filename);
	~TextureManager();

	void BindTexture(unsigned int obj); // bind upto 32 textures
	void TextureLoader(const std::string& filename);
	GLuint getID() { return m_textureManager; }

private:

	GLuint m_textureManager;
	unsigned int m_textureID;
};


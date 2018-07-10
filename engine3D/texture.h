#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
	Texture(const std::string& fileName);

	void Bind();
	void Unbind();

	unsigned int getTexture(){ return m_texture; }

	virtual ~Texture();
protected:
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}

	unsigned int m_texture;
};

#endif

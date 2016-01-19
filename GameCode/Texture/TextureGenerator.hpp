#pragma once

#ifndef TEXTUREGENERATOR_HPP
#define TEXTUREGENERATOR_HPP

#include "Engine\Math\Vec2.hpp"
#include "Engine\Core\VertexStruct.hpp"

#include <vector>


namespace Henry
{

class TextureGenerator
{
public:
	TextureGenerator( Vec2i numOfWindow , Vec2i windowSize , int numOfTexture);
	~TextureGenerator(void);
	void Render(size_t index = 0);
	void GenerateWindowTexture();
	void GenerateRoofTexture();
	void GenerateBlockTexture();

public:
	std::vector<int> m_windowTextureIDs;
	std::vector<int> m_roofTextureIDs;
	int m_blockTextureID;
	int m_specularTextureID;
	Vec2i m_numberOfWindow;
	Vec2i m_windowSize;
	Vec2i m_textureSize;

private:
	std::vector<Vertex_PCT> m_vertices;
};

};

#endif
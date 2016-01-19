#pragma once
#include "Engine\Math\Vec2.hpp"
#include "Engine\Core\VertexStruct.hpp"
#include "Engine\Core\Camera3D.hpp"
#include <vector>

#ifndef SKYBOX_HPP 
#define SKYBOX_HPP

namespace Henry
{

class SkyBox
{
public:
	SkyBox( Vec2i textureSize );
	~SkyBox(void);
	void Update(float deltaSecond , Camera3D camera);
	void Render();
	void GenerateBoxVertices();
	void GenerateTexture();

public:
	int m_textureStar;
	int m_textureBlur;
	Vec3f m_moonPosition;

private:
	std::vector<Vertex_PCT> m_vertices;
	std::vector<Vertex_PCT> m_moonVertices;
	Vec2i m_textureSize;
	float m_moonDistance;
	float m_moonDegrees;
	Vec3f m_cameraUpVector;
	Vec3f m_cameraRightVector;
};

}

#endif
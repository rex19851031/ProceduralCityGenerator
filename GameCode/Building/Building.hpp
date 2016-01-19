#pragma once

#ifndef BUILDING_HPP 
#define BUILDING_HPP

#include "Engine\Core\VertexStruct.hpp"
#include "Engine\Math\GeneralStruct.hpp"

#include "GameCode\Terrain\Terrain.hpp"

#include <vector>


namespace Henry
{

class Building
{
public:
	Building(const SpotAttributes& sa);
	~Building(void);
	virtual void Render();
	virtual void Generate(int generateType = 0);

public:
	bool m_isDirty;
	AABB2 m_aabb;
	Vec2f m_centerPoint;
	int m_numOflayer;
	float m_height;
	SpotAttributes m_spotAttributes;
	std::vector<int> m_textureIDs;
	bool m_drawDebug;
	bool m_enableTexture;

private:
	std::vector<Vertex_PosColor> m_vertices;
};

};

#endif
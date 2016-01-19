#pragma once

#ifndef TERRAIN_HPP 
#define TERRAIN_HPP

#include "Engine\Core\VertexStruct.hpp"
#include "Engine\Math\GeneralStruct.hpp"
#include "Engine\Parsing\TinyXML\TinyXMLHelper.hpp"

#include <vector>


namespace Henry
{


struct SpotAttributes
{
	SpotAttributes() : density(0.0f) , richness(0.0f) , fadeRatio(0.0f) , radius(0) { };
	Vec2i location;
	float density;
	float richness;
	float fadeRatio;
	int radius;
	//float population;
};


class Terrain
{
public:
	Terrain(Vec2i location);
	~Terrain(void);
	void Render();
	float GetHeightAt(Vec2f location);
	bool GetAttributesAt(Vec2i location , SpotAttributes* attribute);
	bool GetAttributesAt(Vec2f location , SpotAttributes* attribute);

public:
	Vec2i m_terrainSize;
	Vec2i m_location;
	AABB2 m_aabb;
	bool m_drawDebug;
	Vec2i m_halfSize;
	//int m_id;

private:
	void LoadXML();
	void ConstructDebugLines();

private:
	std::vector<Vertex_PosColor> m_vertices;
	std::vector<Vertex_PosColor> m_debugVertices;
	TinyXMLHelper m_xml;
	std::vector<SpotAttributes> m_spots;
};


};

#endif
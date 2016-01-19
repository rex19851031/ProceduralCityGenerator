#pragma once

#ifndef ROAD_HPP
#define ROAD_HPP

#include "Engine\Math\Vec2.hpp"
#include "Engine\Core\VertexStruct.hpp"

#include "GameCode\Terrain\Terrain.hpp"


namespace Henry
{

struct RoadTile
{
	RoadTile( Vec2f _position , RGBA _color = RGBA( 1.0f , 0.0f , 1.0f , 1.0f ) , Vec2f _size = Vec2f( 1.0f , 1.0f ) )
		: position(_position)
		, size(_size)
		, color(_color)	{ };
	Vec2f position;
	Vec2f size;
	RGBA color;
};

class Road
{
public:
	Road(void);
	~Road(void);
	void Render();
	void GenerateGridRoad( Terrain& _terrain , Vec2i numOfGrid , Vec2f roadSize = Vec2f( 0.2f , 0.2f) );

public:
	std::vector<RoadTile> m_roads;

private:
	void ConvertAndPushRoadToVertices( Terrain& _terrain);

private:
	std::vector<Vertex_PosColor> m_vertices;
};

}

#endif
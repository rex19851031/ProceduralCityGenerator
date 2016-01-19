#pragma once
#include "Engine\Math\Vec2.hpp"

#include "GameCode\Terrain\Terrain.hpp"
#include "GameCode\Building\Building.hpp"
#include "GameCode\Texture\TextureGenerator.hpp"

#ifndef BLOCK_HPP 
#define BLOCK_HPP

namespace Henry
{

class Block
{
public:
	Block( Vec2i location , Vec2i size , Terrain* terrain , TextureGenerator* tg);
	~Block(void);
	void Render();
	void Generate();
	
public:
	Vec2i m_location;
	Vec2i m_size;
	std::vector<Building*> m_buildings;
	std::vector<Vec2f> m_buildingLocations;

private:
	void GenerateBlockBase();
	Vec2f GenerateBuilding(Vec2f location);

private:
	//bool* m_occupyMap;
	Terrain* m_terrain;
	TextureGenerator* m_textureGenerator;
	std::vector<Vertex_PCT> m_vertices;
	std::vector<Vertex_PosColor> m_lightVertices;
};

}

#endif
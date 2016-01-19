#pragma once

#ifndef BOXBUILDING_HPP
#define BOXBUILDING_HPP

#include "GameCode\Building\Building.hpp"
#include "GameCode\Terrain\Terrain.hpp"
#include <map>


namespace Henry
{

class BoxBuilding : public Building
{
public:
	BoxBuilding(int generateType = 0 , const SpotAttributes& sa = SpotAttributes());
	~BoxBuilding(void);
	virtual void Render();
	virtual void Generate(int generateType);

private:
	void MakeBox(AABB2 dimentions , float currentHeight, float height , RGBA color = RGBA((unsigned char)255,255,255,255) , int listIndex = 0 , int rotateDegrees = 0);
	bool IsInside(AABB2 area , Vec2f checkPoint);
	void CorrectLocation();
	void GenerateTypeOne();
	void GenerateTypeTwo();
	void GenerateTypeThree();
	void GenerateTypeFour();
	void GenerateTypeFive();
	void GenerateTypeSix();
	void GenerateTypeSeven();

private:
	std::map<int,std::vector<Vertex_PCTN>> m_verticesLists;
	std::vector<int> m_vboIDs;
	std::vector<int> m_numOfVertices;
};

};

#endif
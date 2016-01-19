#pragma once

#ifndef CYLINDERBUILDING_HPP
#define CYLINDERBUILDING_HPP

#include "GameCode\Building\Building.hpp"


namespace Henry
{

class CylinderBuilding : public Building
{
public:
	CylinderBuilding( int generateType = 0 , const SpotAttributes& sa = SpotAttributes() );
	~CylinderBuilding(void);
	virtual void Render();
	virtual void Generate(int generateType);

private:
	void MakeCylinder( Vec2f center , float radius , int startDegrees , int endDegrees , float currentHeight , float advanceHeight , int numOfSegments , RGBA color = RGBA() , int rotateDegrees = 0 , bool connectPrevious = true );
	void MakeCap( Vec2f center , float radius , int startDegrees , int endDegrees , float height , int numOfSegments , RGBA color = RGBA() , int rotateDegrees = 0 , bool connectPrevious = true);
	void CloseCylinder( RGBA color = RGBA() );
	void CloseCap( RGBA color = RGBA() );
	void GenerateTypeOne();
	void GenerateTypeTwo();
	void GenerateTypeThree();
	void CorrectLocation();

private:
	std::vector<int> m_vboIDs;
	std::vector<int> m_numOfVertices;
	std::vector<Vertex_PCTN> m_vertices;
	std::vector<Vertex_PCTN> m_capVertices;
};

};

#endif
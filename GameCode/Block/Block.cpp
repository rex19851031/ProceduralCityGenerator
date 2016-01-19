#include "Block.hpp"
#include <string.h>

#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Core\HenryFunctions.hpp"

#include "GameCode\Building\BoxBuilding.hpp"
#include "GameCode\Building\CylinderBuilding.hpp"

namespace Henry
{

Block::Block( Vec2i location , Vec2i size , Terrain* terrain , TextureGenerator* tg)
	: m_location(location)
	, m_size(size)
	, m_terrain(terrain)
	, m_textureGenerator(tg)
{
// 	int accuracy = 10;
// 	int numOfOccupyCell = m_size.x * m_size.y * accuracy;
// 	m_occupyMap = new bool[numOfOccupyCell];
// 	memset(m_occupyMap,false,sizeof(bool) * numOfOccupyCell);
	GenerateBlockBase();
	Generate();
}


Block::~Block(void)
{
	//deleteVectorOfPointer(m_buildings);
}


void Block::Generate()
{
//	GenerateBuilding(Vec2f(0,0));

	bool full = false;
	float x = 0.1f;
	float y = 0.1f;
	float maxX = 0;
	int timeOfTrying = 0;
	int maxTryingTime = 10;

	while(!full)
	{
		Vec2f size = GenerateBuilding( Vec2f(x,y) );
		//y += (size.y + 0.1f);
		while( (y + (size.y + 0.1f) > m_size.y || 
				x + size.x > m_size.x) && 
				timeOfTrying < maxTryingTime )
		{
			m_buildings.pop_back();
			m_buildingLocations.pop_back();
			++timeOfTrying;
			size = GenerateBuilding( Vec2f(x,y) );
		}

		if(timeOfTrying == maxTryingTime)
		{
			m_buildings.pop_back();
			m_buildingLocations.pop_back();
		}

		timeOfTrying = 0;
		y += (size.y + 0.1f);

		if(maxX < size.x)
			maxX = size.x;

		if(y >= m_size.y)
		{
			y = 0.1f;
			x += (maxX + 0.1f);
			if(x >= m_size.x)
				full = true;
		}
	}
}


void Block::Render()
{
	if( m_vertices.size() )
	{
		OpenGLRenderer::BindTexture(m_textureGenerator->m_blockTextureID);
		OpenGLRenderer::DrawVertexWithVertexArray( m_vertices , OpenGLRenderer::SHAPE_TRIANGLES );
	}

	if( m_lightVertices.size() )
	{
		OpenGLRenderer::LineWidth(100.0f);
		OpenGLRenderer::BindWhiteTexture();
		OpenGLRenderer::DrawVertexWithVertexArray( m_lightVertices , OpenGLRenderer::SHAPE_LINES );
	}
// 	if( m_buildings.size() )
// 	{
// 		for(size_t index=0; index<m_buildings.size(); ++index)
// 		{
// 			m_buildings[index]->m_enableTexture = enableTexture;
// 			m_buildings[index]->Render();
// 		}
// 	}
}


void Block::GenerateBlockBase()
{
	Vertex_PCT vertices[6];
	vertices[0].position = Vec3f( 0.0f				, 0.0f				, 0.01f);
	vertices[1].position = Vec3f( (float)m_size.x	, 0.0f				, 0.01f);
	vertices[2].position = Vec3f( 0.0f				, (float)m_size.y	, 0.01f);

	vertices[3].position = Vec3f( 0.0f				, (float)m_size.y	, 0.01f);
	vertices[4].position = Vec3f( (float)m_size.x	, 0.0f				, 0.01f);
	vertices[5].position = Vec3f( (float)m_size.x	, (float)m_size.y	, 0.01f);

	vertices[0].texCoords = Vec2f( 0.0f	, 0.0f	);
	vertices[1].texCoords = Vec2f( 1.0f	, 0.0f	);
	vertices[2].texCoords = Vec2f( 0.0f	, 1.0f	);
							   
	vertices[3].texCoords = Vec2f( 0.0f	, 1.0f	);
	vertices[4].texCoords = Vec2f( 1.0f	, 0.0f	);
	vertices[5].texCoords = Vec2f( 1.0f	, 1.0f	);

	RGBA color( 0.1f , 0.4f , 0.1f , 1.0f );
	for( int index = 0; index < 6; ++index)
	{
		vertices[index].color = color;
		m_vertices.push_back(vertices[index]);
	}

	int numOfLightOnSide = 10;
	float oneOverNumOfLight = 1.0f / (float)numOfLightOnSide;
	Vec2f lightDistance( (float)m_size.x * oneOverNumOfLight , (float)m_size.y * oneOverNumOfLight ); 
	RGBA lightColor( (unsigned char)231 , 236 , 26 , 255 );
	float lightHeight = 0.1f;
	float line1 = 0.001f;
	Vec3f line2( line1  , line1  , -line1 );
	Vec3f line3( line1  , -line1 , -line1 );
	Vec3f line4( -line1 , line1  , -line1 );

	for(int index = 0; index <= numOfLightOnSide; ++index)
	{
		Vertex_PosColor vertices[32];
		vertices[0].position  = Vec3f( lightDistance.x * index	, -0.45f					, lightHeight ) + line1;
		vertices[1].position  = Vec3f( lightDistance.x * index	, -0.45f					, lightHeight ) - line1;
		vertices[2].position  = Vec3f( lightDistance.x * index	, -0.45f					, lightHeight ) + line2;
		vertices[3].position  = Vec3f( lightDistance.x * index	, -0.45f					, lightHeight ) - line2;
		vertices[4].position  = Vec3f( lightDistance.x * index	, -0.45f					, lightHeight ) + line3;
		vertices[5].position  = Vec3f( lightDistance.x * index	, -0.45f					, lightHeight ) - line3;
		vertices[6].position  = Vec3f( lightDistance.x * index	, -0.45f					, lightHeight ) + line4;
		vertices[7].position  = Vec3f( lightDistance.x * index	, -0.45f					, lightHeight ) - line4;

		vertices[8].position  = Vec3f( lightDistance.x * index	, (float)m_size.y + 0.45f	, lightHeight ) + line1;
		vertices[9].position  = Vec3f( lightDistance.x * index	, (float)m_size.y + 0.45f	, lightHeight ) - line1;
		vertices[10].position = Vec3f( lightDistance.x * index	, (float)m_size.y + 0.45f	, lightHeight ) + line2;
		vertices[11].position = Vec3f( lightDistance.x * index	, (float)m_size.y + 0.45f	, lightHeight ) - line2;
		vertices[12].position = Vec3f( lightDistance.x * index	, (float)m_size.y + 0.45f	, lightHeight ) + line3;
		vertices[13].position = Vec3f( lightDistance.x * index	, (float)m_size.y + 0.45f	, lightHeight ) - line3;
		vertices[14].position = Vec3f( lightDistance.x * index	, (float)m_size.y + 0.45f	, lightHeight ) + line4;
		vertices[15].position = Vec3f( lightDistance.x * index	, (float)m_size.y + 0.45f	, lightHeight ) - line4;
							  
		vertices[16].position = Vec3f( 0.0f - 0.45f				, lightDistance.y * index	, lightHeight ) + line1;
		vertices[17].position = Vec3f( 0.0f - 0.45f				, lightDistance.y * index	, lightHeight ) - line1;
		vertices[18].position = Vec3f( 0.0f - 0.45f				, lightDistance.y * index	, lightHeight ) + line2;
		vertices[19].position = Vec3f( 0.0f - 0.45f				, lightDistance.y * index	, lightHeight ) - line2;
		vertices[20].position = Vec3f( 0.0f - 0.45f				, lightDistance.y * index	, lightHeight ) + line3;
		vertices[21].position = Vec3f( 0.0f - 0.45f				, lightDistance.y * index	, lightHeight ) - line3;
		vertices[22].position = Vec3f( 0.0f - 0.45f				, lightDistance.y * index	, lightHeight ) + line4;
		vertices[23].position = Vec3f( 0.0f - 0.45f				, lightDistance.y * index	, lightHeight ) - line4;
		
		vertices[24].position = Vec3f( (float)m_size.x + 0.45f	, lightDistance.y * index	, lightHeight ) + line1;
		vertices[25].position = Vec3f( (float)m_size.x + 0.45f	, lightDistance.y * index	, lightHeight ) - line1;
		vertices[26].position = Vec3f( (float)m_size.x + 0.45f	, lightDistance.y * index	, lightHeight ) + line2;
		vertices[27].position = Vec3f( (float)m_size.x + 0.45f	, lightDistance.y * index	, lightHeight ) - line2;
		vertices[28].position = Vec3f( (float)m_size.x + 0.45f	, lightDistance.y * index	, lightHeight ) + line3;
		vertices[29].position = Vec3f( (float)m_size.x + 0.45f	, lightDistance.y * index	, lightHeight ) - line3;
		vertices[30].position = Vec3f( (float)m_size.x + 0.45f	, lightDistance.y * index	, lightHeight ) + line4;
		vertices[31].position = Vec3f( (float)m_size.x + 0.45f	, lightDistance.y * index	, lightHeight ) - line4;

		for(int count = 0; count < 32; ++ count)
		{
			vertices[count].color = lightColor;
			m_lightVertices.push_back( vertices[count] );
		}
	}
}


Vec2f Block::GenerateBuilding(Vec2f location)
{
	location = Vec2f( location.x + m_location.x , location.y + m_location.y );

	SpotAttributes sa;
	m_terrain->GetAttributesAt( location , &sa );
	int dice = random(0,10);
	//BoxBuilding* building = new BoxBuilding( /*random(1,2)*/1 , sa );
	//CylinderBuilding* newBuilding = new CylinderBuilding( 2 , sa );
	
	Building* newBuilding;
	int buindingType = random(1,7);
	if( dice < 7/* && sa.richness < 0.5f*/ )
	{
		BOX:
		buindingType = buindingType > 2 ? 2 : buindingType;
		newBuilding = new BoxBuilding(buindingType,sa);
	}
	else
	{
		if(sa.richness < 0.5f)
			goto BOX;
		buindingType = buindingType > 2 ? 2 : buindingType;
		newBuilding = new CylinderBuilding(buindingType,sa);
	}

 	newBuilding->m_textureIDs.push_back( m_textureGenerator->m_windowTextureIDs[ random(0,9) ] );
 	m_buildings.push_back( newBuilding );
 	m_buildingLocations.push_back(location);
 	return newBuilding->m_aabb.max;
}


}
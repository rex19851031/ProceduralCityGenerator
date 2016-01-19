#include "Road.hpp"

#include "Engine\Renderer\OpenGLRenderer.hpp"


namespace Henry
{


Road::Road(void)
{
}


Road::~Road(void)
{
}


void Road::Render()
{
	if(m_vertices.size() != 0)
		OpenGLRenderer::DrawVertexWithVertexArray(m_vertices,OpenGLRenderer::SHAPE_TRIANGLES);
}


void Road::GenerateGridRoad( Terrain& _terrain , Vec2i numOfBlock , Vec2f roadSize )
{
	if(m_roads.size() != 0)
	{
		std::vector<RoadTile> empty;
		m_roads.clear();
		m_roads = empty;
	}

	if(m_vertices.size() != 0)
	{
		std::vector<Vertex_PosColor> empty;
		m_vertices.clear();
		m_vertices = empty;
	}

	Vec2f oneOverRoadSize = Vec2f( 1.0f / roadSize.x , 1.0f / roadSize.y );
	Vec2f terrainScope = _terrain.m_aabb.max - _terrain.m_aabb.min;
	Vec2f blockSize = terrainScope / Vec2f( (float)numOfBlock.x , (float)numOfBlock.y );
	Vec2f roadLocate = blockSize * oneOverRoadSize;
	Vec2f terrainGrid = terrainScope * oneOverRoadSize;
	Vec2f startCorner = _terrain.m_aabb.min;
	Vec2f endCorner = _terrain.m_aabb.max;

	for( int x = 0; x < terrainGrid.x; ++x )
	{
		for( int y = 0; y < terrainGrid.y; ++y )
		{
			if( x % (int)roadLocate.x == 0 || (x+1) % (int)roadLocate.x == 0 ||
				y % (int)roadLocate.y == 0 || (y+1) % (int)roadLocate.y == 0 )
			{
				Vec2f location = Vec2f( (float)x , (float)y ) * roadSize + startCorner;
				RoadTile tile( location , RGBA( 0.2f , 0.2f , 0.2f , 1.0f ) , roadSize);
				m_roads.push_back( tile );
			}
		}
	}

	ConvertAndPushRoadToVertices(_terrain);
}

void Road::ConvertAndPushRoadToVertices(Terrain& _terrain)
{
	for( size_t index=0; index < m_roads.size(); index++)
	{
		RoadTile tile = m_roads[index];
		//Vec2f min = tile.position - tile.size;
		//Vec2f max = tile.position + tile.size;
		Vec2f min = tile.position;
		Vec2f max = tile.position + tile.size;

		Vertex_PosColor vertices[6];
		vertices[0].position = Vec3f( (float)min.x , (float)min.y , 0.01f + _terrain.GetHeightAt( min ) );
		vertices[1].position = Vec3f( (float)max.x , (float)min.y , 0.01f + _terrain.GetHeightAt( Vec2f( (float)max.x , (float)min.y ) ) );
		vertices[2].position = Vec3f( (float)min.x , (float)max.y , 0.01f + _terrain.GetHeightAt( Vec2f( (float)min.x , (float)max.y ) ) );

		vertices[3].position = Vec3f( (float)min.x , (float)max.y , 0.01f + _terrain.GetHeightAt( Vec2f( (float)min.x , (float)max.y ) ) );
		vertices[4].position = Vec3f( (float)max.x , (float)min.y , 0.01f + _terrain.GetHeightAt( Vec2f( (float)max.x , (float)min.y ) ) );
		vertices[5].position = Vec3f( (float)max.x , (float)max.y , 0.01f + _terrain.GetHeightAt(max) );

		for( int index = 0; index < 6; ++index)
		{
			vertices[index].color = tile.color;
			m_vertices.push_back(vertices[index]);
		}
	}
}


};
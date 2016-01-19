#include "Terrain.hpp"

#include "Engine\Renderer\OpenGLRenderer.hpp"

#include "GameCode\GameCommon.hpp"


namespace Henry
{

Terrain::Terrain(Vec2i location) : m_location( location )
{
	LoadXML();
	ConstructDebugLines();

	m_halfSize = Vec2i( m_terrainSize.x >> 1 , m_terrainSize.y >> 1 );
	Vec2i min = location - m_halfSize;
	Vec2i max = location + m_halfSize;
	m_aabb.min = Vec2f( (float)min.x , (float)min.y );
	m_aabb.max = Vec2f( (float)max.x , (float)max.y );

	Vertex_PosColor vertices[6];
	vertices[0].position = Vec3f( (float)min.x , (float)min.y , 0.0f);
	vertices[1].position = Vec3f( (float)max.x , (float)min.y , 0.0f);
	vertices[2].position = Vec3f( (float)min.x , (float)max.y , 0.0f);
	
	vertices[3].position = Vec3f( (float)min.x , (float)max.y , 0.0f);
	vertices[4].position = Vec3f( (float)max.x , (float)min.y , 0.0f);
	vertices[5].position = Vec3f( (float)max.x , (float)max.y , 0.0f);

	RGBA color( 0.1f , 0.1f , 0.1f , 1.0f );
	for( int index = 0; index < 6; ++index)
	{
		vertices[index].color = color;
		m_vertices.push_back(vertices[index]);
	}

	m_drawDebug = false;
}


Terrain::~Terrain(void)
{
}


void Terrain::Render()
{
	if(m_vertices.size())
		OpenGLRenderer::DrawVertexWithVertexArray( m_vertices , OpenGLRenderer::SHAPE_TRIANGLES );
	
	if(m_debugVertices.size() && m_drawDebug)
		OpenGLRenderer::DrawVertexWithVertexArray( m_debugVertices , OpenGLRenderer::SHAPE_LINES );
}


float Terrain::GetHeightAt(Vec2f location)
{
	// if generated from perlin noise , can access that texture and return based on that
	// or do sampling for points around , maybe have accurate value pass in too , the smaller the more accurate
	return 0.0f;
}


void Terrain::LoadXML()
{
	if(m_spots.size() != 0)
	{
		std::vector<SpotAttributes> empty;
		m_spots.clear();
		m_spots = empty;
	}

	m_xml.LoadFile("./Data/MapAttribute.xml");
	sscanf_s(m_xml.m_root->Attribute("size"),"%d,%d",&m_terrainSize.x,&m_terrainSize.y);
	Vec2i halfTerrainSize(m_terrainSize.x >> 1 , m_terrainSize.y >> 1);

	for(TiXmlElement* elem = m_xml.m_root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string elemName = elem->Value();
		if(elemName == "Spot")
		{
			SpotAttributes sa;
			sscanf_s(elem->Attribute("location"),"%d,%d",&sa.location.x,&sa.location.y);
			sa.density = (float)std::atof(elem->Attribute("density"));
			sa.richness = (float)std::atof(elem->Attribute("richness"));
			sa.fadeRatio = (float)std::atof(elem->Attribute("fadeRatio"));
			sa.radius = std::atoi(elem->Attribute("affectRadius"));
			sa.location -= halfTerrainSize;
			m_spots.push_back(sa);
		}
	}
}


void Terrain::ConstructDebugLines()
{
	if(m_debugVertices.size() != 0)
	{
		std::vector<Vertex_PosColor> empty;
		m_debugVertices.clear();
		m_debugVertices = empty;
	}

	float scale = 10.0f;
	SpotAttributes sa;
	Vec2i halfTerrainSize(m_terrainSize.x >> 1 , m_terrainSize.y >> 1);

	for(int x = -halfTerrainSize.x; x < halfTerrainSize.x; ++x)
	{
		for(int y = -halfTerrainSize.y; y < halfTerrainSize.y; ++y)
		{
			if( GetAttributesAt( Vec2i( x , y ) , &sa ) )
			{
				Vertex_PosColor vertices[4];
				vertices[0].position = Vec3f( (float)x , (float)y , 0.0f );
				vertices[1].position = Vec3f( (float)x , (float)y , sa.density * scale );
				vertices[0].color = RGBA( 0.5f , 1.0f , 0.8f , 1.0f );
				vertices[1].color = RGBA( 0.5f , 1.0f , 0.8f , 1.0f );

				vertices[2].position = Vec3f( (float)x , (float)y , 0.0f );
				vertices[3].position = Vec3f( (float)x , (float)y , sa.richness * scale );
				vertices[2].color = RGBA( 0.5f , 0.2f , 0.2f , 1.0f );
				vertices[3].color = RGBA( 0.5f , 0.2f , 0.2f , 1.0f );

				for(int count=0; count<4; ++count)
					m_debugVertices.push_back(vertices[count]);
			}
		}
	}
}


bool Terrain::GetAttributesAt(Vec2f location , SpotAttributes* attribute)
{
	Vec2i location_i( (int)location.x , (int)location.y );
	return GetAttributesAt( location_i , attribute );
}


bool Terrain::GetAttributesAt(Vec2i location , SpotAttributes* attribute)
{
	bool success = false;
	//location = location - m_halfSize;

	for(size_t index=0; index < m_spots.size(); ++index)
	{
		Vec2f location2Spot = Vec2f( (float)m_spots[index].location.x - location.x , (float)m_spots[index].location.y - location.y );
		float radiusSquare = (float)(m_spots[index].radius * m_spots[index].radius);
		float distanceSquare = location2Spot.lengthSquare();
		
		if(distanceSquare < radiusSquare)
		{
			float ratio = 1.0f - (1.0f - m_spots[index].fadeRatio) * (distanceSquare / radiusSquare);

			if(success)
			{
				attribute->density = (attribute->density + m_spots[index].density * ( 1.0f - (distanceSquare / radiusSquare) ) ) * 0.5f * ratio;
				attribute->richness = (attribute->richness + m_spots[index].richness * ( 1.0f - (distanceSquare / radiusSquare) ) ) * 0.5f * ratio;
			}
			else
			{
				attribute->density = m_spots[index].density * ( 1.0f - (distanceSquare / radiusSquare) ) * ratio;
				attribute->richness = m_spots[index].richness * ( 1.0f - (distanceSquare / radiusSquare) ) * ratio;
			}

			success = true;
		}
	}

	return success;
}


};
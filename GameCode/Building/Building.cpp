#include "Building.hpp"

#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Math\Vec2.hpp"
#include "Engine\Math\Vec3.hpp"
#include "Engine\Core\HenryFunctions.hpp"

#define UNUSED(x) (void)(x);


namespace Henry
{


Building::Building( const SpotAttributes& sa)
	: m_spotAttributes(sa)
{
	m_height = 0.0f;
	m_aabb.min = Vec2f(0.0f,0.0f);
	m_aabb.max = Vec2f(1.0f,1.0f);
	m_drawDebug = false;
	m_enableTexture = false;
	m_isDirty = true;
	Generate();
}


Building::~Building(void)
{
}


void Building::Render()
{
	if(m_vertices.size() && m_drawDebug)
		OpenGLRenderer::DrawVertexWithVertexArray(m_vertices,OpenGLRenderer::SHAPE_LINES);
}


void Building::Generate(int generateType)
{
	UNUSED(generateType);

	if(m_vertices.size())
	{
		std::vector<Vertex_PosColor> empty;
		m_vertices.clear();
		m_vertices = empty;
	}

	Vec2f min = m_aabb.min;
	Vec2f max = m_aabb.max;
	Vertex_PosColor vertices[8];
	vertices[0].position = Vec3f( min.x, min.y, 0.0f );
	vertices[1].position = Vec3f( min.x, min.y, m_height );
													 
	vertices[2].position = Vec3f( min.x, max.y, 0.0f );
	vertices[3].position = Vec3f( min.x, max.y, m_height );
													 
	vertices[4].position = Vec3f( max.x, min.y, 0.0f );
	vertices[5].position = Vec3f( max.x, min.y, m_height );
													 
	vertices[6].position = Vec3f( max.x, max.y, 0.0f );
	vertices[7].position = Vec3f( max.x, max.y, m_height );

	RGBA color(1.0f,0.0f,1.0f,1.0f);
	for(int index=0; index<8; ++index)
	{
		vertices[index].color = color;
		m_vertices.push_back(vertices[index]);
	}
}


};
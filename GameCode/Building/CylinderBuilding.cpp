#include "CylinderBuilding.hpp"
#include "Engine\Core\HenryFunctions.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"


namespace Henry
{

CylinderBuilding::CylinderBuilding( int generateType , const SpotAttributes& sa) : Building( sa )
{	
	m_aabb.min = Vec2f(50.0f,50.0f);
	m_aabb.max = Vec2f(-50.0f,-50.0f);
	Generate(generateType);
	m_drawDebug = true;
}


CylinderBuilding::~CylinderBuilding(void)
{
	for(size_t index=0; index < m_vboIDs.size(); ++index)
		OpenGLRenderer::DeleteBuffer(m_vboIDs[index]);
}


void CylinderBuilding::Render()
{
	Building::Render();
	if(m_isDirty)
	{
		size_t count = 0;
		if( m_vertices.size() != 0 )
		{
			int textureID = m_enableTexture ? m_textureIDs[0] : OpenGLRenderer::DIFFUSE_TEXTURE_ID;

			if(count == m_vboIDs.size())
			{
				int vboID = 0;
				OpenGLRenderer::GenerateBuffers(&vboID);
				m_vboIDs.push_back(vboID);
				m_numOfVertices.push_back(m_vertices.size());
			}
			OpenGLRenderer::BindTexture(textureID);
			OpenGLRenderer::BufferData( m_vertices , m_vboIDs[count] );
			//OpenGLRenderer::DrawVertexWithVertexArray(m_vertices,OpenGLRenderer::SHAPE_QUADS);
			++count;
		}

		if( m_capVertices.size() != 0 )
		{
			if(count == m_vboIDs.size())
			{
				int vboID = 0;
				OpenGLRenderer::GenerateBuffers(&vboID);
				m_vboIDs.push_back(vboID);
				m_numOfVertices.push_back(m_vertices.size());
			}
			OpenGLRenderer::BufferData( m_capVertices , m_vboIDs[count] );
			//OpenGLRenderer::DrawVertexWithVertexArray(m_capVertices,OpenGLRenderer::SHAPE_TRIANGLES);
			++count;
		}

		m_isDirty = false;
	}
	else
	{
		for(size_t index=0; index < m_vboIDs.size(); ++index)
		{			
			int textureID = m_enableTexture ? m_textureIDs[0] : OpenGLRenderer::DIFFUSE_TEXTURE_ID;

			OpenGLRenderer::BindTexture(textureID);
			if( (int)(index & 1) == 1 )
				OpenGLRenderer::DrawVertexWithVertexBufferObject(m_vboIDs[index],m_numOfVertices[index],OpenGLRenderer::SHAPE_TRIANGLES,Henry::Vertex_PCTN());
			else
				OpenGLRenderer::DrawVertexWithVertexBufferObject(m_vboIDs[index],m_numOfVertices[index],OpenGLRenderer::SHAPE_QUADS,Henry::Vertex_PCTN());
		}
	}
}


void CylinderBuilding::CorrectLocation()
{
	Vec2f offset( m_aabb.min * -1 );

	for(size_t index = 0; index < m_vertices.size(); ++index)
	{
		m_vertices[index].position.x += offset.x;
		m_vertices[index].position.y += offset.y;
	}

	for(size_t index=0; index < m_capVertices.size(); ++index)
	{
		m_capVertices[index].position.x += offset.x;
		m_capVertices[index].position.y += offset.y;
	}

	m_aabb.min += offset;
	m_aabb.max += offset;
	Building::Generate();
}


void CylinderBuilding::Generate(int generateType)
{
	std::vector<Vertex_PCTN> empty;
	m_vertices.clear();
	m_vertices = empty;
	m_capVertices.clear();
	m_capVertices = empty;

	if(generateType == 0)
		generateType = random((int)1,2);
	
	switch(generateType)
	{
	case 1:
		GenerateTypeOne();
		break;	
	case 2:
		GenerateTypeTwo();
		break;
	case 3:
		GenerateTypeThree();
		break;
	}

	CorrectLocation();
	
	int count = 0;
	for(size_t index=0; index < m_vertices.size(); ++index)
	{
		count++;
		if(count == 3)
		{
			Vec3f tangent;
			Vec3f biTangent;
			ComputeSurfaceTangentsAtVertex(	tangent ,
				biTangent ,
				m_vertices[index-1].normals.normal ,
				m_vertices[index-1].position , 
				m_vertices[index-1].texCoords ,
				m_vertices[index-2].position , 
				m_vertices[index-2].texCoords ,
				m_vertices[index-0].position ,
				m_vertices[index-0].texCoords );

			m_vertices[index-2].normals.tangent = tangent;
			m_vertices[index-1].normals.tangent = tangent;
			m_vertices[index-0].normals.tangent = tangent;

			m_vertices[index-2].normals.biTangent = biTangent;
			m_vertices[index-1].normals.biTangent = biTangent;
			m_vertices[index-0].normals.biTangent = biTangent;

			count = 0;
		}
	}

	count = 0;
	for(size_t index=0; index < m_capVertices.size(); ++index)
	{
		count++;
		if(count == 3)
		{
			Vec3f tangent;
			Vec3f biTangent;
			ComputeSurfaceTangentsAtVertex(	tangent ,
				biTangent ,
				m_capVertices[index-1].normals.normal ,
				m_capVertices[index-1].position , 
				m_capVertices[index-1].texCoords ,
				m_capVertices[index-2].position , 
				m_capVertices[index-2].texCoords ,
				m_capVertices[index-0].position ,
				m_capVertices[index-0].texCoords );

			m_capVertices[index-2].normals.tangent = tangent;
			m_capVertices[index-1].normals.tangent = tangent;
			m_capVertices[index-0].normals.tangent = tangent;

			m_capVertices[index-2].normals.biTangent = biTangent;
			m_capVertices[index-1].normals.biTangent = biTangent;
			m_capVertices[index-0].normals.biTangent = biTangent;

			count = 0;
		}
	}
}


void CylinderBuilding::MakeCylinder( Vec2f center , float radius , int startDegrees , int endDegrees , float currentHeight , float advanceHeight , int numOfSegments , RGBA color , int rotateDegrees , bool connectPrevious)
{	
	startDegrees += rotateDegrees;
	endDegrees += rotateDegrees;

	float oneOverSegments = 1.0f / (float)numOfSegments;
	float px = center.x + radius * cos( degree2radians((float)startDegrees) );
	float py = center.y + radius * sin( degree2radians((float)startDegrees) );

	if( m_vertices.size() != 0 && connectPrevious )
	{
		Vertex_PCTN vertices[4];
		vertices[0].position = m_vertices[m_vertices.size() - 4].position;
		vertices[1].position = Vec3f( px , py , currentHeight );
		vertices[2].position = Vec3f( px , py , advanceHeight );
		vertices[3].position = m_vertices[m_vertices.size() - 1].position;

		Vec2f texMin( 0.0f , 0.0f );
		float texHeight = vertices[3].position.z * 0.15625f;
		float length = Vec2f( vertices[3].position.x - vertices[1].position.x , vertices[3].position.y - vertices[1].position.y ).length() * 0.15625f;

		vertices[0].texCoords = Vec2f( texMin.x	, texHeight );
		vertices[1].texCoords = Vec2f( length	, texHeight );
		vertices[2].texCoords = Vec2f( length	, texMin.y );
		vertices[3].texCoords = Vec2f( texMin.x	, texMin.y );

		vertices[0].color = color;
		vertices[1].color = color;
		vertices[2].color = color;
		vertices[3].color = color;

		for(int num = 0; num < 4; ++num)
			m_vertices.push_back(vertices[num]);
	}

	float texHeight = (advanceHeight - currentHeight) * 0.15625f;
	float arcLength = 2.0f * 3.1415926f * radius * ( abs(endDegrees - startDegrees) / 360.0f ) * 0.15625f;
	float unitLength = arcLength / numOfSegments;
	float prevLength = 0;
	float texMax = unitLength;
	bool aabbChanged = false;

	for(int index = 1; index < numOfSegments; ++index)
	{
		float theta = degree2radians( startDegrees + (endDegrees - startDegrees) * index * oneOverSegments);
		float x = center.x + radius * cos(theta);
		float y = center.y + radius * sin(theta);
		
		Vertex_PCTN vertices[4];
		vertices[0].position = Vec3f( px , py , currentHeight );
		vertices[1].position = Vec3f( x  , y  , currentHeight );
		vertices[2].position = Vec3f( x  , y  , advanceHeight );
		vertices[3].position = Vec3f( px , py , advanceHeight );

 		vertices[0].color = color;
 		vertices[1].color = color;
 		vertices[2].color = color;
 		vertices[3].color = color;

		texMax += unitLength;
		vertices[0].texCoords = Vec2f( prevLength	, texHeight );
		vertices[1].texCoords = Vec2f( texMax		, texHeight );
		vertices[2].texCoords = Vec2f( texMax		, 0.0f );
		vertices[3].texCoords = Vec2f( prevLength	, 0.0f );
		prevLength = texMax;

		for(int num = 0; num < 4; ++num)
		{
			m_vertices.push_back(vertices[num]);

			if( vertices[num].position.x > m_aabb.max.x || vertices[num].position.y > m_aabb.max.y ||
				vertices[num].position.x < m_aabb.min.x || vertices[num].position.y < m_aabb.min.y )
				aabbChanged = true;

			if(vertices[num].position.x > m_aabb.max.x)
				m_aabb.max.x = vertices[num].position.x;
			if(vertices[num].position.y > m_aabb.max.y)
				m_aabb.max.y = vertices[num].position.y;
			if(vertices[num].position.x < m_aabb.min.x)
				m_aabb.min.x = vertices[num].position.x;
			if(vertices[num].position.y < m_aabb.min.y)
				m_aabb.min.y = vertices[num].position.y;
		}

		px = x;
		py = y;
	}

	if(aabbChanged)
		Building::Generate();
}


void CylinderBuilding::MakeCap( Vec2f center , float radius , int startDegrees , int endDegrees , float height , int numOfSegments , RGBA color , int rotateDegrees , bool connectPrevious )
{
	startDegrees += rotateDegrees;
	endDegrees += rotateDegrees;

	float oneOverSegments = 1.0f / (float)numOfSegments;
	float px = center.x + radius * cos( degree2radians((float)startDegrees) );
	float py = center.y + radius * sin( degree2radians((float)startDegrees) );

	if( m_capVertices.size() != 0 && connectPrevious )
	{
		Vertex_PCTN vertices[3];
		vertices[0].position = m_capVertices[m_capVertices.size() - 3].position;
		vertices[1].position = m_capVertices[m_capVertices.size() - 1].position;
		vertices[2].position = Vec3f( px		, py		, height );

		vertices[0].texCoords = Vec2f( 0.0f , 0.0f );
		vertices[1].texCoords = Vec2f( 0.0f , 0.0f );
		vertices[2].texCoords = Vec2f( 0.0f , 0.0f );

		for(int num = 0; num < 3; ++num)
			m_capVertices.push_back(vertices[num]);

	}

	for(int index = 1; index < numOfSegments; ++index)
	{
		float theta = degree2radians( startDegrees + (endDegrees - startDegrees) * index * oneOverSegments);
		float x = center.x + radius * cos(theta);
		float y = center.y + radius * sin(theta);

		Vertex_PCTN vertices[3];
		vertices[0].position = Vec3f( center.x	, center.y	, height );
		vertices[1].position = Vec3f( px		, py		, height );
		vertices[2].position = Vec3f( x			, y			, height );
		
		vertices[0].texCoords = Vec2f( 0.0f , 0.0f );
		vertices[1].texCoords = Vec2f( 0.0f , 0.0f );
		vertices[2].texCoords = Vec2f( 0.0f , 0.0f );

		vertices[0].color = color;
		vertices[1].color = color;
		vertices[2].color = color;
		
		for(int num = 0; num < 3; ++num)
			m_capVertices.push_back(vertices[num]);

		px = x;
		py = y;
	}
}


void CylinderBuilding::CloseCylinder( RGBA color )
{
	Vertex_PCTN vertices[4];
	vertices[0].position = m_vertices[m_vertices.size() - 4].position;
	vertices[1].position = m_vertices[0].position;
	vertices[2].position = m_vertices[3].position;
	vertices[3].position = m_vertices[m_vertices.size() - 1].position;

	Vec2f texMin( 0.0f , 0.0f );
	Vec2f texMax( vertices[3].position.x - vertices[1].position.x , vertices[3].position.y - vertices[1].position.y );
	float height = vertices[3].position.z * 0.15625f;
	float length = texMax.length() * 0.15625f;

	vertices[0].texCoords = Vec2f( texMin.x	, height );
	vertices[1].texCoords = Vec2f( length	, height );
	vertices[2].texCoords = Vec2f( length	, texMin.y );
	vertices[3].texCoords = Vec2f( texMin.x	, texMin.y );

	vertices[0].color = color;
	vertices[1].color = color;
	vertices[2].color = color;
	vertices[3].color = color;

	for(int num = 0; num < 4; ++num)
		m_vertices.push_back(vertices[num]);
}


void CylinderBuilding::CloseCap( RGBA color )
{
	Vertex_PCTN capVertices[3];
	capVertices[0].position = m_capVertices[m_capVertices.size()-3].position;
	capVertices[1].position = m_capVertices[m_capVertices.size()-1].position;
	capVertices[2].position = m_capVertices[1].position;

	capVertices[0].texCoords = Vec2f( 0.0f , 0.0f );
	capVertices[1].texCoords = Vec2f( 0.0f , 0.0f );
	capVertices[2].texCoords = Vec2f( 0.0f , 0.0f );

	capVertices[0].color = color;
	capVertices[1].color = color;
	capVertices[2].color = color;

	for(int num = 0; num < 3; ++num)
		m_capVertices.push_back(capVertices[num]);
}


void CylinderBuilding::GenerateTypeOne()
{
	//float radius = 0.2f + 0.3f * getRandomPercent();
	//float height = 0.5f + 1.5f * getRandomPercent();
	RGBA BuildingColor((unsigned char)random(0,2),(unsigned char)random(0,16),(unsigned char)random(16,32),64);
	m_height = 10.0f * random( 10 , (int)(100 * m_spotAttributes.richness)) * 0.01f;
	float radius = 1.0f - (int)( m_spotAttributes.density * random( 4 , 9 ) ) * 0.1f;
	int rotateDegrees = random(0,360);
	int degree1 = random(   60 , 120 );
	int degree2 = random( 240 , 360 );
	MakeCylinder( Vec2f(0.5f,0.5f) , radius , 0	  , degree1 , 0.0f , m_height , 50 , BuildingColor , rotateDegrees );
	MakeCylinder( Vec2f(0.5f,0.5f) , radius , 180 , degree2 , 0.0f , m_height , 50 , BuildingColor , rotateDegrees );
	CloseCylinder( BuildingColor );

	MakeCap( Vec2f(0.5f,0.5f) , radius , 0   , degree1 , m_height , 50 , BuildingColor , rotateDegrees );
	MakeCap( Vec2f(0.5f,0.5f) , radius , 180 , degree2 , m_height , 50 , BuildingColor , rotateDegrees );
	CloseCap( BuildingColor );
}


void CylinderBuilding::GenerateTypeTwo()
{
	//float radius = 0.1f + 0.4f * getRandomPercent();
	//float m_height = 1.0f + 1.0f * getRandomPercent();
	RGBA BuildingColor((unsigned char)random(0,2),(unsigned char)random(0,16),(unsigned char)random(16,32),64);
	m_height = 10.0f * random( 10 , (int)(100 * m_spotAttributes.richness)) * 0.01f;
	float radius = 1.0f - (int)( m_spotAttributes.density * random( 4 , 9 ) ) * 0.1f;
	int rotateDegrees = random(0,360);
	MakeCylinder( Vec2f(0.5f,0.5f) , radius , 0	  , 360 , 0.0f , m_height , 50 , BuildingColor , rotateDegrees );
	CloseCylinder( BuildingColor );
	MakeCap( Vec2f(0.5f,0.5f) , radius , 0   , 360 , m_height , 50 , BuildingColor , rotateDegrees );
	CloseCap( BuildingColor );
}


void CylinderBuilding::GenerateTypeThree()
{
	RGBA BuildingColor((unsigned char)random(0,2),(unsigned char)random(0,16),(unsigned char)random(16,32),64);
	float maximumHeight = 2.0f;
	float currentHeight = 0.0f;
	float pre_radius = 0.4f;
	float pre_advanceHeight = 1.5f;

	while(currentHeight < maximumHeight)
	{
		float radius = 0.1f + pre_radius * getRandomPercent();
		float advanceHeight = 0.5f + pre_advanceHeight * getRandomPercent();

		int rotateDegrees = random(0,360);
		MakeCylinder( Vec2f(0.5f,0.5f) , radius , 0	  , 360 , currentHeight , advanceHeight , 50 , BuildingColor , rotateDegrees , false );
		CloseCylinder( BuildingColor );

		MakeCap( Vec2f(0.5f,0.5f) , radius , 0   , 360 , currentHeight + advanceHeight , 50 , BuildingColor , rotateDegrees , false );
		CloseCap( BuildingColor );

		pre_advanceHeight = advanceHeight;
		pre_radius = radius;
		currentHeight += advanceHeight;
	}
}


};
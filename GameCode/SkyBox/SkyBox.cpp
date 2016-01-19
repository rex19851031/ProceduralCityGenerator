#include "SkyBox.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Core\HenryFunctions.hpp"


namespace Henry
{

SkyBox::SkyBox( Vec2i textureSize ) : m_textureSize( textureSize )
{
	OpenGLRenderer::PointSmooth(true);
	GenerateBoxVertices();
	GenerateTexture();
	m_moonDistance = 200.0f;
	m_moonDegrees = 0.0f;
}


SkyBox::~SkyBox(void)
{
}


void SkyBox::GenerateBoxVertices()
{
	Vertex_PCT vertices[24];
	vertices[0].position  = Vec3f( -500.0f	, -500.0f	, -500.0f	);
	vertices[1].position  = Vec3f( -500.0f	, -500.0f	, +500.0f	);
	vertices[2].position  = Vec3f( +500.0f	, -500.0f	, +500.0f	);
	vertices[3].position  = Vec3f( +500.0f	, -500.0f	, -500.0f	);

	vertices[4].position  = Vec3f( -500.0f	, +500.0f	, -500.0f	);
	vertices[5].position  = Vec3f( -500.0f	, +500.0f	, +500.0f	);
	vertices[6].position  = Vec3f( -500.0f	, -500.0f	, +500.0f	);
	vertices[7].position  = Vec3f( -500.0f	, -500.0f	, -500.0f	);
	
	vertices[8].position  = Vec3f( +500.0f	, +500.0f	, -500.0f	);
	vertices[9].position  = Vec3f( +500.0f	, +500.0f	, +500.0f	);
	vertices[10].position = Vec3f( -500.0f	, +500.0f	, +500.0f	);
	vertices[11].position = Vec3f( -500.0f	, +500.0f	, -500.0f	);
								   	     	    	  
	vertices[12].position = Vec3f( +500.0f	, -500.0f	, -500.0f	);
	vertices[13].position = Vec3f( +500.0f	, -500.0f	, +500.0f	);
	vertices[14].position = Vec3f( +500.0f	, +500.0f	, +500.0f	);
	vertices[15].position = Vec3f( +500.0f	, +500.0f	, -500.0f	);
								   	     	    	  
	vertices[16].position = Vec3f( -500.0f	, -500.0f	, +500.0f	);
	vertices[17].position = Vec3f( -500.0f	, +500.0f	, +500.0f	);
	vertices[18].position = Vec3f( +500.0f	, +500.0f	, +500.0f	);
	vertices[19].position = Vec3f( +500.0f	, -500.0f	, +500.0f	);
								   	     	    	  
	vertices[20].position = Vec3f( -500.0f	, -500.0f	, -500.0f	);
	vertices[21].position = Vec3f( +500.0f	, -500.0f	, -500.0f	);
	vertices[22].position = Vec3f( +500.0f	, +500.0f	, -500.0f	);
	vertices[23].position = Vec3f( -500.0f	, +500.0f	, -500.0f	);

	vertices[0].texCoords  = Vec2f( 1.0f , 1.0f );
	vertices[1].texCoords  = Vec2f( 1.0f , 0.0f );
	vertices[2].texCoords  = Vec2f( 0.0f , 0.0f );
	vertices[3].texCoords  = Vec2f( 0.0f , 1.0f );

	vertices[4].texCoords  = Vec2f( 1.0f , 1.0f );
	vertices[5].texCoords  = Vec2f( 1.0f , 0.0f );
	vertices[6].texCoords  = Vec2f( 0.0f , 0.0f );
	vertices[7].texCoords  = Vec2f( 0.0f , 1.0f );

	vertices[8].texCoords  = Vec2f( 1.0f , 1.0f );
	vertices[9].texCoords  = Vec2f( 1.0f , 0.0f );
	vertices[10].texCoords = Vec2f( 0.0f , 0.0f );
	vertices[11].texCoords = Vec2f( 0.0f , 1.0f );

	vertices[12].texCoords = Vec2f( 1.0f , 1.0f );
	vertices[13].texCoords = Vec2f( 1.0f , 0.0f );
	vertices[14].texCoords = Vec2f( 0.0f , 0.0f );
	vertices[15].texCoords = Vec2f( 0.0f , 1.0f );

	vertices[16].texCoords = Vec2f( 1.0f , 1.0f );
	vertices[17].texCoords = Vec2f( 1.0f , 0.0f );
	vertices[18].texCoords = Vec2f( 0.0f , 0.0f );
	vertices[19].texCoords = Vec2f( 0.0f , 1.0f );

	vertices[20].texCoords = Vec2f( 1.0f , 1.0f );
	vertices[21].texCoords = Vec2f( 1.0f , 0.0f );
	vertices[22].texCoords = Vec2f( 0.0f , 0.0f );
	vertices[23].texCoords = Vec2f( 0.0f , 1.0f );

	vertices[0].color  = RGBA( (unsigned char)170 , 68 , 0  , 255);
	vertices[1].color  = RGBA( (unsigned char)26  , 19 , 79 , 255);
	vertices[2].color  = RGBA( (unsigned char)26  , 19 , 79 , 255);
	vertices[3].color  = RGBA( (unsigned char)170 , 68 , 0  , 255);
						 									
	vertices[4].color  = RGBA( (unsigned char)170 , 68 , 0  , 255);
	vertices[5].color  = RGBA( (unsigned char)26  , 19 , 79 , 255);
	vertices[6].color  = RGBA( (unsigned char)26  , 19 , 79 , 255);
	vertices[7].color  = RGBA( (unsigned char)170 , 68 , 0  , 255);
						 									
	vertices[8].color  = RGBA( (unsigned char)170 , 68 , 0  , 255);
	vertices[9].color  = RGBA( (unsigned char)26  , 19 , 79 , 255);
	vertices[10].color = RGBA( (unsigned char)26  , 19 , 79 , 255);
	vertices[11].color = RGBA( (unsigned char)170 , 68 , 0  , 255);
				 		 									
	vertices[12].color = RGBA( (unsigned char)170 , 68 , 0  , 255);
	vertices[13].color = RGBA( (unsigned char)26  , 19 , 79 , 255);
	vertices[14].color = RGBA( (unsigned char)26  , 19 , 79 , 255);
	vertices[15].color = RGBA( (unsigned char)170 , 68 , 0  , 255);
				 		 									
	vertices[16].color = RGBA( (unsigned char)26  , 19 , 79 , 255);
	vertices[17].color = RGBA( (unsigned char)26  , 19 , 79 , 255);
	vertices[18].color = RGBA( (unsigned char)26  , 19 , 79 , 255);
	vertices[19].color = RGBA( (unsigned char)26  , 19 , 79 , 255);
				 		 									
	vertices[20].color = RGBA( (unsigned char)170 , 68 , 0  , 255);
	vertices[21].color = RGBA( (unsigned char)170 , 68 , 0  , 255);
	vertices[22].color = RGBA( (unsigned char)170 , 68 , 0  , 255);
	vertices[23].color = RGBA( (unsigned char)170 , 68 , 0  , 255);

	for(size_t index = 0; index < 24; ++index)
	{
		m_vertices.push_back(vertices[index]);
	}
}


void SkyBox::GenerateTexture()
{
	int numChannelOfRGBA = 4;
	const int imgBufferSize = m_textureSize.x * m_textureSize.y * numChannelOfRGBA;
	unsigned char* imageData = new unsigned char[imgBufferSize];
	memset(imageData,0,imgBufferSize);
	for(int count = 0; count < 1000; ++count)
	{
		int x = random( 1 , m_textureSize.x ) - 1;
		int y = random( 1 , m_textureSize.y ) - 1;
		int index = (x + y * m_textureSize.x) * 4;
		imageData[ index + 0 ] = (unsigned char)random(160,224);
		imageData[ index + 1 ] = (unsigned char)random(160,224);
		imageData[ index + 2 ] = (unsigned char)random(160,224);
		imageData[ index + 3 ] = 255;
	}

	m_textureStar = OpenGLRenderer::LoadTextureFromData(imageData,m_textureSize);
	delete imageData;
}


void SkyBox::Update(float deltaSecond , Camera3D camera)
{
	m_moonDegrees += 5.0f * deltaSecond;
	float moonRadians = degree2radians(m_moonDegrees);
	m_moonPosition = Vec3f( 0.0f , m_moonDistance * cos(moonRadians) , m_moonDistance * sin(moonRadians));
	m_cameraRightVector = camera.m_forwardVector.crossProductWith( Vec3f(0,0,1) );
	m_cameraUpVector = camera.m_forwardVector.crossProductWith( m_cameraRightVector );
}


void SkyBox::Render()
{
	if( m_vertices.size() )
	{
		for(size_t index=0; index < m_vertices.size(); ++index)
			m_vertices[index].texCoords.y += 0.0001f;
		OpenGLRenderer::BindTexture(m_textureStar);
		OpenGLRenderer::DrawVertexWithVertexArray( m_vertices , OpenGLRenderer::SHAPE_QUADS );
	} 

	OpenGLRenderer::PointSize(150.0f);
	Vertex_PCT vertex;
	vertex.position = m_moonPosition;
	OpenGLRenderer::DrawVertexWithVertexArray(&vertex,1,OpenGLRenderer::SHAPE_POINTS);
	
// 	Vertex_PCT vertices[4];
// 	vertices[0].position = m_moonPosition + m_cameraUpVector*200 + m_cameraRightVector*200;
// 	vertices[1].position = m_moonPosition + m_cameraUpVector*200 - m_cameraRightVector*200;
// 	vertices[2].position = m_moonPosition - m_cameraUpVector*200 - m_cameraRightVector*200;
// 	vertices[3].position = m_moonPosition - m_cameraUpVector*200 + m_cameraRightVector*200;
// 	
// 	vertices[0].color = RGBA(0.8f,0.0f,0.8f,1.0f);
// 	vertices[1].color = RGBA(0.8f,0.0f,0.8f,1.0f);
// 	vertices[2].color = RGBA(0.8f,0.0f,0.8f,1.0f);
// 	vertices[3].color = RGBA(0.8f,0.0f,0.8f,1.0f);
// 	
// 	OpenGLRenderer::DrawVertexWithVertexArray(vertices,4,OpenGLRenderer::SHAPE_POINTS);
}

}
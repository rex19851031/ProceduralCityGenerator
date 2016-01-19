#include "TextureGenerator.hpp"

#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Core\HenryFunctions.hpp"
#include "GameCode\Noise.hpp"


namespace Henry
{

TextureGenerator::TextureGenerator(Vec2i numOfWindow , Vec2i windowSize , int numOfTexture)
	: m_numberOfWindow(numOfWindow)
	, m_windowSize(windowSize)
	, m_textureSize(numOfWindow * windowSize)
{
	Vertex_PCT vertices[4];
	vertices[0].position = Vec3f( -1.0f , 0.0f , 0.0f );
	vertices[1].position = Vec3f( -1.0f , 2.0f , 0.0f );
	vertices[2].position = Vec3f( -1.0f , 2.0f , 2.0f );
	vertices[3].position = Vec3f( -1.0f , 0.0f , 2.0f );

	vertices[0].texCoords = Vec2f( 0.0f , 1.0f );
	vertices[1].texCoords = Vec2f( 1.0f , 1.0f );
	vertices[2].texCoords = Vec2f( 1.0f , 0.0f );
	vertices[3].texCoords = Vec2f( 0.0f , 0.0f );

	for(int index = 0; index < 4; ++index)
	{
		vertices[index].color = RGBA();
		m_vertices.push_back( vertices[index] );
	}

	for(int index = 0; index < numOfTexture; ++index)
		GenerateWindowTexture();

	GenerateRoofTexture();
	GenerateBlockTexture();
}


TextureGenerator::~TextureGenerator(void)
{
}


void TextureGenerator::GenerateBlockTexture()
{
	int numChannelOfRGBA = 4;
	const int imgBufferSize = m_textureSize.x * m_textureSize.y * numChannelOfRGBA;
	unsigned char* imageData = new unsigned char[imgBufferSize];

	for(int x=0; x < m_textureSize.x; ++x)
	{
		for(int y=0; y < m_textureSize.y; ++y)
		{
			float perlinNoise = ComputePerlinNoiseValueAtPosition2D( Vector2( (float)x*25 , (float)y*25 ) , 250.f , 10 , 80.0f , 0.5f );
			int value = (int)(abs(perlinNoise) * 10);
			value = value > 192 ? 192 : value;

			int index = (x + y * m_textureSize.x) * 4;
			imageData[ index + 0 ] = 32;
			imageData[ index + 1 ] = (unsigned char)(value < 128 ? 128 : value);
			imageData[ index + 2 ] = 32;
			imageData[ index + 3 ] = 255;
		}
	}

	m_blockTextureID = OpenGLRenderer::LoadTextureFromData(imageData,m_textureSize);
	delete imageData;
}


void TextureGenerator::GenerateWindowTexture()
{
	int numChannelOfRGBA = 4;
	const int imgBufferSize = m_textureSize.x * m_textureSize.y * numChannelOfRGBA;
	unsigned char* imageData = new unsigned char[imgBufferSize];
	unsigned char* specularData = new unsigned char[imgBufferSize];
	float* grayScale = new float[m_numberOfWindow.x * m_numberOfWindow.y];

	int min = 0;
	int max = 10;
	int continusOn = 0;
	int continusOff = 0;
	int nextMaximumContinusNumber = random(10,20);
	bool previousIsLight = false;

	for(int index=0; index < m_numberOfWindow.x * m_numberOfWindow.y; ++index)
	{
		grayScale[index] = random(min , max) * 0.1f;
		if( grayScale[index] > 0.5f && continusOn < nextMaximumContinusNumber)
		{
			++continusOn;
			min = 6;
			max = 10;
			previousIsLight = true;
		}

		if( grayScale[index] <= 0.5f && continusOff < nextMaximumContinusNumber)
		{
			++continusOff;
			min = 0;
			max = 4;
			previousIsLight = false;
		}

		if(continusOn >= nextMaximumContinusNumber || continusOff >= nextMaximumContinusNumber)
		{
			nextMaximumContinusNumber = random(10,20);
			continusOn = 0;
			continusOff = 0;
			min = previousIsLight ? 0 : 4;
			max = previousIsLight ? 6 : 10;
		}
	}

	for( int x = 0; x < m_textureSize.x; ++x)
	{
		for( int y = 0; y < m_textureSize.y; ++y)
		{			
			int index = (x + y * m_textureSize.x) * 4;
			int windowIndex = (y / m_windowSize.y * m_numberOfWindow.x) + x / m_windowSize.x;

			if( x % m_windowSize.x == 0 || (x+1) % m_windowSize.x == 0 ||
				y % m_windowSize.y == 0 || (y+1) % m_windowSize.y == 0 )
			{
				imageData[ index + 0 ] = 0;
				imageData[ index + 1 ] = 0;
				imageData[ index + 2 ] = 0;
				imageData[ index + 3 ] = 255;

				specularData[ index + 0 ] = 0;
				specularData[ index + 1 ] = 0;
				specularData[ index + 2 ] = 0;
				specularData[ index + 3 ] = 255;
			}
 			else
 			{
				float percentOfNoise = 1.0f - (y % m_windowSize.y) / (float)m_windowSize.y;

				imageData[ index + 0 ] = (unsigned char)(255 * grayScale[windowIndex] * percentOfNoise /** getRandomPercent()*/);
				imageData[ index + 1 ] = (unsigned char)(255 * grayScale[windowIndex] * percentOfNoise /** getRandomPercent()*/);
				imageData[ index + 2 ] = (unsigned char)(255 * grayScale[windowIndex] * percentOfNoise /** getRandomPercent()*/);
				imageData[ index + 3 ] = 255;

				specularData[ index + 0 ] = 128;
				specularData[ index + 1 ] = 128;
				specularData[ index + 2 ] = 128;
				specularData[ index + 3 ] = 255;

				if( (x-1) % m_windowSize.y == 0 || (x+2) % m_windowSize.y == 0)
				{
					imageData[ index + 0 ] = (unsigned char)(random(128,255) * grayScale[windowIndex]);
					imageData[ index + 1 ] = (unsigned char)(random(128,255) * grayScale[windowIndex]);
					imageData[ index + 2 ] = (unsigned char)(random(128,255) * grayScale[windowIndex]);
					imageData[ index + 3 ] = 255;
				}
 			}
		}
	}

	//RGBA windowColor;
	m_windowTextureIDs.push_back(OpenGLRenderer::LoadTextureFromData( imageData , m_textureSize ));
	m_specularTextureID = OpenGLRenderer::LoadTextureFromData( specularData , m_textureSize);
	
	delete imageData;
	delete specularData;
}


void TextureGenerator::Render(size_t index)
{
	if(index < m_windowTextureIDs.size())
		OpenGLRenderer::BindTexture(m_windowTextureIDs[index]);

// 	if(index < m_roofTextureIDs.size())
// 		OpenGLRenderer::BindTexture(m_roofTextureIDs[index]);

	OpenGLRenderer::DrawVertexWithVertexArray( m_vertices , OpenGLRenderer::SHAPE_QUADS );
}


void TextureGenerator::GenerateRoofTexture()
{
	int numChannelOfRGBA = 4;
	const int imgBufferSize = m_textureSize.x * m_textureSize.y * numChannelOfRGBA;
	unsigned char* imageData = new unsigned char[imgBufferSize];

	int numOfSegments = 50;
	float oneOverSegments = 1.0f / (float)numOfSegments;
	int radius = 10;
	Vec2i center( m_textureSize.x >> 1 , m_textureSize.y >> 1 );
	
	for(int index = 0; index < imgBufferSize; ++index)
	{
		imageData[ index ] = 0;
		if( (index+1) % 4 == 0)
			imageData[ index ] = 255;
	}

	for(int count = 0; count < numOfSegments; ++count)
	{
		float theta = degree2radians( 360.0f * count * oneOverSegments);
		float x = center.x + radius * cos(theta);
		float y = center.y + radius * sin(theta);
		int index = (int)(x + y * m_textureSize.x) * 4;
		imageData[ index + 0 ] = 128;
		imageData[ index + 1 ] = 128;
		imageData[ index + 2 ] = 128;
		imageData[ index + 3 ] = 255;
	}

	//RGBA windowColor;
	m_roofTextureIDs.push_back(OpenGLRenderer::LoadTextureFromData( imageData , m_textureSize ));
	
	delete imageData;
}


};
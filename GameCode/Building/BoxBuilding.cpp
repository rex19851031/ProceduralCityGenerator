#include "BoxBuilding.hpp"

#include "Engine\Core\HenryFunctions.hpp"
#include "Engine\Renderer\OpenGLRenderer.hpp"


namespace Henry
{

BoxBuilding::BoxBuilding(int generateType , const SpotAttributes& sa) : Building( sa )
{
	m_aabb.min = Vec2f(50.0f,50.0f);
	m_aabb.max = Vec2f(-50.0f,-50.0f);
	Generate(generateType);
}


BoxBuilding::~BoxBuilding(void)
{
	for(size_t index=0; index < m_vboIDs.size(); ++index)
		OpenGLRenderer::DeleteBuffer(m_vboIDs[index]);
}


void BoxBuilding::Render()
{
	Building::Render();
	if(m_isDirty)
	{
		std::map<int,std::vector<Vertex_PCTN>>::iterator it = m_verticesLists.begin();
		size_t count = 0;

		while(it != m_verticesLists.end())
		{
			if(count == m_vboIDs.size())
			{
				int vboID = 0;
				OpenGLRenderer::GenerateBuffers(&vboID);
				m_vboIDs.push_back(vboID);
				m_numOfVertices.push_back(it->second.size());
			}

			//OpenGLRenderer::DrawVertexWithVertexArray(it->second,OpenGLRenderer::SHAPE_TRIANGLES);
			OpenGLRenderer::BufferData( it->second , m_vboIDs[count] );
			++it;
			++count;
		}

		m_isDirty = false;
	}
	else
	{
		for(size_t index=0; index < m_vboIDs.size(); ++index)
		{			
			int textureID;
			if(index + 1 > m_textureIDs.size() || !m_enableTexture)
				textureID = OpenGLRenderer::DIFFUSE_TEXTURE_ID;
			else
				textureID = m_textureIDs[index];
			
			OpenGLRenderer::BindTexture(textureID);
			OpenGLRenderer::DrawVertexWithVertexBufferObject(m_vboIDs[index],m_numOfVertices[index],OpenGLRenderer::SHAPE_TRIANGLES,Henry::Vertex_PCTN());
		}
	}
}


void BoxBuilding::Generate(int generateType)
{
	m_centerPoint = Vec2f( 0.5f , 0.5f );
	m_height = 0.0f;

	std::vector<Vertex_PCTN> empty;
	std::map<int,std::vector<Vertex_PCTN>>::iterator it = m_verticesLists.begin();

	while(it != m_verticesLists.end())
	{
		std::vector<Vertex_PCTN> vertices = it->second;
		vertices.clear();
		vertices = empty;
		it = m_verticesLists.erase(it);
	}

	if(generateType == 0)
		generateType = random((int)1,7);
	
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
	case 4:
		GenerateTypeFour();
		break;
	case 5:
		GenerateTypeFive();
		break;
	case 6:
		GenerateTypeSix();
		break;
	case 7:
		GenerateTypeSeven();
		break;
	}

	CorrectLocation();
	std::map<int,std::vector<Vertex_PCTN>>::iterator draw_it = m_verticesLists.begin();

	while(draw_it != m_verticesLists.end())
	{
		int count = 0;
		for(size_t index=0; index < draw_it->second.size(); ++index)
		{
			count++;
			if(count == 3)
			{
				Vec3f tangent;
				Vec3f biTangent;
				ComputeSurfaceTangentsAtVertex(	tangent ,
												biTangent ,
												draw_it->second[index-1].normals.normal ,
												draw_it->second[index-1].position , 
												draw_it->second[index-1].texCoords ,
												draw_it->second[index-2].position , 
												draw_it->second[index-2].texCoords ,
												draw_it->second[index-0].position ,
												draw_it->second[index-0].texCoords );

				draw_it->second[index-2].normals.tangent = tangent;
				draw_it->second[index-1].normals.tangent = tangent;
				draw_it->second[index-0].normals.tangent = tangent;

				draw_it->second[index-2].normals.biTangent = biTangent;
				draw_it->second[index-1].normals.biTangent = biTangent;
				draw_it->second[index-0].normals.biTangent = biTangent;

				count = 0;
			}
		}

		++draw_it;
	}
}


void BoxBuilding::MakeBox(AABB2 scope , float currentHeight , float height , RGBA color , int listIndex , int rotateDegrees)
{
	if(currentHeight + height > m_height)
		m_height = currentHeight + height;

	Vertex_PCTN vertex[36];

	vertex[0].position	= Vec3f( scope.min.x , scope.min.y , currentHeight	      );
	vertex[1].position	= Vec3f( scope.max.x , scope.min.y , currentHeight		  );
	vertex[2].position	= Vec3f( scope.max.x , scope.min.y , currentHeight + height );

	vertex[3].position	= Vec3f( scope.min.x , scope.min.y , currentHeight		  );
	vertex[4].position	= Vec3f( scope.max.x , scope.min.y , currentHeight + height );
	vertex[5].position	= Vec3f( scope.min.x , scope.min.y , currentHeight + height );

	vertex[6].position	= Vec3f( scope.min.x , scope.min.y , currentHeight			);
	vertex[7].position	= Vec3f( scope.min.x , scope.max.y , currentHeight + height );
	vertex[8].position	= Vec3f( scope.min.x , scope.max.y , currentHeight			);

	vertex[9].position	= Vec3f( scope.min.x , scope.max.y , currentHeight + height );
	vertex[10].position = Vec3f( scope.min.x , scope.min.y , currentHeight			);
	vertex[11].position = Vec3f( scope.min.x , scope.min.y , currentHeight + height );

	vertex[12].position	= Vec3f( scope.min.x , scope.max.y , currentHeight + height );
	vertex[13].position = Vec3f( scope.max.x , scope.max.y , currentHeight			);
	vertex[14].position = Vec3f( scope.min.x , scope.max.y , currentHeight			);

	vertex[15].position	= Vec3f( scope.min.x , scope.max.y , currentHeight + height );
	vertex[16].position = Vec3f( scope.max.x , scope.max.y , currentHeight + height );
	vertex[17].position = Vec3f( scope.max.x , scope.max.y , currentHeight			);

	vertex[18].position	= Vec3f( scope.max.x , scope.max.y , currentHeight			);
	vertex[19].position = Vec3f( scope.max.x , scope.min.y , currentHeight + height );
	vertex[20].position = Vec3f( scope.max.x , scope.min.y , currentHeight			);

	vertex[21].position	= Vec3f( scope.max.x , scope.min.y , currentHeight + height );
	vertex[22].position = Vec3f( scope.max.x , scope.max.y , currentHeight			);
	vertex[23].position = Vec3f( scope.max.x , scope.max.y , currentHeight + height );

	vertex[24].position	= Vec3f( scope.min.x , scope.min.y , currentHeight + height );
	vertex[25].position = Vec3f( scope.max.x , scope.min.y , currentHeight + height );
	vertex[26].position = Vec3f( scope.min.x , scope.max.y , currentHeight + height );

	vertex[27].position	= Vec3f( scope.min.x , scope.max.y , currentHeight + height );
	vertex[28].position = Vec3f( scope.max.x , scope.min.y , currentHeight + height );
	vertex[29].position = Vec3f( scope.max.x , scope.max.y , currentHeight + height );

	vertex[30].position	= Vec3f( scope.min.x , scope.min.y , currentHeight			);
	vertex[31].position = Vec3f( scope.min.x , scope.max.y , currentHeight			);
	vertex[32].position = Vec3f( scope.max.x , scope.max.y , currentHeight			);

	vertex[33].position	= Vec3f( scope.min.x , scope.min.y , currentHeight			);
	vertex[34].position = Vec3f( scope.max.x , scope.max.y , currentHeight			);
	vertex[35].position = Vec3f( scope.max.x , scope.min.y , currentHeight			);

	Vec2f texMin( 0.0f , 0.0f );
	Vec2f texMaxX( (scope.max.x - scope.min.x) * 0.15625f , height * 0.15625f );
	Vec2f texMaxY( (scope.max.y - scope.min.y) * 0.15625f , height * 0.15625f );

	vertex[0].texCoords	 = Vec2f( texMin.x  , texMaxX.y );
	vertex[1].texCoords	 = Vec2f( texMaxX.x , texMaxX.y );
	vertex[2].texCoords	 = Vec2f( texMaxX.x , texMin.y  );

	vertex[3].texCoords	 = Vec2f( texMin.x  , texMaxX.y );
	vertex[4].texCoords	 = Vec2f( texMaxX.x , texMin.y  );
	vertex[5].texCoords	 = Vec2f( texMin.x  , texMin.y  );

	vertex[6].texCoords	 = Vec2f( texMaxY.x , texMaxY.y );
	vertex[7].texCoords	 = Vec2f( texMin.x  , texMin.y  );
	vertex[8].texCoords	 = Vec2f( texMin.x  , texMaxY.y );

	vertex[9].texCoords	 = Vec2f( texMin.x  , texMin.y  );
	vertex[10].texCoords = Vec2f( texMaxY.x , texMaxY.y );
	vertex[11].texCoords = Vec2f( texMaxY.x , texMin.y  );

	vertex[12].texCoords = Vec2f( texMaxX.x , texMin.y  );
	vertex[13].texCoords = Vec2f( texMin.x  , texMaxX.y );
	vertex[14].texCoords = Vec2f( texMaxX.x , texMaxX.y );

	vertex[15].texCoords = Vec2f( texMaxX.x , texMin.y  );
	vertex[16].texCoords = Vec2f( texMin.x  , texMin.y  );
	vertex[17].texCoords = Vec2f( texMin.x  , texMaxX.y );

	vertex[18].texCoords = Vec2f( texMaxY.x , texMaxY.y );
	vertex[19].texCoords = Vec2f( texMin.x  , texMin.y  );
	vertex[20].texCoords = Vec2f( texMin.x  , texMaxY.y );

	vertex[21].texCoords = Vec2f( texMin.x  , texMin.y  );
	vertex[22].texCoords = Vec2f( texMaxY.x , texMaxY.y );
	vertex[23].texCoords = Vec2f( texMaxY.x , texMin.y  );

	//Vec2f centerPoint = (dimentions.max - dimentions.min) * 0.5f + dimentions.min;
	bool aabbChanged = false;

	for(int index=0; index < 36; index++)
	{
		vertex[index].color = color;

		Vec2f offsetToCenter = ( m_centerPoint - Vec2f( vertex[index].position.x , vertex[index].position.y ) );// * 0.5f;
		float radians = degree2radians((float)rotateDegrees);
		vertex[index].position.x = m_centerPoint.x + ( offsetToCenter.x * cos(radians) ) - ( offsetToCenter.y * sin(radians) );
		vertex[index].position.y = m_centerPoint.y + ( offsetToCenter.x * sin(radians) ) + ( offsetToCenter.y * cos(radians) );

		m_verticesLists[listIndex].push_back( vertex[index] );

		if( vertex[index].position.x > m_aabb.max.x || vertex[index].position.y > m_aabb.max.y ||
			vertex[index].position.x < m_aabb.min.x || vertex[index].position.y < m_aabb.min.y )
			aabbChanged = true;

		if(vertex[index].position.x > m_aabb.max.x)
			m_aabb.max.x = vertex[index].position.x;
		if(vertex[index].position.y > m_aabb.max.y)
			m_aabb.max.y = vertex[index].position.y;
		if(vertex[index].position.x < m_aabb.min.x)
			m_aabb.min.x = vertex[index].position.x;
		if(vertex[index].position.y < m_aabb.min.y)
			m_aabb.min.y = vertex[index].position.y;
	}

	if(aabbChanged)
		Building::Generate();
}


bool BoxBuilding::IsInside(AABB2 area , Vec2f checkPoint)
{
	if( checkPoint.x < area.max.x && checkPoint.y < area.max.y &&
		checkPoint.x > area.min.x && checkPoint.y > area.min.y )
		return true;

	return false;
}


void BoxBuilding::CorrectLocation()
{
	std::map<int,std::vector<Vertex_PCTN>>::iterator it = m_verticesLists.begin();
	Vec2f offset( m_aabb.min * -1 );
	
	while(it != m_verticesLists.end())
	{
		for(size_t index = 0; index < it->second.size(); ++index)
		{
			it->second[index].position.x += offset.x;
			it->second[index].position.y += offset.y;
		}

		++it;
	}

	m_aabb.min += offset;
	m_aabb.max += offset;
	Building::Generate();
}


void BoxBuilding::GenerateTypeOne()
{
	RGBA BuildingColor((unsigned char)random(0,2),(unsigned char)random(0,16),(unsigned char)random(16,32),64);
	AABB2 scope;
	float densityX = 1.0f - (int)( m_spotAttributes.density * random( 0 , 7 ) ) * 0.1f;
	float densityY = 1.0f - (int)( m_spotAttributes.density * random( 0 , 7 ) ) * 0.1f;
	float height = 10.0f * random( 10 , (int)(100 * m_spotAttributes.richness) ) * 0.01f;//10.0f * ( m_spotAttributes.richness ) * random( (int)(10 * m_spotAttributes.richness) , 95 ) * 0.1f;
	int degrees = random( 0 , 360 );
	scope.min = Vec2f( 0.0f , 0.0f );
	scope.max = Vec2f( densityX , densityY );
	m_centerPoint = (scope.max - scope.min) * 0.5f;
	MakeBox( scope , 0.0f , height , BuildingColor , 0 , degrees);
	
	int percent_min_X = random( 1 , 5 );
	int percent_min_Y = random( 1 , 5 );
	scope.min.x = densityX * percent_min_X * 0.1f;
	scope.min.y = densityY * percent_min_Y * 0.1f;

	int percent_max_X = percent_min_X + random( 2 , 10 - percent_min_X );
	int percent_max_Y = percent_min_Y + random( 2 , 10 - percent_min_Y );
	scope.max.x = densityX * percent_max_X * 0.1f;
	scope.max.y = densityY * percent_max_Y * 0.1f;

	MakeBox( scope , height , 0.1f , BuildingColor , 1 , degrees);
}


void BoxBuilding::GenerateTypeTwo()
{
	RGBA BuildingColor((unsigned char)random(0,2),(unsigned char)random(0,16),(unsigned char)random(16,32),64);
	float maximumHeight = 10.0f * random( 10 , (int)(100 * m_spotAttributes.richness) ) * 0.01f;
	float accumulateHeight = 0.0f;
	Vec2f maximumAABB(1.0f,1.0f);
	Vec2f centerPoint;
	
	AABB2 result;
	int densityX = random( 0 , 6 );
	int densityY = random( 0 , 6 );
	result.min = Vec2f( 0.0f , 0.0f );
	result.max = result.min + Vec2f( maximumAABB.x - (int)( m_spotAttributes.density * densityX ) * 0.1f , maximumAABB.y - (int)( m_spotAttributes.density * densityY ) * 0.1f );
	centerPoint = ( result.max - result.min ) * 0.5f;
	float offset = 0.0001f;

	MakeBox(result , 0.0f , maximumHeight , BuildingColor);
	for( int index = 0; index < 3; ++index )
	{
		accumulateHeight += random( 2 , 10 - (int)(accumulateHeight * 10) )  * 0.1f;
		result = AABB2(); 

		while( !IsInside(result , centerPoint) )
		{
			Vec2f aabb_min = m_aabb.min + Vec2f( maximumAABB.x * random(1,9) * 0.1f , maximumAABB.y * random(1,9) * 0.1f );
			Vec2f aabb_max;
			Vec2f distanceFromMinToCenter = centerPoint - aabb_min;

			if( distanceFromMinToCenter.x >= 0.0f )
			{
				if( abs(distanceFromMinToCenter.x) >= abs(distanceFromMinToCenter.y) )
				{
					if( distanceFromMinToCenter.y >= 0.0f )
					{
						//1
						aabb_max.x = aabb_min.x + distanceFromMinToCenter.x * 2.0f;
						aabb_max.y = m_aabb.max.y + random(1 , (int)(( maximumAABB.y - m_aabb.max.y ) * 10) ) * 0.1f;
					}
					else
					{
						//2
						aabb_max.x = aabb_min.x + distanceFromMinToCenter.x * 2.0f;
						aabb_max.y = aabb_min.y;
						aabb_min.y = m_aabb.min.y - random( 1 , 10 ) * 0.1f;
					} 
				}
				else
				{
					if( distanceFromMinToCenter.y >= 0.0f )
					{
						//3
						aabb_max.x = m_aabb.max.x + random( 1 , (int)(( maximumAABB.x - m_aabb.max.x ) * 10) ) * 0.1f;
						aabb_max.y = aabb_min.y + distanceFromMinToCenter.y * 2.0f;
					}
					else
					{
						//4
						aabb_max.x = m_aabb.max.x + random( 1 , (int)(( maximumAABB.x - m_aabb.max.x ) * 10) ) * 0.1f;
						aabb_max.y = aabb_min.y;
						aabb_min.y = aabb_min.y + distanceFromMinToCenter.y *2.0f;
					}
				}
			}
			else
			{
				if( abs(distanceFromMinToCenter.x) >= abs(distanceFromMinToCenter.y) )
				{
					if( distanceFromMinToCenter.y >= 0.0f )
					{
						//5
						aabb_max.x = aabb_min.x;
						aabb_max.y = m_aabb.max.y + random( 1 , (int)(( maximumAABB.y - m_aabb.max.y ) * 10 )) * 0.1f;
						aabb_min.x = aabb_min.x + distanceFromMinToCenter.x * 2.0f;
					}
					else
					{
						//6
						aabb_max = aabb_min;
						aabb_min.x = aabb_min.x + distanceFromMinToCenter.x * 2.0f;
						aabb_min.y = m_aabb.min.y - random( 1 , 10 ) * 0.1f;
					}
				}
				else
				{
					if( distanceFromMinToCenter.y >= 0.0f )
					{
						//7
						aabb_max.x = aabb_min.x;
						aabb_max.y = aabb_min.y + distanceFromMinToCenter.y * 2.0f;
						aabb_min.x = m_aabb.min.x - random( 1 , 10 ) * 0.1f;
					}
					else
					{
						//8
						aabb_max = aabb_min;
						aabb_min.x = m_aabb.min.x -  random( 1 , 10 ) * 0.1f;
						aabb_min.y = aabb_min.y + distanceFromMinToCenter.y * 2.0f;
					}
				}
			}

			result.min = aabb_min;
			result.max = aabb_max;
		}

		result.min = result.min + offset;
		result.max = result.max + offset;
		offset += 0.0001f;

		MakeBox(result , 0.0f , accumulateHeight , BuildingColor);
	}
}


void BoxBuilding::GenerateTypeThree()
{
	float maximumAdvanceHeight = 10.0f - (int)( (1.0f - m_spotAttributes.richness) * random( 0 , 90 ) ) * 0.1f;
	float currentHeight = 0.0f;
	Vec2f previous_aabb_min(0.0f,0.0f);
	Vec2f previous_aabb_max(1.0f,1.0f);
	Vec2f centerPoint = (previous_aabb_max - previous_aabb_min) * 0.5f;
	m_centerPoint = centerPoint;
	bool mainBuilding = true;

	for( ;  maximumAdvanceHeight > 0.5f ; )
	{
		Vec2f maximumAABB( (previous_aabb_max - previous_aabb_min) * 0.5f );
		float advanceHeight = random( 5 , (int)(maximumAdvanceHeight * 10) ) * 0.1f;
		Vec2f distanceWithCenter = maximumAABB * random(50.0f,95.0f) * 0.01f;

		Vec2f aabb_min = centerPoint - distanceWithCenter;
		Vec2f aabb_max = centerPoint + distanceWithCenter;

		if(mainBuilding)
		{
			MakeBox( AABB2( aabb_min , aabb_max ) , currentHeight , advanceHeight );
			mainBuilding = false;
		}
		else
			MakeBox( AABB2( aabb_min , aabb_max ) , currentHeight , advanceHeight , RGBA() , 1 );

		currentHeight += advanceHeight;
		previous_aabb_min = aabb_min;
		previous_aabb_max = aabb_max;
		maximumAdvanceHeight = advanceHeight;
	}
}


void BoxBuilding::GenerateTypeFour()
{
	float maximumAdvanceHeight = 2.0f;
	float currentHeight = 0.0f;
	float previousAdvancedHeight = maximumAdvanceHeight;
	Vec2f previous_aabb_min(0.0f,0.0f);
	Vec2f previous_aabb_max(1.0f,1.0f);
	Vec2f centerPoint = (previous_aabb_max - previous_aabb_min) * 0.5f;

	for( ;  maximumAdvanceHeight > 0.1f ; )
	{
		Vec2f maximumAABB( previous_aabb_max - previous_aabb_min );
		float advanceHeight = maximumAdvanceHeight * getRandomPercent();
		Vec2f distanceWithCenter = maximumAABB * getRandomPercent();
		if( distanceWithCenter.x > maximumAABB.x * 0.5f )
		{
			distanceWithCenter *= 0.7f;
		}

		Vec2f aabb_min = centerPoint - distanceWithCenter;
		Vec2f aabb_max = centerPoint + distanceWithCenter;

		MakeBox( AABB2( aabb_min , aabb_max ) , currentHeight , advanceHeight );

		currentHeight += advanceHeight;
		previous_aabb_min = aabb_min;
		previous_aabb_max = aabb_max;
		previousAdvancedHeight = advanceHeight;
		maximumAdvanceHeight = advanceHeight;
	}
}


void BoxBuilding::GenerateTypeFive()
{
	float maximumAdvanceHeight = 2.0f;
	float currentHeight = 0.0f;
	float minimumHeight = 0.3f;
	Vec2f previous_aabb_min(0.0f,0.0f);
	Vec2f previous_aabb_max(1.0f,1.0f);
	Vec2f maximumAABB;
	Vec2f minimumSizeRatio( 0.3f , 0.3f );

	maximumAABB = Vec2f( previous_aabb_max - previous_aabb_min ) * (Vec2f(1.0f,1.0f) - minimumSizeRatio);
	float advanceHeight = maximumAdvanceHeight * getRandomPercent();
	if(currentHeight == 0.0f && advanceHeight < minimumHeight)
		advanceHeight += minimumHeight;

	Vec2f aabb_min = Vec2f( maximumAABB.x * getRandomPercent() , maximumAABB.y * getRandomPercent() );
	Vec2f ratioAgainstMaximum = Vec2f(1.0f,1.0f) - (aabb_min / maximumAABB);
	aabb_min += previous_aabb_min;
	Vec2f aabb_max = aabb_min + Vec2f( maximumAABB.x * getRandomPercent() , maximumAABB.y * getRandomPercent() ) * ratioAgainstMaximum + maximumAABB * minimumSizeRatio;
	AABB2 previous_aabb( aabb_min , aabb_max );
	MakeBox( previous_aabb , 0.0f , advanceHeight );
	int currentConnection = 0;
	int maxConnection = 3;

	while( currentConnection != maxConnection )
	{
		maximumAABB = Vec2f( previous_aabb_max - previous_aabb_min ) * (Vec2f(1.0f,1.0f) - minimumSizeRatio);
		float advanceHeight = maximumAdvanceHeight * getRandomPercent();
		if(currentHeight == 0.0f && advanceHeight < minimumHeight)
			advanceHeight += minimumHeight;

		Vec2f aabb_min = Vec2f( maximumAABB.x * getRandomPercent() , maximumAABB.y * getRandomPercent() );
		Vec2f ratioAgainstMaximum = Vec2f(1.0f,1.0f) - (aabb_min / maximumAABB);
		aabb_min += previous_aabb_min;
		Vec2f aabb_max = aabb_min + Vec2f( maximumAABB.x * getRandomPercent() , maximumAABB.y * getRandomPercent() ) * ratioAgainstMaximum + maximumAABB * minimumSizeRatio;
		AABB2 newAABB( aabb_min , aabb_max );

		if( IsInside( previous_aabb , newAABB.min ) || IsInside( previous_aabb , Vec2f( newAABB.min.x , newAABB.max.y ) ) || 
			IsInside( previous_aabb , newAABB.max ) || IsInside( previous_aabb , Vec2f( newAABB.max.x , newAABB.min.y ) ) )
		{
 			if( !IsInside( previous_aabb , newAABB.min ) || !IsInside( previous_aabb , Vec2f( newAABB.min.x , newAABB.max.y ) ) || 
 				!IsInside( previous_aabb , newAABB.max ) || !IsInside( previous_aabb , Vec2f( newAABB.max.x , newAABB.min.y ) ) )
 			{
				++currentConnection;
				MakeBox( newAABB , 0.0f , advanceHeight );
			}
		}
	}
}


void BoxBuilding::GenerateTypeSix()
{
	float maximumAdvanceHeight = 2.0f;
	float currentHeight = maximumAdvanceHeight;
	float minimumHeight = 0.3f;
	Vec2f maximumAABB;
	Vec2f minimumSizeRatio( 0.3f , 0.3f );

	maximumAABB = Vec2f( 1.0f, 1.0f ) * (Vec2f(1.0f,1.0f) - minimumSizeRatio);
	float advanceHeight = maximumAdvanceHeight * getRandomPercent();
	if(currentHeight == 0.0f && advanceHeight < minimumHeight)
		advanceHeight += minimumHeight;

	Vec2f aabb_min = Vec2f( maximumAABB.x , maximumAABB.y ) * getRandomPercent();
	Vec2f ratioAgainstMaximum = Vec2f(1.0f,1.0f) - (aabb_min / maximumAABB);
	Vec2f aabb_max = aabb_min + Vec2f( maximumAABB.x , maximumAABB.y ) * getRandomPercent() * ratioAgainstMaximum + maximumAABB * minimumSizeRatio;

// 	Vec2f differenceToCenter = Vec2f( 0.5f , 0.5f ) - ( aabb_max - aabb_min ) * 0.5f;
// 	aabb_min -= differenceToCenter;
// 	aabb_max -= differenceToCenter;

	AABB2 result( aabb_min , aabb_max );
	MakeBox( result , 0.0f , maximumAdvanceHeight );

// 	maximumAdvanceHeight = advanceHeight;
// 	Vec2f centerPoint = Vec2f(aabb_max - aabb_min) * 0.5f + aabb_min;
// 	Vec2f previousDistance = centerPoint - aabb_min;
// 
// 	int currentConnection = 0;
// 	int maxConnection = 3;
// 
// 	while( currentConnection != maxConnection )
// 	{
// 		currentHeight -= 0.5f;
// 		Vec2f distanceFromMinToCenter;
// 
// 		int rollCount = 0;
// 		while( IsInside( result , aabb_min ) || distanceFromMinToCenter.x <= previousDistance.x || distanceFromMinToCenter.y <= previousDistance.y ) 
// 		{
// 			aabb_min = Vec2f( maximumAABB.x * getRandomPercent() , maximumAABB.y * getRandomPercent() );
// 			distanceFromMinToCenter = centerPoint - aabb_min;
// 			if( ++rollCount >= 1000 )
// 				return;
// 		}
// 
// 		aabb_max = aabb_min + distanceFromMinToCenter * 2.0f;
// 		MakeBox( AABB2( aabb_min , aabb_max ) , 0.0f , currentHeight , 45.0f );
// 
// 		aabb_min = centerPoint - Vec2f( distanceFromMinToCenter.y , distanceFromMinToCenter.x );
// 		aabb_max = centerPoint + Vec2f( distanceFromMinToCenter.y , distanceFromMinToCenter.x );
// 		MakeBox( AABB2( aabb_min , aabb_max ) , 0.0f , currentHeight , 45.0f );
// 
// 		previousDistance = distanceFromMinToCenter;
// 		++currentConnection;
// 	}
}


void BoxBuilding::GenerateTypeSeven()
{
	float maximumHeight = 2.0f;
	float currentHeight = 0.0f;
	float advanceHeight = 0.05f;
	int currentDegrees = 0;
	int advanceDegrees = 10;
	Vec2f maximumAABB( 1.0f , 1.0f );
	Vec2f minimumSizeRatio( 0.3f , 0.3f );

	maximumAABB = maximumAABB * (Vec2f(1.0f,1.0f) - minimumSizeRatio);
	Vec2f aabb_min = Vec2f( maximumAABB.x , maximumAABB.y ) * getRandomPercent();
	Vec2f ratioAgainstMaximum = Vec2f(1.0f,1.0f) - (aabb_min / maximumAABB);
	Vec2f aabb_max = aabb_min + Vec2f( maximumAABB.x , maximumAABB.y ) * getRandomPercent() * ratioAgainstMaximum + maximumAABB * minimumSizeRatio;
	AABB2 result( aabb_min , aabb_max );

	for( ; currentHeight < maximumHeight ; )
	{
		MakeBox( result , currentHeight , advanceHeight , RGBA() , 0 , currentDegrees);
		currentDegrees += advanceDegrees;
		currentHeight += advanceHeight;
	}
}


};
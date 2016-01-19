#include "TheGame.hpp"
#include "TheApplication.hpp"

#include "GameCode\GameCommon.hpp"
#include "GameCode\Building\BoxBuilding.hpp"
#include "GameCode\Building\CylinderBuilding.hpp"

#include "Engine\Renderer\OpenGLRenderer.hpp"
#include "Engine\Core\Time.hpp"
#include "Engine\Core\HenryFunctions.hpp"

#include <fstream>
#include <sstream>


namespace Henry
{

extern TheApplication* g_theApplication;


TheGame::TheGame(void)
{
	m_isQuitting = false;
	SetGameState(STARTING_UP);
	Initialize();
}


TheGame::~TheGame(void)
{
	delete m_terrain;
	delete m_material;
	delete m_lightMaterial;
	delete m_fboMaterial;
	delete m_blurMaterial;
	delete m_fetchBrightMaterial;
	delete m_combineMaterial;
	delete m_skybox;
	
	m_terrain				= nullptr;
	m_material				= nullptr;
	m_lightMaterial			= nullptr;
	m_fboMaterial			= nullptr;
	m_blurMaterial			= nullptr;
	m_fetchBrightMaterial	= nullptr;
	m_combineMaterial		= nullptr;
	m_skybox				= nullptr;
}


void TheGame::Input()
{
	if(m_input->PressedOnce(Inputs::INPUT_ESCAPE))
		SetGameState(QUITTING);

	switch(m_gameState)
	{
	case STARTING_UP:
		if(m_input->PressedOnce('R'))
		{
// 			std::vector<Building*> empty;
// 			m_buildings.clear();
// 			m_buildings = empty;
// 			GenerateBuildings();

 			std::vector<Block> empty;
 			m_blocks.clear();
 			m_blocks = empty;
 			GenerateBuildings();
		}

		if(m_input->PressedOnce(Inputs::INPUT_TAB))
			m_enableTexture = !m_enableTexture;
		
		if(m_input->PressedOnce('1'))
			m_terrain->m_drawDebug = !m_terrain->m_drawDebug;
		
		if(m_input->PressedOnce('2'))
			m_drawDebug = !m_drawDebug;
		break;
	};
}


void TheGame::Render()
{
	SetUpPerspectiveProjection();
	ApplyCameraTransform( m_camera );

	OpenGLRenderer::BindFrameBuffer(m_fbo.ID);
	OpenGLRenderer::FrameBufferColorTexture(m_fbo.colorTextureID);
	OpenGLRenderer::ClearScreen(0.0f,0.0f,0.2f);

	switch(m_gameState)
	{
	case STARTING_UP:

		m_skyBoxMaterial->Activate();
		m_skybox->Render();
		m_skyBoxMaterial->SendUniform( "u_cameraPosition" , m_camera.m_position );
		m_skyBoxMaterial->SendUniform( "u_fogStartDistance" , 0 );
		m_skyBoxMaterial->SendUniform( "u_fogEndDistance" , 25 );
		m_skyBoxMaterial->SendUniform( "u_fogColorAndIntensity" , RGBA(0.2f,0.2f,0.4f,0.5f) );

		m_material->Activate();
		m_objectToWorldMatrix.LoadIdentity();
		m_material->SendUniform( "u_objectToWorldMatrix" , m_objectToWorldMatrix );
		m_material->SendUniform( "u_cameraPosition" , m_camera.m_position );
		m_material->SendUniform( "u_fogStartDistance" , 8 );
		m_material->SendUniform( "u_fogEndDistance" , 25 );
		m_material->SendUniform( "u_fogColorAndIntensity" , RGBA(0.25f,0.25f,0.25f,0.4f) );
		OpenGLRenderer::BindWhiteTexture();
		m_terrain->Render();
		//RenderAxes();

		for(size_t index=0; index < m_blocks.size(); ++index)
		{
			m_objectToWorldMatrix.LoadIdentity();
			m_objectToWorldMatrix.ApplyTransform( (float)m_blocks[index].m_location.x , (float)m_blocks[index].m_location.y , 0.0f );
			m_material->SendUniform( "u_objectToWorldMatrix" , m_objectToWorldMatrix );
			m_blocks[index].Render();
		}

		//m_road.Render();
		//m_textureGenerator->Render(/*m_textureIndex*/);

		m_lightMaterial->Activate();
		m_lightMaterial->SendUniform( "u_lightPosition"  , Vec3f(m_skybox->m_moonPosition.y,m_skybox->m_moonPosition.x,m_skybox->m_moonPosition.z) );
		m_lightMaterial->SendUniform( "u_cameraPosition" , m_camera.m_position );
		m_lightMaterial->SendUniform( "u_fogStartDistance" , 8 );
		m_lightMaterial->SendUniform( "u_fogEndDistance" , 25 );
		m_lightMaterial->SendUniform( "u_fogColorAndIntensity" , RGBA(0.25f,0.25f,0.25f,0.4f) );
		
		if(m_enableTexture)
		{
			//OpenGLRenderer::BindTexture(OpenGLRenderer::NORMAL_TEXTURE_ID,1);
			OpenGLRenderer::BindTexture(m_textureGenerator->m_specularTextureID,2);
		}
		else
		{
			//OpenGLRenderer::BindTexture(0,1);
			OpenGLRenderer::BindTexture(0,2);
		}

// 		if(m_buildings.size() != 0)
// 		{
// 			m_objectToWorldMatrix.LoadIdentity();
// 			m_lightMaterial->SendUniform( "u_objectToWorldMatrix" , m_objectToWorldMatrix );
// 			m_buildings[0]->m_drawDebug = m_drawDebug;
// 			m_buildings[0]->m_enableTexture = m_enableTexture;
// 			m_buildings[0]->m_textureIDs[0] = m_textureGenerator->m_diffuseTextureIDs[m_textureIndex];
// 			m_buildings[0]->Render();
// 		}
		//m_material->Activate();

		if(m_blocks.size() != 0)
		{
			for(size_t index=0; index < m_blocks.size(); ++index)
			{
				for(size_t buildingIndex = 0; buildingIndex < m_blocks[index].m_buildings.size(); ++buildingIndex)
				{
					m_objectToWorldMatrix.LoadIdentity();
					m_objectToWorldMatrix.ApplyTransform((float)m_blocks[index].m_buildingLocations[buildingIndex].x , (float)m_blocks[index].m_buildingLocations[buildingIndex].y , 0.0f);
					m_lightMaterial->SendUniform( "u_objectToWorldMatrix" , m_objectToWorldMatrix );
					//m_blocks[index].m_buildings[buildingIndex]->m_textureIDs[0] = m_textureGenerator->m_windowTextureIDs[random(0,9)];
					m_blocks[index].m_buildings[buildingIndex]->m_enableTexture = m_enableTexture;
					m_blocks[index].m_buildings[buildingIndex]->m_drawDebug = m_drawDebug;
					m_blocks[index].m_buildings[buildingIndex]->Render();
				}
			}
		}
		break;
	};

	OpenGLRenderer::BindFrameBuffer(0);
	OpenGLRenderer::ClearScreen(0.f, 0.f, 0.f);
	DoPostEffect();
}


void TheGame::Update( float deltaSeconds )
{
	switch(m_gameState)
	{
	case STARTING_UP:
		m_skybox->Update(deltaSeconds,m_camera);
		break;
	case QUITTING:
		m_isQuitting = true;
		break;
	};

	m_timerForTexture += deltaSeconds;
	if(m_timerForTexture >= 0.1f)
	{
		m_timerForTexture = 0;
		m_textureIndex = random( 1 , m_textureGenerator->m_windowTextureIDs.size()) - 1;
	}

	UpdateCameraFromMouseAndKeyboard( m_camera, deltaSeconds );
}


void TheGame::RenderAxes()
{
	OpenGLRenderer::LineWidth(2.0f);
	float axisLength = 2.0f;
	float axisStart = 0.0f;
	Vertex_PosColor vertices[8];
	vertices[0].color = RGBA(1.0f,0.0f,0.0f,1.0f);
	vertices[0].position = Vec3f(axisStart,0.0f,0.0f);
	vertices[1].color = RGBA(1.0f,0.0f,0.0f,1.0f);
	vertices[1].position = Vec3f(axisLength,0.0f,0.0f);

	vertices[2].color = RGBA(0.0f,1.0f,0.0f,1.0f);
	vertices[2].position = Vec3f(0.0f,axisStart,0.0f);
	vertices[3].color = RGBA(0.0f,1.0f,0.0f,1.0f);
	vertices[3].position = Vec3f(0.0f,axisLength,0.0f);

	vertices[4].color = RGBA(0.0f,0.0f,1.0f,1.0f);
	vertices[4].position = Vec3f(0.0f,0.0f,axisStart);
	vertices[5].color = RGBA(0.0f,0.0f,1.0f,1.0f);
	vertices[5].position = Vec3f(0.0f,0.0f,axisLength);

	OpenGLRenderer::DrawVertexWithVertexArray(vertices,6,OpenGLRenderer::SHAPE_LINES);
}


void TheGame::DoPostEffect()
{	
	OpenGLRenderer::BindFrameBuffer(m_fbo.ID);
	OpenGLRenderer::SetOrtho(0.0f , (float)g_theApplication->m_windowSize.x , 0.0f , (float)g_theApplication->m_windowSize.y , 0 , 1 );	
	OpenGLRenderer::FrameBufferColorTexture( m_lightTexture );
	OpenGLRenderer::BindTexture( m_fbo.colorTextureID );
	m_fetchBrightMaterial->Activate();
	OpenGLRenderer::DrawVertexWithVertexArray2D(m_quad,4,OpenGLRenderer::SHAPE_QUADS,(float)g_theApplication->m_windowSize.x,(float)g_theApplication->m_windowSize.y);

	GaussianBlur(m_lightTexture,m_renderTexture);

	OpenGLRenderer::BindFrameBuffer(0);
	OpenGLRenderer::ClearScreen( 0.0f , 0.0f , 0.0f );
	OpenGLRenderer::SetOrtho(0.0f , (float)g_theApplication->m_windowSize.x , 0.0f , (float)g_theApplication->m_windowSize.y , 0 , 1 );
	OpenGLRenderer::BindTexture( m_fbo.colorTextureID , 0);
	OpenGLRenderer::BindTexture( m_lightTexture , 1);
	m_combineMaterial->Activate();
	OpenGLRenderer::DrawVertexWithVertexArray2D(m_quad,4,OpenGLRenderer::SHAPE_QUADS,(float)g_theApplication->m_windowSize.x,(float)g_theApplication->m_windowSize.y);

	OpenGLRenderer::BindFrameBuffer(m_fbo.ID);
	OpenGLRenderer::ClearScreen( 0.0f , 0.0f , 0.0f );
	OpenGLRenderer::BindFrameBuffer(0);
}


void TheGame::SetUpPerspectiveProjection()
{
	float aspect = (16.f / 9.f); // VIRTUAL_SCREEN_WIDTH / VIRTUAL_SCREEN_HEIGHT;
	float fovX = 70.f;
	float fovY = (fovX / aspect);
	float zNear = 0.1f;
	float zFar = 1200.f;

	OpenGLRenderer::LoadIdentity();
	OpenGLRenderer::Perspective( fovY, aspect, zNear, zFar ); // Note: Absent in OpenGL ES 2.0*/
}


void TheGame::ApplyCameraTransform( const Camera3D& camera )
{
	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);

	// Put us in our preferred coordinate system: +X is east (forward), +Y is north, +Z is up
	OpenGLRenderer::Rotatef( -90.f, 1.f, 0.f, 0.f ); // lean "forward" 90 degrees, to put +Z up (was +Y)
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	OpenGLRenderer::Rotatef( 90.f, 0.f, 0.f, 1.f ); // spin "left" 90 degrees, to put +X forward (was +Y)
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	// Orient the view per the camera's orientation
	OpenGLRenderer::Rotatef( -camera.m_orientation.rollDegreesAboutX,	1.f, 0.f, 0.f );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	OpenGLRenderer::Rotatef( -camera.m_orientation.pitchDegreesAboutY,	0.f, 1.f, 0.f );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	OpenGLRenderer::Rotatef( -camera.m_orientation.yawDegreesAboutZ,	0.f, 0.f, 1.f );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
	// Position the view per the camera's position
	OpenGLRenderer::Translatef( -camera.m_position.x, -camera.m_position.y, -camera.m_position.z );
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix);
}


Vec2f TheGame::GetMouseMovementSinceLastChecked()
{
	POINT cursorPos;
	GetCursorPos( &cursorPos );

	if(m_input->isFocus)
		SetCursorPos( (int)m_centerCursorPos.x, (int)m_centerCursorPos.y );
	else
		return Vec2f(0.0f,0.0f);

	SetCursorPos( (int)m_centerCursorPos.x, (int)m_centerCursorPos.y );
	Vec2i mouseDeltaInts( (int)(cursorPos.x - m_centerCursorPos.x), (int)(cursorPos.y - m_centerCursorPos.y) );
	Vec2f mouseDeltas( (float) mouseDeltaInts.x, (float) mouseDeltaInts.y );
	return mouseDeltas;
}


void TheGame::UpdateCameraFromMouseAndKeyboard( Camera3D& camera, float deltaSeconds )
{
	// Update camera orientation (yaw and pitch only) from mouse x,y movement
	const float degreesPerMouseDelta = 0.04f;
	float moveSpeed = 8.0f;
	Vec2f mouseDeltas = GetMouseMovementSinceLastChecked();
	camera.m_orientation.yawDegreesAboutZ		-= (degreesPerMouseDelta * mouseDeltas.x);
	camera.m_orientation.pitchDegreesAboutY		+= (degreesPerMouseDelta * mouseDeltas.y);

	// Update camera position based on which (if any) movement keys are down
	float cameraYawRadians = degree2radians(camera.m_orientation.yawDegreesAboutZ);
	float cameraPitchRadians = degree2radians(camera.m_orientation.pitchDegreesAboutY);
	Vec3f cameraForwardVector = Vec3f( cos(cameraYawRadians) * cos(cameraPitchRadians) , sin(cameraYawRadians) * cos(cameraPitchRadians) , -sin(cameraPitchRadians));
	camera.m_forwardVector = cameraForwardVector;

	if(m_input->m_keyState[Inputs::INPUT_SHIFT].isHold)
		moveSpeed = 32.0f;

	Vec3f movementVector( 0.f, 0.f, 0.f );
	if( m_input->m_keyState[ 'W' ].isHold )
		movementVector += camera.m_forwardVector * moveSpeed;
	if( m_input->m_keyState[ 'S' ].isHold )
		movementVector -= camera.m_forwardVector * moveSpeed;
	if( m_input->m_keyState[ 'A' ].isHold )
		movementVector -= camera.m_forwardVector.crossProductWith(Vec3f(0,0,1)) * moveSpeed;
	if( m_input->m_keyState[ 'D' ].isHold )
		movementVector += camera.m_forwardVector.crossProductWith(Vec3f(0,0,1)) * moveSpeed;

	if( m_input->m_keyState[Inputs::INPUT_SPACE].isHold )
		movementVector = Vec3f(0.f, 0.f, 0.25f) * moveSpeed;

	if( m_input->m_keyState[ 'Z' ].isHold )
		movementVector = Vec3f(0.f, 0.f, -0.25f) * moveSpeed;
	//...and so on, for A,S,D moving left, back, right - and for E,C moving up, down.

	camera.m_position += (movementVector * deltaSeconds);
}


void TheGame::ToggleCursorDisplay(bool show)
{
	if(!show)
	{
		while(ShowCursor(false)>=0);
		SetCursorPos( (int)m_centerCursorPos.x, (int)m_centerCursorPos.y );
	}
	else
		ShowCursor(true);
}


GameState TheGame::GetGameState()
{
	return m_gameState;
}


void TheGame::SetGameState(GameState state)
{
	m_gameState = state;
}


void TheGame::Initialize()
{
	m_centerCursorPos = Vec2f(400,300);
	SetCursorPos(400,300);
	ShowCursor(false);
	m_isQuitting = false;
	m_timeOfLastUpdate = GetCurrentTimeSeconds();

	m_material				= new Material( ShaderProgram ( OpenGLRenderer::CreateProgram() , new OpenGLShader( "./Data/Shaders/doNothing_fragment.glsl"		, OpenGLRenderer::FRAGMENT_SHADER) , new OpenGLShader( "./Data/Shaders/vertex.glsl"				, OpenGLRenderer::VERTEX_SHADER ) ) );
	m_lightMaterial			= new Material( ShaderProgram ( OpenGLRenderer::CreateProgram() , new OpenGLShader( "./Data/Shaders/fragment_lighting.glsl"			, OpenGLRenderer::FRAGMENT_SHADER) , new OpenGLShader( "./Data/Shaders/vertex.glsl"				, OpenGLRenderer::VERTEX_SHADER ) ) );
	m_fboMaterial			= new Material( ShaderProgram ( OpenGLRenderer::CreateProgram() , new OpenGLShader( "./Data/Shaders/fragment_frameBuffer.glsl"		, OpenGLRenderer::FRAGMENT_SHADER) , new OpenGLShader( "./Data/Shaders/doNothing_vertex.glsl"	, OpenGLRenderer::VERTEX_SHADER ) ) );
	m_blurMaterial			= new Material( ShaderProgram ( OpenGLRenderer::CreateProgram() , new OpenGLShader( "./Data/Shaders/fragment_blur.glsl"				, OpenGLRenderer::FRAGMENT_SHADER) , new OpenGLShader( "./Data/Shaders/doNothing_vertex.glsl"	, OpenGLRenderer::VERTEX_SHADER ) ) );
	m_fetchBrightMaterial	= new Material( ShaderProgram ( OpenGLRenderer::CreateProgram() , new OpenGLShader( "./Data/Shaders/fragment_fetchBrightPart.glsl"	, OpenGLRenderer::FRAGMENT_SHADER) , new OpenGLShader( "./Data/Shaders/doNothing_vertex.glsl"	, OpenGLRenderer::VERTEX_SHADER ) ) );
	m_combineMaterial		= new Material( ShaderProgram ( OpenGLRenderer::CreateProgram() , new OpenGLShader( "./Data/Shaders/fragment_combine.glsl"			, OpenGLRenderer::FRAGMENT_SHADER) , new OpenGLShader( "./Data/Shaders/doNothing_vertex.glsl"	, OpenGLRenderer::VERTEX_SHADER ) ) );
	m_skyBoxMaterial		= new Material( ShaderProgram ( OpenGLRenderer::CreateProgram() , new OpenGLShader( "./Data/Shaders/fragment_skyBox.glsl"			, OpenGLRenderer::FRAGMENT_SHADER) , new OpenGLShader( "./Data/Shaders/doNothing_vertex.glsl"	, OpenGLRenderer::VERTEX_SHADER ) ) );

	OpenGLRenderer::InitializeFrameBuffer(m_fbo);
	m_input = g_theApplication->m_input;

	m_terrain = new Terrain( Vec2i( 0 , 0 ) );
	m_numOfGrid = Vec2i( 8 , 8 );
	//m_road.GenerateGridRoad( *m_terrain , m_numOfGrid );
	
	m_textureGenerator = new TextureGenerator( Vec2i(64,64) , Vec2i(8,8) , 10 );
	m_enableTexture = false;
	m_drawDebug = false;
	m_timerForTexture = 0;
	m_textureIndex = 0;

	m_sampleOffset = Vec2f( 1.0f / (float)g_theApplication->m_windowSize.x, 1.0f / (float)g_theApplication->m_windowSize.y );
	m_renderTexture = OpenGLRenderer::CreateTexture(g_theApplication->m_windowSize.x,g_theApplication->m_windowSize.y);
	m_lightTexture = OpenGLRenderer::CreateTexture(g_theApplication->m_windowSize.x,g_theApplication->m_windowSize.y);

	m_quad[0].position = Vec3f( 0.0f									, 0.0f										, 0.0f );
	m_quad[1].position = Vec3f( (float)g_theApplication->m_windowSize.x	, 0.0f										, 0.0f );
	m_quad[2].position = Vec3f( (float)g_theApplication->m_windowSize.x	, (float)g_theApplication->m_windowSize.y	, 0.0f );
	m_quad[3].position = Vec3f( 0.0f									, (float)g_theApplication->m_windowSize.y	, 0.0f );

	m_quad[0].texCoords = Vec2f( 0.0f	, 0.0f );
	m_quad[1].texCoords = Vec2f( 1.0f	, 0.0f );
	m_quad[2].texCoords = Vec2f( 1.0f	, 1.0f );
	m_quad[3].texCoords = Vec2f( 0.0f	, 1.0f );

	m_skybox = new SkyBox( Vec2i(1024,1024) );
}


void TheGame::GaussianBlur(int textureID1 , int textureID2)
{
	Vec2f verticalBlur = Vec2f( 0.0f , m_sampleOffset.y );
	Vec2f horizontalBlur = Vec2f( m_sampleOffset.x , 0.0f );
	bool PingPong = true;
	for(int index=0; index < 20; ++index)
	{
		OpenGLRenderer::BindFrameBuffer(m_fbo.ID);
		OpenGLRenderer::SetOrtho(0.0f , (float)g_theApplication->m_windowSize.x , 0.0f , (float)g_theApplication->m_windowSize.y , 0 , 1 );
		OpenGLRenderer::FrameBufferColorTexture(PingPong ? textureID2 : textureID1);
		OpenGLRenderer::BindTexture(PingPong ? textureID1 : textureID2);
		m_blurMaterial->Activate();
		m_blurMaterial->SendUniform("u_worldToClipMatrix",OpenGLRenderer::WorldToClipMatrixStack.back());
		m_blurMaterial->SendUniform("u_colorMap" , 0);
		m_blurMaterial->SendUniform("u_sampleOffset" , PingPong ? verticalBlur : horizontalBlur);
		m_blurMaterial->SendUniform("u_attenuation" , 1.0f);
		OpenGLRenderer::DrawVertexWithVertexArray2D(m_quad,4,OpenGLRenderer::SHAPE_QUADS,(float)g_theApplication->m_windowSize.x,(float)g_theApplication->m_windowSize.y);
		PingPong = !PingPong;
	}
}


void TheGame::GenerateBuildings()
{
// 	SpotAttributes sa;
// 	m_terrain->GetAttributesAt( Vec2i(0,0) , &sa );
// 	BoxBuilding* building = new BoxBuilding( 2 , sa );
// 	building->m_textureIDs.push_back(m_textureGenerator->m_diffuseTextureIDs[0]);
// 	m_buildings.push_back( building );

// 	SpotAttributes sa;
// 	Vec2i roadLocation = m_terrain->m_terrainSize / m_numOfGrid;
// 	Vec2i halfTerrainSize = Vec2i( m_terrain->m_terrainSize.x >> 1 , m_terrain->m_terrainSize.y >> 1 );
// 	for(int x = 0; x < m_terrain->m_terrainSize.x; ++x)
// 	{
// 		for(int y = 0; y < m_terrain->m_terrainSize.y; ++y)
// 		{
// 			m_terrain->GetAttributesAt( Vec2i(x,y) - halfTerrainSize , &sa );
// 			
// 			if( x % roadLocation.x == 0 || (x + 1) % roadLocation.x == 0 || 
// 				y % roadLocation.y == 0 || (y + 1) % roadLocation.y == 0 )
// 			{
// 				m_buildings.push_back(new Building(sa));
// 			}
// 			else
// 			{
// // 				float dice = getRandomPercent();
// // 				if(dice > 0.8f)
// // 					m_buildings.push_back(new CylinderBuilding(0,sa)); 
// // 				else
// 					BoxBuilding* building = new BoxBuilding( random( 1 , 2 ) , sa );
// 
// 				building->m_textureIDs.push_back( m_textureGenerator->m_diffuseTextureIDs[0] );
// 				m_buildings.push_back( building ); 
// 			}
// 		}
// 	}

	Vec2i blockSize(10,10);
	Vec2i limitation = m_terrain->m_terrainSize / blockSize;

	for(int x = 0; x < limitation.x; ++x)
	{
		for(int y = 0; y < limitation.y; ++y)
		{
			m_blocks.push_back( Block( Vec2i( x , y ) * blockSize - m_terrain->m_halfSize , blockSize - 1 , m_terrain , m_textureGenerator ) );
		}
	}

	//m_blocks.push_back( Block( Vec2i( 0 , 0 ) , Vec2i( 10 , 10 ) , m_terrain , m_textureGenerator ) );
}


};
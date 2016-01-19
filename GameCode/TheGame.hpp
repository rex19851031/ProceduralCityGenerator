#pragma once

#ifndef THEGAME_HPP
#define THEGAME_HPP

#include <stdio.h>
#include <Windows.h>
#include <Xinput.h>					// include the Xinput API
#pragma comment( lib, "xinput" )	// Link in the xinput.lib static library

#include "Engine\Core\Camera3D.hpp"
#include "Engine\Renderer\Material.hpp"
#include "Engine\Math\GeneralStruct.hpp"
#include "Engine\Input\InputSystem.hpp"
#include "Engine\Parsing\TinyXML\TinyXMLHelper.hpp"

#include "GameCode\Terrain\Terrain.hpp"
#include "GameCode\Road\Road.hpp"
#include "GameCode\Texture\TextureGenerator.hpp"
#include "GameCode\Building\Building.hpp"
#include "GameCode\Block\Block.hpp"
#include "SkyBox\SkyBox.hpp"


namespace Henry
{

enum GameState{ STARTING_UP = 0 , QUITTING };


class TheGame
{
public:
	TheGame(void);
	~TheGame(void);
	void Input();
	void Update( float deltaSeconds );
	void Render();
	GameState GetGameState();
	void SetGameState(GameState state);
	void GenerateBuildings();

public:
	bool m_isQuitting;

private:
	void Initialize();
	void LoadDataFiles();
	void SetUpPerspectiveProjection();
	void ApplyCameraTransform( const Camera3D& camera );
	void DoPostEffect();
	void RenderAxes();
	void UpdateCameraFromMouseAndKeyboard( Camera3D& camera, float deltaSeconds );
	void PollKeyStates();
	void ToggleCursorDisplay(bool show);
	Vec2f GetMouseMovementSinceLastChecked();
	void GaussianBlur(int textureID1,int textureID2);

private:
	Material* m_material;
	Material* m_lightMaterial;
	Material* m_fboMaterial;
	Material* m_blurMaterial;
	Material* m_fetchBrightMaterial;
	Material* m_combineMaterial;
	Material* m_skyBoxMaterial;

	Camera3D m_camera;
	FBO m_fbo;
	Vec2f m_centerCursorPos;
	double m_timeOfLastUpdate;
	GameState m_gameState;
	InputSystem* m_input;
	std::vector<Building*> m_buildings;
	//Road m_road;
	std::vector<Block> m_blocks;
	Terrain* m_terrain;
	bool m_enableTexture;
	bool m_drawDebug;
	bool m_changeTexture;
	double m_timerForTexture;
	int m_textureIndex;
	int m_renderTexture;
	int m_lightTexture;
	Vec2f m_sampleOffset;
	Vertex_PCT m_quad[4];
	SkyBox* m_skybox;

	Vec2i m_numOfGrid;
	Matrix4 m_objectToWorldMatrix;
	TextureGenerator* m_textureGenerator;
};

};

#endif
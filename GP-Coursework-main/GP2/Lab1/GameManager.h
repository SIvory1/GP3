#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_mixer.h>
#include "DisplayGame.h" 
#include "ShaderManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "transform.h"
#include "AudioManager.h"
#include "GameObject.h"
#include "TextureManager.h"
#include "CubemapManager.h"
#include "FrameBufferObject.h"


enum class GameState{PLAY, EXIT};

class GameManager
{
public:
	GameManager();
	~GameManager();

	void run();

private:

	void SystemsStart();
	void ProcessInputs();
	void GameActive();
	void Spinning();
	void Ground();
	void Ground1();
	void UpdateDeltaTime();
	void DrawSkyBox();
	void DrawReflection();
	void DrawRefraction();
	void DrawGeom();
	void DrawEMap();
	void DrawGame();
	void DrawMix();
	void DrawWater();
	void DrawPlayer();
	void ProcessKeyboardInputs(const Uint8* inputs);
	void ShootMissile();
	void InitMissile();

	void print(float value)
	{
		cout << value << endl;
	}

	void Tree();
	bool IsColliding(MeshManager& mesh, MeshManager& mesh1);

	DisplayGame m_gameDisplay;
	GameState m_gameState;
	MeshManager m_cube;
	MeshManager m_apple;
	MeshManager m_tree;
	MeshManager missleMesh;
	MeshManager m_player;
	MainCamera m_mainCamera;
	ShaderManager m_shader;
	ShaderManager m_cubemapShader;
	ShaderManager m_reflectionShader;
	ShaderManager m_refractionShader;
	ShaderManager m_geoShader;
	ShaderManager m_mixShader;
	ShaderManager m_emapShader;
	ShaderManager m_waterShader;
	ShaderManager m_FBOShader;

	AudioManager m_gameAudio;

    TextureManager m_tarmacTex;
	TextureManager m_treeTex;
	TextureManager m_appleTex;
	TextureManager m_noise;
	TextureManager m_noise1;
	TextureManager m_texture1;
	TextureManager m_water;

	CubemapManager m_cubeMap;
    FrameBufferObject m_FBO;

	GameObject gameObject;


	// for object transform
	float m_collsionCounter;

	// mouse movement
	float m_x, m_preX, m_y, m_preY;

	// object manipluation
	float m_scale;
	float m_offset;

	Uint64 m_currentFrame = SDL_GetPerformanceCounter();
	Uint64 m_lastFrame = 0;
	float m_time = 0;

	bool cameraLock;

	glm::vec3 desiredVelocity;
	glm::vec3 steeringVelocity;
};


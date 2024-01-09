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

class GameObject
{
public:
	GameObject();
	~GameObject();


	void DrawAll(Transform transform, MainCamera mainCamera, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
	void InitComponents();
	//void SetTransform(Transform transform, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);


private:
	Transform tObject;

	ShaderManager shader1;
	TextureManager tarmacTex;
	MeshManager mesh1;

	enum
	{
		TARMAC,
		NUM_OF_TEXTURES
	};

	TextureManager* textureHolder = new TextureManager[20];

	bool active;
	float counter = 0.0f;

	float deltaTime;
}; 
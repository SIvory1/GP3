#pragma once
#include <string>
#include <vector>
#include <GL\glew.h>
#include "ShaderManager.h"
class CubemapManager
{
public:
	CubemapManager();
	~CubemapManager();

	void CubeMapLoader();
	void CubemapVAO();
	void DrawCubemap();
	void InitalizeCubeMap();
	void CubeVertexArrayObject();
	void DrawCube();

private:

	unsigned int m_cubemapTextureID;
	unsigned int m_cubeVAO, m_cubeVBO;
	unsigned int m_skyboxVAO, m_skyboxVBO;
};
#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include <string>
#include "ShaderManager.h"

class FrameBufferObject
{
public:

    FrameBufferObject();
    ~FrameBufferObject();

    void GenerateFBO(float w, float h);
    void BindFBO();
    void UnbindFBO();
    void GenerateQuad();
    void RenderFBO(const Transform& transform, const MainCamera& cam, float counter);

    GLuint FBO, RBO, CBO, quadVAO, quadVBO;
    ShaderManager FBOShader;
};
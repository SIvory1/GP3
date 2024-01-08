#include "GameManager.h"
#include "MainCamera.h"
#include <iostream>
#include <string>

Transform transform;

bool shoot;

GameManager::GameManager()
{

	m_tarmacTex = new TextureManager("..\\res\\TarmacDark_D.jpg");
	m_noise = new TextureManager("..\\res\\waternoise.png");
	m_texture1 = new TextureManager("..\\res\\sky.jpg");
	m_water = new TextureManager("..\\res\\water1.jpg");

	// init variables
	m_offset = 0.0;
	m_scale = 1;

	m_gameState = GameState::PLAY;

	m_collsionCounter = 1.0f;

	m_x = 0;
	m_y = 0;
	m_preX = 0;
	m_preY = 0;

	cameraLock = false;
	shoot = true;
}

GameManager::~GameManager()
{
}

// function called when game starts
void GameManager::run()
{
	SystemsStart();
	GameActive();
}

// initalizes shader, textures, meshes, sounds 
void GameManager::SystemsStart()
{
	m_gameDisplay.InitalizeDisplay();

	m_geoShader.initGeo();
    // audio 
	m_gameAudio.AddAudio("..\\res\\background.wav");
	m_gameAudio.AddSound("..\\res\\shoot.wav");
//	gameAudio.PlayAudio();

	// loads in models
	m_cube.ModelLoader("..\\res\\cube.obj");
	missleMesh.ModelLoader("..\\res\\cube.obj");
	m_player.ModelLoader("..\\res\\cube.obj");


    //tree.ModelLoader("..\\res\\WoodenLog_obj.obj");
	//apple.ModelLoader("..\\res\\Apple_obj.obj");

	m_mainCamera.InitializeCamera(glm::vec3(0, 0, -0), glm::radians(m_mainCamera.fov), (float)m_gameDisplay.getX()/ m_gameDisplay.getY(), 0.01f, 1000.0f);

	// initalizes shaders
	m_shader.InitalizeShader("..\\res\\shader.vert", "..\\res\\shader.frag");
	m_cubemapShader.InitalizeShader("..\\res\\cubemapShader.vert", "..\\res\\cubemapShader.frag");
	//reflectionShader.InitalizeShader("..\\res\\reflectionShader.vert", "..\\res\\reflectionShader.frag");
	//refractionShader.InitalizeShader("..\\res\\refractionShader.vert", "..\\res\\refractionShader.frag");
//	mixShader.InitalizeShader("..\\res\\mix.vert", "..\\res\\mix.frag");
	m_emapShader.InitalizeShader("..\\res\\environmentMap.vert", "..\\res\\environmentMap.frag");
	m_waterShader.InitalizeShader("..\\res\\water.vert", "..\\res\\water.frag");

	// initalizes cubemap/cube functions
	m_cubeMap.InitalizeCubeMap();
	m_cubeMap.CubeVertexArrayObject();

	m_FBO.GenerateFBO(m_gameDisplay.getX(), m_gameDisplay.getY());
	m_FBO.GenerateQuad();

}

// do mili sec aswell?
void GameManager::UpdateDeltaTime()
{
	m_lastFrame = m_currentFrame;
	m_currentFrame = SDL_GetPerformanceCounter();

	m_time = (float)((m_currentFrame - m_lastFrame) / (float)SDL_GetPerformanceFrequency());

}

void GameManager::GameActive()
{
	cout << "done" << endl;
	// if game isnt closed then constantly function calls fucntiosn that need to be updated regulary
	while (m_gameState != GameState::EXIT)
	{
		ProcessInputs();
		DrawGame();
	//	IsColliding(m_cube, missleMesh);

		UpdateDeltaTime();

		// locks mouse in window and hide it 
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
}
void GameManager::ProcessInputs()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) //get and process events
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEWHEEL:
		{
			// when player used scroll wheel
			if (event.wheel.y > 0 && m_mainCamera.fov != 0)
			{			
				// increases fov
				m_mainCamera.Zoom(-1);
				// updates fov 
				m_mainCamera.ChangeFOV(glm::radians(m_mainCamera.fov), (float)m_gameDisplay.getX() / m_gameDisplay.getY(), 0.01f, 1000.0f);
			}
		    if (event.wheel.y < 0)
			{
				// decreases fov
				m_mainCamera.Zoom(1);
				// updates fov 
				m_mainCamera.ChangeFOV(glm::radians(m_mainCamera.fov), (float)m_gameDisplay.getX() / m_gameDisplay.getY(), 0.01f, 1000.0f);
			}		
		}
		break;
		case SDL_MOUSEBUTTONDOWN:
		{
			// when player inputs mouse buttons objects in scene moves 
			if (event.button.button == SDL_BUTTON_LEFT)
			{

				shoot = true;
				//m_offset += 1;
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				m_offset -= 20;
			}
		}
		break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				// can adjust object size
			case SDLK_e:
			//	m_scale += .1;
				cameraLock = true;
				break;
			case SDLK_r:
				//	m_scale += .1;
				cameraLock = false;
				break;
			case SDLK_q:
				if (m_scale > 0)
				{
					m_scale -= .1;
				}
				break;
				// reset camera pos 
			case SDLK_v:
				m_mainCamera.InitializeCamera(glm::vec3(0, 0, -5), glm::radians(m_mainCamera.fov), (float)m_gameDisplay.getX() / m_gameDisplay.getY(), 0.01f, 1000.0f);
				break;
			}
			break;
		case SDL_MOUSEMOTION:
		{
			if (!cameraLock)
			{
			    m_x = event.motion.x;
				m_y = event.motion.y;
				// checks to see if the x value has increased or decreases from last pos
				if (m_x > m_preX || m_x >= (m_gameDisplay.getX()) - 1)
				{
					m_mainCamera.RotateX(-0.01);
				}
				else if (m_x < m_preX || m_x == 0)
				{
					m_mainCamera.RotateX(0.01);
				}
				// checks to see if the y value has increased or decreases from last pos
				if (m_y > m_preY || m_preY >= (m_gameDisplay.getY()) - 1)
				{
					m_mainCamera.RotateY(0.01);
				}
				if (m_y < m_preY || m_y == 0)
				{
					m_mainCamera.RotateY(-0.01);
				}
				// gives current pos to variables
				m_preX = m_x;
				m_preY = m_y;	// stores the x and y position of screen space			
			}
		}
		break;
		}
    }
	const Uint8* keyInputs = SDL_GetKeyboardState(NULL);
	ProcessKeyboardInputs(keyInputs);
}
	
void GameManager::ProcessKeyboardInputs(const Uint8* inputs)
{
	float distance = 40.0f * m_time;

	if (inputs[SDL_SCANCODE_W]) { // Move forward
		m_mainCamera.MoveForward(distance);
	}
	if (inputs[SDL_SCANCODE_S]) { // Move backward
		m_mainCamera.MoveForward(-distance);
	}
	if (inputs[SDL_SCANCODE_A]) { // Move left
		m_mainCamera.MoveHoriz(-distance);
	}
	if (inputs[SDL_SCANCODE_D]) { // Move right
		m_mainCamera.MoveHoriz(distance);
	}
	if (inputs[SDL_SCANCODE_SPACE]) {
		m_mainCamera.MoveVertical(distance);		
	}
	if (inputs[SDL_SCANCODE_LSHIFT]) {
		m_mainCamera.MoveVertical(-distance);
	}
};


// reads in 2 meshes and checks if they are colliding
bool GameManager::IsColliding(MeshManager& mesh, MeshManager& mesh1)
{

	float distance = 
		((mesh1.getSpherePos().x - mesh.getSpherePos().x) * (mesh1.getSpherePos().x - mesh.getSpherePos().x) 
		+ (mesh1.getSpherePos().y - mesh.getSpherePos().y) * (mesh1.getSpherePos().y - mesh.getSpherePos().y) 
		+ (mesh1.getSpherePos().z - mesh.getSpherePos().z) * (mesh1.getSpherePos().z - mesh.getSpherePos().z));
	if (distance * distance < (mesh.getSphereRad() + mesh1.getSphereRad()))
	{	
		
	  // m_collsionCounter *= 0;
		m_gameAudio.PlaySound(0);
		return true;
	}
		return false;
}

/*void GameManager::Spinning()
{
	// sets pos, rotation, scale
	transform.SetPos(glm::vec3(-sinf(collsionCounter), 0, -sinf(collsionCounter) * 5));
	transform.SetRot(glm::vec3(0.0, 0.0, 0));
	transform.SetScale(glm::vec3(scale, scale, scale));

	// binds texture
	appleTex.BindTexture(0);

	// bind shader and updates 
	shader.Bind();
	shader.UpdateShader(transform, mainCamera);

	// updates col
	apple.UpdateColData(*transform.GetPos(), scale);
	
	// draws object 
	apple.Draw();
}

void GameManager::Spinning()
{
	// sets pos, rotation, scale
	transform.SetRot(glm::vec3(0.0, 0.0, 0));
	transform.SetScale(glm::vec3(m_scale, m_scale, m_scale));

	// binds texture
	m_tarmacTex->BindTexture(0);

	// bind shader and updates
	m_shader.Bind();
	m_shader.UpdateShader(transform, m_mainCamera);

	// updates col
	m_apple.UpdateColData(*transform.GetPos(), m_scale);

	// draws object
	m_apple.Draw();
}
void GameManager::Tree()
{
	transform.SetPos(glm::vec3(5 + m_offset, 0.0, 0));
	transform.SetRot(glm::vec3(0.0, m_time * 2, 0));
	transform.SetScale(glm::vec3(m_scale, m_scale, m_scale));

	m_treeTex.BindTexture(0);

	m_geoShader.Bind();

	m_geoShader.UpdateShader(transform, m_mainCamera);

	m_geoShader.setFloat("time", m_tree / 2);

	m_tree.UpdateColData(*transform.GetPos(), m_scale);

	m_tree.Draw();
}


void GameManager::DrawRefraction()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	transform.SetPos(glm::vec3(2, 0.0, 2));
	transform.SetRot(glm::vec3(0.0, 0, 0));
	transform.SetScale(glm::vec3(1, 1, 1));

	refractionShader.Bind();

	refractionShader.setMat4("view", mainCamera.GetView());
	refractionShader.setMat4("projection", mainCamera.GetProjection());
	refractionShader.setMat4("model", transform.GetModel());
	refractionShader.setVec3("cameraPos", mainCamera.getPosition());

	refractionShader.UpdateShader(transform, mainCamera);

	cubeMap.DrawCube();
}

void GameManager::DrawReflection()
{
	transform.SetPos(glm::vec3(4, 0.0, 2));
	transform.SetRot(glm::vec3(0.0, counter , 0));
	transform.SetScale(glm::vec3(1, 1, 1));

	reflectionShader.Bind();

	reflectionShader.setMat4("view", mainCamera.GetView());
	reflectionShader.setMat4("projection", mainCamera.GetProjection());
	reflectionShader.setMat4("model", transform.GetModel());
	reflectionShader.setVec3("cameraPos", mainCamera.getPosition());

	reflectionShader.UpdateShader(transform, mainCamera);
	cubeMap.DrawCube();
}

void GameManager::DrawMix()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	transform.SetPos(glm::vec3(6, 0.0, 2));
	transform.SetRot(glm::vec3(0.0, counter, 0));
	transform.SetScale(glm::vec3(1, 1, 1));

	mixShader.Bind();

	mixShader.setMat4("view", mainCamera.GetView());
	mixShader.setMat4("projection", mainCamera.GetProjection());
	mixShader.setMat4("model", transform.GetModel());
	mixShader.setVec3("cameraPos", mainCamera.getPosition());
	mixShader.setVec3("waterColor", glm::vec3(0.5,0.5,0.5));

	mixShader.UpdateShader(transform, mainCamera);

	cubeMap.DrawCube();

}*/



void GameManager::Ground()
{
	transform.SetPos(glm::vec3(0, -5, 0));
	transform.SetScale(glm::vec3(40, 1, 40));

	m_tarmacTex->BindTexture(0);

	m_shader.Bind();
	m_shader.UpdateShader(transform, m_mainCamera);

	m_cube.UpdateColData(*transform.GetPos(), 1);

	m_cube.Draw();
}
void GameManager::Ground1()
{
	transform.SetPos(glm::vec3(-20, 30, m_offset));
	transform.SetScale(glm::vec3(1, 1, 1));

	m_tarmacTex->BindTexture(0);

	m_shader.Bind();
	m_shader.UpdateShader(transform, m_mainCamera);

	m_cube.UpdateColData(*transform.GetPos(), 1);

	m_cube.Draw();
}

void GameManager::DrawWater()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	transform.SetPos(glm::vec3(1, 0, 0));
	transform.SetRot(glm::vec3(0.0, 0, 0));
	transform.SetScale(glm::vec3(m_scale, m_scale, m_scale));

	m_waterShader.Bind();
	m_waterShader.setMat4("model", transform.GetModel());
	m_waterShader.setVec3("cameraPos", m_mainCamera.getPosition());

	m_waterShader.setFloat("time", m_time / 20);

	GLuint noiseTex = glGetUniformLocation(m_waterShader.getID(), "noiseTex");
	GLuint waterTex = glGetUniformLocation(m_waterShader.getID(), "water");

	//set textures
	glActiveTexture(GL_TEXTURE1); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, m_noise->getID());
	glUniform1i(noiseTex, 1);

	glActiveTexture(GL_TEXTURE2); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, m_water->getID());
	glUniform1i(waterTex, 2);

	m_waterShader.UpdateShader(transform, m_mainCamera);

	m_cube.Draw();
}
void GameManager::DrawSkyBox()
{
	transform.SetPos(glm::vec3(0.0, 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(100, 100, 100));

	m_cubemapShader.Bind();
	m_cubemapShader.UpdateCubemap(m_mainCamera);
	m_cubeMap.DrawCubemap();
}
void GameManager::DrawEMap()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	transform.SetPos(glm::vec3(-2, 0.0, 0));
	transform.SetRot(glm::vec3(0.0, 0, 0));
	transform.SetScale(glm::vec3(m_scale, m_scale, m_scale));

	m_emapShader.Bind();
	m_emapShader.setMat4("model", transform.GetModel());
	m_emapShader.setVec3("cameraPos", m_mainCamera.getPosition());

	GLuint blur = glGetUniformLocation(m_emapShader.getID(), "diffuse");

	//set textures
	glActiveTexture(GL_TEXTURE1); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, m_tarmacTex->getID());
	glUniform1i(blur, 1);

	m_emapShader.UpdateShader(transform, m_mainCamera);

	m_cube.Draw();
}





// store in process inputs, checks for missle, if it finds one you make it move, init the missle esle where 
void GameManager::ShootMissile()
{
	
		transform.SetPos(glm::vec3(steeringVelocity * m_collsionCounter));
		transform.SetRot(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(m_scale, m_scale, m_scale));

		m_tarmacTex->BindTexture(0);

		desiredVelocity = (glm::normalize(m_cube.getSpherePos() - missleMesh.getSpherePos()));
		steeringVelocity = desiredVelocity - glm::vec3(0.0f, 0.0f, 0.0f); // need to get curren tvecoltiy


		missleMesh.UpdateColData(*transform.GetPos(), 1);

		m_shader.Bind();
		m_shader.UpdateShader(transform, m_mainCamera);

		missleMesh.Draw();
	
}

// have an array of missles and then when a missle hits an object remvoe it from the array

void GameManager::InitMissile()
{
	transform.SetPos(glm::vec3(m_mainCamera.getPosition() - glm::vec3(0, 5, 0)));
	transform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(m_scale, m_scale, m_scale));

	m_tarmacTex->BindTexture(0);

	m_player.UpdateColData(*transform.GetPos(), m_scale);

	m_shader.Bind();
	m_shader.UpdateShader(transform, m_mainCamera);


	m_player.Draw();
}

void GameManager::DrawPlayer()
{
	transform.SetPos(glm::vec3(m_mainCamera.getPosition()));
	transform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(m_scale, m_scale, m_scale));

	m_tarmacTex->BindTexture(0);

	m_player.UpdateColData(*transform.GetPos(), m_scale);

	m_shader.Bind();
	m_shader.UpdateShader(transform, m_mainCamera);

	m_player.Draw();
}




void GameManager::DrawGame()
{
	m_gameDisplay.ClearDisplay(0.5, 0.5, 0.5, 1.0);

    m_FBO.BindFBO();
	DrawSkyBox();
	InitMissile();
	Ground();

	//m_mainCamera.LookAt(m_player.getSpherePos());

	m_FBO.UnbindFBO();
	
	// if ship is dmaaged shake screen
	m_FBO.RenderFBO(transform, m_mainCamera, m_collsionCounter);
	
	DrawSkyBox();
	DrawPlayer();
	Ground();
	//Ground1();

	m_collsionCounter = m_collsionCounter + 0.05f;

	m_gameDisplay.ChangeBuffer();
} 
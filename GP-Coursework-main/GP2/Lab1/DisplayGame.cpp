#include "DisplayGame.h"

// renamed for lab 1
DisplayGame::DisplayGame()
{
	// init variables
	m_glContext = nullptr;
	m_window = nullptr;
	m_screenX = 1024;
	m_screenY = 768;
}

DisplayGame::~DisplayGame()
{
	// when game ends deletes left over systems
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

// getters to return screen size
float DisplayGame::getX() { return m_screenX; } //getters
float DisplayGame::getY() { return m_screenY; }

// reuseable fucntion used to show which error has occured
void DisplayGame::ReturnError(std::string errorMessage)
{
	std::cout << errorMessage << std::endl;
	std::cout << "Please press any key to exit.";
	int in;
	std::cin >> in;
	SDL_Quit();

	

}

void DisplayGame::ChangeBuffer()
{
	// updates window, used for doubel buffering
	SDL_GL_SwapWindow(m_window);
}


void DisplayGame::ClearDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears colour and depth buffers 
}

void DisplayGame::InitalizeDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	// setting up double buffer
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);// set up z-buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// creates window 
	m_window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenX, m_screenY, SDL_WINDOW_OPENGL);

	// creates context for our window
	m_glContext = SDL_GL_CreateContext(m_window);

	//opengl extension wrangler libary 
	GLenum error = glewInit();

	// if window fails, ensures we know it was window for debugging
	if (m_window == nullptr)
	{
		ReturnError("window failed to create");
	}

	if (m_glContext == nullptr)
	{
		ReturnError("glContext failed to create");
	}

	if (error != GLEW_OK)
	{
		ReturnError("GLEW failed to initalize");
	}

	glEnable(GL_DEPTH_TEST); //enable z-buffering 
	glEnable(GL_CULL_FACE);  // faces arent drawn if the cam is not looking at it

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}
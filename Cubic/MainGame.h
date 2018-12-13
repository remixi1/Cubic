#pragma once

#include <SDL/SDL.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "GLSLProgram.h"
#include "GLTexture.h"
#include "Controls.h"



enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

	void initSystems();

	void initShaders();

	void gameLoop();

	void processInput();

	void drawGame();

	void calculateFPS();

private:

	SDL_Window * m_window;

	int m_screenWidth, m_screenHeight;

	GameState m_gameState;

	Mesh m_mesh;

	GLSLProgram m_colorProgram;
	GLTexture m_meshTexture;
	Controls m_controls;
	float m_fps;
	float m_maxFPS;
	float m_frameTime;

};
#include <iostream>
#include "MainGame.h"
#include "Errors.h"
#include "ImageLoader.h"



MainGame::MainGame()
{
	m_window = nullptr;
	m_screenWidth = 1200;
	m_screenHeight = 768;
	m_gameState = GameState::PLAY;
	m_maxFPS = 60.0f;
}


MainGame::~MainGame()
{
}
void MainGame::run() {


	initSystems();
	m_mesh.init();
	m_meshTexture = ImageLoader::loadPNG("Textures/Grass 4.png");

	gameLoop();
}

void MainGame::initSystems() {

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_OPENGL);



	SDL_GLContext GLContext = SDL_GL_CreateContext(m_window);

	GLenum initiliazeGlew = glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	initShaders();
}
void MainGame::initShaders() {

	m_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_colorProgram.linkShaders();
}

void MainGame::gameLoop() {


	while (m_gameState != GameState::EXIT) {


		float startTicks = SDL_GetTicks();
		m_controls.computeMatricesFromInputs();
		processInput();

		drawGame();
		calculateFPS();

		static int framecounter = 0;
		framecounter++;
		if (framecounter == 10) {
			std::cout << m_fps << std::endl;
			framecounter = 0;
		}
		float frameTicks = SDL_GetTicks() - startTicks;
		if (1000.0f / m_maxFPS > frameTicks) {
			SDL_Delay(1000.0f / m_maxFPS - frameTicks);
		}
	}
}

void MainGame::processInput() {

	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
		}
	}
}
void MainGame::drawGame() {

	glClearDepth(1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_colorProgram.use();


	GLuint textureLocation = m_colorProgram.getUniformLocation("myTextureSampler");


	GLuint location = m_colorProgram.getUniformLocation("MVP");


	// Compute the MVP matrix from keyboard and mouse input

	glm::mat4 ProjectionMatrix = m_controls.getProjectionMatrix();
	glm::mat4 ViewMatrix = m_controls.getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glUniformMatrix4fv(location, 1, GL_FALSE, &MVP[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_meshTexture.id);
	glUniform1i(textureLocation, 0);

	m_mesh.draw();

	m_colorProgram.unuse();

	SDL_GL_SwapWindow(m_window);
}

void MainGame::calculateFPS() {

	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();

	float currentTicks;
	currentTicks = SDL_GetTicks();

	m_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = m_frameTime;

	prevTicks = currentTicks;

	int count;

	currentFrame++;

	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	}
	else {
		count = NUM_SAMPLES;
	}
	float frameTimeAverage = 0;

	for (int i = 0; i < count; i++) {

		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	if (frameTimeAverage > 0) {

		m_fps = 1000.0f / frameTimeAverage;
	}
	else {
		m_fps = 60.0f;
	}

}
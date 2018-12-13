#include "Controls.h"
#include "MainGame.h"
#include <unordered_map>


SDL_Window* m_window;

using namespace glm;

Controls::Controls()
{

}


Controls::~Controls()
{
}

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 Controls::getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 Controls::getProjectionMatrix() {
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 0, 5);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 0.0070f; // 3 units / second
float mouseSpeed = 0.00005f;

void Controls::computeMatricesFromInputs() {

	// glfwGetTime is called only once, the first time this function is called
	static float lastTime = SDL_GetTicks();

	// Compute time difference between current and last frame
	float currentTime = SDL_GetTicks();
	float deltaTime = (currentTime - lastTime);


	// Get mouse position
	static int xpos = 0;
	static int ypos = 0;

	SDL_ShowCursor(1);

	SDL_GetMouseState(&xpos, &ypos);

	//Reset mouse position for next frame


	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1200 / 2 - xpos);
	verticalAngle += mouseSpeed * float(768 / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);


	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

						   // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;



	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		}
	}
	if (m_inputManager.iskeyPressed(SDLK_w)) {
		position += direction * deltaTime * speed;
	}
	if (m_inputManager.iskeyPressed(SDLK_s)) {
		position -= direction * deltaTime * speed;
	}
	if (m_inputManager.iskeyPressed(SDLK_d)) {
		position += right * deltaTime * speed;
	}
	if (m_inputManager.iskeyPressed(SDLK_a)) {
		position -= right * deltaTime * speed;
	}
	if (evnt.type == SDL_MOUSEMOTION) {

		evnt.motion.xrel;
		evnt.motion.yrel;

	}

}
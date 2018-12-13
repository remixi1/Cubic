#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "InputManager.h"


class Controls
{
public:
	Controls();
	~Controls();


	glm::vec2 oldMousePosition;

	glm::mat4 getViewMatrix();

	glm::mat4 getProjectionMatrix();



	void computeMatricesFromInputs();

	int getX();
	int getY();

private:

	InputManager m_inputManager;
};


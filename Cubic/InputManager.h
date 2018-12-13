#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

class InputManager
{
public:
	InputManager();
	~InputManager();

	void pressKey(unsigned int keyID);

	void releaseKey(unsigned int keyID);

	bool iskeyPressed(unsigned int keyID);



private:

	std::unordered_map<unsigned int, bool> m_keyMap;



};


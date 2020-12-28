#ifndef _PG_CORE_GAME_H_
#define _PG_CORE_GAME_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "OpenGL/Shader.h"

namespace PGame {
	class Game {
	public:
		Game(int windowWidth, int windowHeight, const std::string &windowTitle);
		~Game();

		bool Init();
		void Loop();

	public:
		GLFWwindow* window;
		std::string title;
		int width;
		int height;
	};
}

#endif

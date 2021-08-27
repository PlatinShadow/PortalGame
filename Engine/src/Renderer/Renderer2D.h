#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <array>

#include "OpenGL/Shader.h"
#include "OpenGL/Buffer.h"
#include "OpenGL/Texture.h"

namespace Engine {
	namespace Renderer {
#pragma pack(push)
		struct QuadVertex {
			glm::vec2 Pos;
			glm::vec2 Uv;
		};

		struct Quad {
			QuadVertex Vertecies[6];
		};

#pragma pack(pop)

		class R2D {
		public:
			R2D() = default;

			void Init(int width, int height);
			void SetResolution(int width, int height);

			void Finish();
			void SetClearColor(const glm::vec3& color);
			void Clear();

			void DrawRect(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& color);
		private:
			static const int MAX_QUADS = 2000;

			void UpdateProjectionMatrix();

			int m_RenderWidth;
			int m_RenderHeight;

			unsigned int m_RectanglesToDraw;

			std::shared_ptr<GL::Shader> m_Shader;
			std::shared_ptr<GL::Texture> m_Texture;

			GL::Buffer m_QuadBuffer;
			std::array<Quad, MAX_QUADS> m_Quads;
		};
	}
}
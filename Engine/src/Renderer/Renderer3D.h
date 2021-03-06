#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>

#include "Renderer/Data.h"
#include "OpenGL/Buffer.h"
#include "OpenGL/Shader.h"


namespace Engine {
	namespace Renderer {
		class Renderer3D {
		public:
			Renderer3D() = default;
			~Renderer3D();
			
			void Init(int width, int height, float fov);

			void SetViewMatrix(const glm::mat4 view);
			void UpdateProjectionMatrix();
			void SetFov(float fov);
			void SetResolution(int width, int height);
			void SetClippingDistance(float zNear, float zFar);

			void SubmitMesh(Mesh* mesh);
			void AddInstance(Mesh* mesh, const glm::mat4& transform);

			void Render();

		private:
			static const uint32_t MAX_BUFFER_SIZE;

			float m_Fov;
			float m_ClippingZNear;
			float m_ClippingZFar;
			int m_RenderWidth;
			int m_RenderHeight;

			std::shared_ptr<GL::Shader> m_Shader;

			glm::mat4 m_ProjectionMatrix;
			glm::mat4 m_ViewMatrix;

			GLuint m_RenderVAO;
			Mesh* m_LastMesh;
			GL::Buffer m_MeshBuffer;
			GL::Buffer m_CommandBuffer;
			GL::Buffer m_InstanceDataBuffer;
			std::vector<DrawArraysIndirectCommand> m_RenderCommands;
			std::unordered_map<Mesh*, MeshInfo> m_MeshInfo;
		};
	}
}
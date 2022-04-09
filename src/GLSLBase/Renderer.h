#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies/glew.h"
#include "Dependencies/wglew.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	void Test();
	void Lecture3();
	void Lecture3Particle();
	void CreateParticle(int count);

	GLuint CreatePngTexture(char* filePath);
	GLuint CreateBmpTexture(char* filePath);

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(const char* filename, std::string* target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(const char* file_vs, const char* file_fs);
	void CreateVertexBufferObjects();
	PUCHAR loadBMPRaw(const char* image, UINT& width, UINT& height);

	bool m_Initialized = false;

	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	//camera position
	glm::vec3 m_v3Camera_Position;
	//camera lookat position
	glm::vec3 m_v3Camera_Lookat;
	//camera up vector
	glm::vec3 m_v3Camera_Up;

	glm::mat4 m_m4OrthoProj;
	glm::mat4 m_m4PersProj;
	glm::mat4 m_m4Model;
	glm::mat4 m_m4View;
	glm::mat4 m_m4ProjView;

	GLuint m_VBORect = 0;
	GLuint m_VBORectLecture2 = 0;
	GLuint m_VBORectLecture3 = 0;
	GLuint m_VBOQuadLecture3Particle = 0; // single particle
	GLuint m_VBOManyParticle = 0;
	GLuint m_iManyParticleVertexCount = 0;

	GLuint m_SolidRectShader = 0;
	GLuint m_ShaderLecture3 = 0;
	GLuint m_ShaderLecture3Particle = 0;
};


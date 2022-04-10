#pragma once
#include "stdafx.h"
#include "Pipeline.hpp"
#include "VertexObjet.hpp"

#include "./Dependencies/glm/glm.hpp"
#include "./Dependencies/glm/gtc/matrix_transform.hpp"
#include "./Dependencies/glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();

	void Test();
	void Lecture3();
	void Lecture3Particle();

	GLuint CreatePngTexture(char* filePath);
	GLuint CreateBmpTexture(char* filePath);

private:
	void Initialize(int width, int height);

	GLuint CreatePipeline();
	void CreateVertexBufferObjects();
	void CreateParticle(const int count);

	PUCHAR loadBMPRaw(const char* image, UINT& w, UINT& h);

	bool Ready = false;

	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	glm::vec3 cameraPositions; //camera position
	glm::vec3 cameraLookAt; //camera lookat position
	glm::vec3 cameraLookUp; //camera up vector

	glm::mat4 m_m4OrthoProj;
	glm::mat4 m_m4PersProj;
	glm::mat4 m_m4Model;
	glm::mat4 m_m4View;
	glm::mat4 m_m4ProjView;

	Pipeline plSolidRect;
	Pipeline plLecture2;
	Pipeline plLecture3, plLecture3Particle;

	VertexObjet m_VBORect;
	VertexObjet m_VBORectLecture2;
	VertexObjet m_VBORectLecture3;
	VertexObjet m_QuadManyParticle;
	GLuint m_iManyParticleVertexCount = 0;
};


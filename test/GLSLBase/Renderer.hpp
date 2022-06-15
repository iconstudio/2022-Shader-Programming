#pragma once
#include "stdafx.h"
#include "Pipeline.hpp"
#include "VertexObjet.hpp"

#include "./Dependencies/glm/glm.hpp"
#include "./Dependencies/glm/gtc/matrix_transform.hpp"
#include "./Dependencies/glm/gtx/euler_angles.hpp"

enum class PRIMITIVE_METHODS : GLenum
{
	LINES = GL_LINES
	, LINE_STRIP = GL_LINE_STRIP				// 0x0003
	, TRIANGLES = GL_TRIANGLES					// 0x0004
	, TRIANGLE_STRIP = GL_TRIANGLE_STRIP		// 0x0005
	, TRIANGLE_FAN = GL_TRIANGLE_FAN			// 0x0006
	, QUADS = GL_QUADS							// 0x0007
	, QUAD_STRIP = GL_QUAD_STRIP				// 0x0008
	, POLYGON = GL_POLYGON						// 0x0009
	, POLYGON_STIPPLE = GL_POLYGON_STIPPLE_BIT	// 0x00000010
};

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();

	void Test();
	void Lecture3();
	void Lecture3Particle();
	void Lecture4();
	void Lecture4Raindrop();
	void Lecture4RaderCircle();
	void Lecture5LineSegment();
	void Lecture5Fullfil();
	void Lecture6Texture();
	void Lecture8MultiTextures();
	void Lecture9GridMesh();

	GLuint CreatePngTexture(const char* filePath);
	GLuint CreateBmpTexture(const char* filePath);

private:
	void Initialize(int width, int height);

	GLuint CreatePipeline();

	void CreateVertexBufferObjects();
	void CreateLecture3Particle(const int count);
	void CreateLecture4Objects();
	void CreateLecture5Line(int seg_count);
	void CreateLecture6Textures();
	void CreateLecture8Textures();
	void CreateLecture9Flag();

	void Render(PRIMITIVE_METHODS method, GLint first, GLsizei count);

	PUCHAR loadBMPRaw(const char* image, UINT& w, UINT& h);

	bool Ready = false;
	unsigned int windowWidth = 0;
	unsigned int windowHeight = 0;

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
	Pipeline plLecture4;
	Pipeline plLecture5Curve;
	Pipeline plLecture5Fullfil;
	Pipeline plLecture6Tex;
	Pipeline plLecture8MultipleTextures;
	Pipeline plLecture9Flag;

	VertexObjet vbSolidRect;
	VertexObjet vbLecture2;
	VertexObjet vbLecture3;
	VertexObjet vbQuadParticle;
	GLuint countParticleVertex = 0;

	VertexObjet vbQuadLecture4;
	VertexObjet vboPackaged0Pos;
	VertexObjet vboPackaged0Color;
	VertexObjet vboPackaged1;
	VertexObjet vboPackaged2;

	VertexObjet vboLecture5LineSegment;
	GLsizei vboLecture5SegCount;
	VertexObjet vboLecture5Fullfil;

	GLuint texLecture6Checker = 0;
	GLuint texLecture6RGB = 0;
	VertexObjet vboLecture6Positions;

	GLuint vboLecture8Tex0;
	GLuint vboLecture8Tex1;
	GLuint vboLecture8Tex2;
	GLuint vboLecture8Tex3;
	GLuint vboLecture8Tex4;
	GLuint vboLecture8Tex5;
	VertexObjet vboLecture8Texture;

	VertexObjet vboLecture9GridMesh;
	int gDummyVertexCount = 0;

	float ptLecture4[30]; // (x, y, z) * 10°³
	float ptLecture5[18]; // (x, y, z) * 6°³
};


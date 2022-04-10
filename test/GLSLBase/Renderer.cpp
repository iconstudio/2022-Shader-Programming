#include "stdafx.h"
#include "Renderer.hpp"
#include "Pipeline.hpp"
#include "Shader.hpp"
#include "VertexObjet.hpp"
#include "VertexBuffer.hpp"
#include "AttributeView.hpp"
#include "LoadPng.h"

float Time = 0.0f;

Renderer::Renderer(int width, int height)
	: plSolidRect(), plLecture2(), plLecture3(), plLecture3Particle()
	, m_VBORect()
	, m_VBORectLecture2()
	, m_VBORectLecture3(), m_QuadManyParticle()
{
	glClearDepth(1.f);

	Initialize(width, height);
}

Renderer::~Renderer() {}

void Renderer::Lecture3()
{
	auto& pipeline = plLecture3;
	pipeline.Use();
	pipeline.UseBuffer(m_VBORectLecture3, GL_ARRAY_BUFFER);

	// stride 값을 줘야 제대로 된 색을 읽는다.
	GLsizei stride = sizeof(float) * 7;

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	attrPosition.Stream(GL_FLOAT, 3, stride);

	auto attrColor = pipeline.GetAttribute("a_Color");
	attrColor.EnableVertexArray();
	attrColor.Stream(GL_FLOAT, 4, stride, (GLvoid*)(sizeof(float) * 3)); // 4번째 부터 읽기

	auto uniformTime = pipeline.GetUniform("u_Time");
	glUniform1f(uniformTime, Time);

	auto uniformColor = pipeline.GetUniform("u_Color");
	glUniform4f(uniformColor, 1, 1, 1, 1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	Time -= 0.01f;
	if (Time < 0.0f)
	{
		Time = 1.0f;
	}

	attrPosition.DisableVertexArray();
	attrColor.DisableVertexArray();
}

void Renderer::Lecture3Particle()
{
	auto& pipeline = plLecture3Particle;
	pipeline.Use();
	pipeline.UseBuffer(m_QuadManyParticle, GL_ARRAY_BUFFER);

	GLsizei stride = sizeof(float) * 6; // (x, y, z, sx, sy, sz)

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	attrPosition.Stream(GL_FLOAT, 3, stride);
	// (3 * float == stride) 라면 정점 갯수와 메모리 간격을 0으로 해도 된다. (어느것이든)

	auto attrVelocity = pipeline.GetAttribute("a_Velocity");
	attrVelocity.EnableVertexArray();
	attrVelocity.Stream(GL_FLOAT, 3, stride, (GLvoid*)(sizeof(float) * 3)); // 4번째 부터 읽기

	auto uniformTime = pipeline.GetUniform("u_Time");
	glUniform1f(uniformTime, Time);

	Time += 0.0001f;
	if (1.0f <= Time)
	{
		Time = 0.0f;
	}

	glDrawArrays(GL_TRIANGLES, 0, m_iManyParticleVertexCount);

	attrPosition.DisableVertexArray();
	attrVelocity.DisableVertexArray();
}

void Renderer::Test()
{
	auto& pipeline = plSolidRect;
	pipeline.Use();
	pipeline.UseBuffer(m_VBORect, GL_ARRAY_BUFFER);

	// 여기서 stride 값은 사실 0이어도 된다.
	GLsizei stride = sizeof(float) * 3;

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	attrPosition.Stream(GL_FLOAT, 3, stride);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	attrPosition.DisableVertexArray();
}

void Renderer::Initialize(int width, int height)
{
	// Set window size
	m_WindowSizeX = width;
	m_WindowSizeY = height;

	// 
	plSolidRect.AssignProgram(CreatePipeline());
	plLecture2.AssignProgram(CreatePipeline());
	plLecture3.AssignProgram(CreatePipeline());
	plLecture3Particle.AssignProgram(CreatePipeline());

	// Load shaders
	plSolidRect.LoadShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	plLecture2.LoadShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	plLecture3.LoadShaders("./Shaders/Lecture3.vs", "./Shaders/Lecture3.fs");
	plLecture3Particle.LoadShaders("./Shaders/Lecture3Particle.vs", "./Shaders/Lecture3Particle.fs");

	// Ready
	plSolidRect.Readymade();
	plLecture2.Readymade();
	plLecture3.Readymade();
	plLecture3Particle.Readymade();

	// Create VBOs
	CreateVertexBufferObjects();
	CreateParticle(1000);

	// Initialize camera settings
	cameraPositions = glm::vec3(0.f, 0.f, 1000.f);
	cameraLookAt = glm::vec3(0.f, 0.f, 0.f);
	cameraLookUp = glm::vec3(0.f, 1.f, 0.f);
	m_m4View = glm::lookAt(
		cameraPositions,
		cameraLookAt,
		cameraLookUp
	);

	//Initialize projection matrix
	m_m4OrthoProj = glm::ortho(
		(float)(-width) / 2.f, (float)(width) / 2.f,
		(float)(-height) / 2.f, (float)(width) / 2.f,
		0.0001f, 10000.f);

	m_m4PersProj = glm::perspectiveRH(45.f, 1.f, 1.f, 1000.f);

	//Initialize projection-view matrix
	m_m4ProjView = m_m4OrthoProj * m_m4View; //use ortho at this time
	//m_m4ProjView = m_m4PersProj * m_m4View;

	//Initialize model transform matrix :; used for rotating quad normal to parallel to camera direction
	m_m4Model = glm::rotate(glm::mat4(1.0f), glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
}

GLuint Renderer::CreatePipeline()
{
	return glCreateProgram();
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[] =
	{
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f, // Triangle1

		  0.5f, -0.5f, 0.0f,
		 -0.5f, 0.5f, 0.0f,
		  0.5f, 0.5f, 0.0f, // Triangle2
	};
	VertexBuffer rect_vbo(GL_ARRAY_BUFFER);
	rect_vbo.Bind(rect, sizeof(rect), GL_STATIC_DRAW);
	m_VBORect.Attach(&rect_vbo, 1);

	float rect_lecture2[] =
	{
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.f,
	};
	VertexBuffer lecture2_vbo(GL_ARRAY_BUFFER);
	lecture2_vbo.Bind(rect_lecture2, sizeof(rect_lecture2), GL_STATIC_DRAW);
	m_VBORectLecture2.Attach(&lecture2_vbo, 1);

	float rect_lecture3[] =
	{
		0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
	};
	VertexBuffer lecture3_vbo(GL_ARRAY_BUFFER);
	lecture3_vbo.Bind(rect_lecture3, sizeof(rect_lecture3), GL_STATIC_DRAW);
	m_VBORectLecture3.Attach(&lecture3_vbo, 1);

	float part_size = 0.1f;
	float rect_lecture3part[] =
	{
		// 왼쪽 위 삼각형
		-part_size, -part_size, 0.0f,//	1.0f, 1.0f, 1.0f, 1.0f,
		 part_size,  part_size, 0.0f,//	1.0f, 1.0f, 1.0f, 1.0f,
		-part_size,  part_size, 0.0f,//	1.0f, 1.0f, 1.0f, 1.0f,
		// 오른쪽 아래 삼각형
		-part_size, -part_size, 0.0f,//	1.0f, 1.0f, 1.0f, 1.0f,
		 part_size, -part_size, 0.0f,//	1.0f, 1.0f, 1.0f, 1.0f,
		 part_size,  part_size, 0.0f,//	1.0f, 1.0f, 1.0f, 1.0f,
	};
	VertexBuffer lecture3part_vbo(GL_ARRAY_BUFFER);
	lecture3part_vbo.Bind(rect_lecture3part, sizeof(rect_lecture3part), GL_STATIC_DRAW);
	m_QuadManyParticle.Attach(&lecture3part_vbo, 1);
}

void Renderer::CreateParticle(const int count)
{
	const int vertexCount = count * 3 * 2;

	// (x, y, z, sx, sy, sz) * 정점 3개 * 삼각형 2개
	const int floatCount = count * (3 + 3) * 3 * 2;
	float* particleVertices = new float[floatCount];

	int index = 0;
	float particleSize = 0.01f;
	float randomValueX = 0.f;
	float randomValueY = 0.f;
	float randomValueZ = 0.f;
	float randomValueVX = 0.f;
	float randomValueVY = 0.f;
	float randomValueVZ = 0.f;

	for (int i = 0; i < count; i++)
	{
		randomValueX = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueY = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueZ = 0.f;
		randomValueVX = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueVY = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueVZ = 0.f;

		//v0
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.1f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		//v1
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++;

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		//v2
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++;

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		//v3
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++;
		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		//v4
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++;
		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		//v5
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++;
		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ
	}

	m_QuadManyParticle.Assign(GL_ARRAY_BUFFER, 1);
	//m_QuadManyParticle.SetBufferType(GL_ARRAY_BUFFER);
	m_QuadManyParticle.BindData(particleVertices, sizeof(float) * floatCount, GL_STATIC_DRAW);

	m_iManyParticleVertexCount = vertexCount;

	delete[] particleVertices;
}

PUCHAR Renderer::loadBMPRaw(const char* image, UINT& width, UINT& height)
{
	std::cout << "Loading bmp file " << image << " ... " << std::endl;
	width = -1;
	height = -1;

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char* data;

	// Open the file
	FILE* file = NULL;
	fopen_s(&file, image, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << image << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << image << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << image << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << image << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << image << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = width * height * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << image << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char* file)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;

	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, file);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char* file)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char* bmp
		= loadBMPRaw(file, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << file << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

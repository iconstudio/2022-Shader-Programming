#include "stdafx.h"
#include "Renderer.hpp"
#include "Pipeline.hpp"
#include "Shader.hpp"
#include "VertexObjet.hpp"
#include "VertexBuffer.hpp"
#include "UniformView.hpp"
#include "AttributeView.hpp"
#include "LoadPng.h"

float Time = 0.0f;

std::random_device Random_Seed{};
std::default_random_engine Random_Engine{ Random_Seed() };
std::uniform_real<float> Random_Distribution{ 0.0f };
std::uniform_real<float> Random_NegDistribution{ -1.0f, 1.0f };

Renderer::Renderer(int width, int height)
	: plSolidRect(), plLecture2(), plLecture3(), plLecture3Particle(), plLecture4()
	, vbSolidRect()
	, vbLecture2()
	, vbLecture3(), vbQuadParticle()
	, vbQuadLecture4()
{
	glClearDepth(1.f);

	Initialize(width, height);
}

Renderer::~Renderer() {}

void Renderer::Lecture3()
{
	auto& pipeline = plLecture3;
	pipeline.Use();
	pipeline.UseBuffer(vbLecture3, GL_ARRAY_BUFFER);

	// stride 값을 줘야 제대로 된 색을 읽는다.
	GLsizei stride = sizeof(float) * 7;

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	attrPosition.Stream(GL_FLOAT, 3, stride);

	auto attrColor = pipeline.GetAttribute("a_Color");
	attrColor.EnableVertexArray();
	attrColor.Stream(GL_FLOAT, 4, stride, (GLvoid*)(sizeof(float) * 3)); // 4번째 부터 읽기

	auto uniformColor = pipeline.GetUniform("u_Color");
	uniformColor.Stream(1.0f, 1.0f, 1.0f, 1.0f);

	Render(PRIMITIVE_METHODS::TRIANGLES, 0, 3);

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
	pipeline.UseBuffer(vbQuadParticle, GL_ARRAY_BUFFER);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 정보 11개
	// (x, y, z, sx, sy, sz, et, lt, amp, period, sphere, r, g, b, a)
	constexpr GLsizei stride = sizeof(float) * 15;

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	attrPosition.Stream(GL_FLOAT, 3, stride);

	auto attrVelocity = pipeline.GetAttribute("a_Velocity");
	attrVelocity.EnableVertexArray();
	// 4번째 부터 읽기
	attrVelocity.Stream(GL_FLOAT, 3, stride, (GLvoid*)(sizeof(float) * 3));

	auto attrEmitTime = pipeline.GetAttribute("a_EmitTime");
	attrEmitTime.EnableVertexArray();
	// 7번째 부터 읽기
	attrEmitTime.Stream(GL_FLOAT, 1, stride, (GLvoid*)(sizeof(float) * 6));

	auto attrDuration = pipeline.GetAttribute("a_Duration");
	attrDuration.EnableVertexArray();
	// 8번째 부터 읽기
	attrDuration.Stream(GL_FLOAT, 1, stride, (GLvoid*)(sizeof(float) * 7));

	auto attrCurveAmp = pipeline.GetAttribute("a_CurveAmplify");
	attrCurveAmp.EnableVertexArray();
	// 9번째 부터 읽기
	attrCurveAmp.Stream(GL_FLOAT, 1, stride, (GLvoid*)(sizeof(float) * 8));

	auto attrCurvePeriod = pipeline.GetAttribute("a_CurvePeriod");
	attrCurvePeriod.EnableVertexArray();
	// 10번째 부터 읽기
	attrCurvePeriod.Stream(GL_FLOAT, 1, stride, (GLvoid*)(sizeof(float) * 9));

	auto attrSphere = pipeline.GetAttribute("a_Sphere");
	attrSphere.EnableVertexArray();
	// 11번째 부터 읽기
	attrSphere.Stream(GL_FLOAT, 1, stride, (GLvoid*)(sizeof(float) * 10));

	auto attrColor = pipeline.GetAttribute("a_Colour");
	attrColor.EnableVertexArray();
	// 12번째 부터 읽기
	attrColor.Stream(GL_FLOAT, 4, stride, (GLvoid*)(sizeof(float) * 11));

	auto uniformTime = pipeline.GetUniform("u_Time");
	uniformTime.Stream(Time);

	auto uniformAccel = pipeline.GetUniform("u_Acceleration");
	uniformAccel.Stream(0.0f, 1.0f, 0.0f); // std::cos(Time)

	auto uniformLoop = pipeline.GetUniform("u_Loop");
	uniformLoop.Stream(true);

	Time += 0.002f;
	if (1.0f <= Time)
	{
		//Time = 0.0f;
	}

	Render(PRIMITIVE_METHODS::TRIANGLES, 0, countParticleVertex);

	attrPosition.DisableVertexArray();
	attrVelocity.DisableVertexArray();
	glDisable(GL_BLEND);
}

void Renderer::Lecture4()
{
	auto& pipeline = plLecture4;
	pipeline.Use();
	pipeline.UseBuffer(vbQuadLecture4, GL_ARRAY_BUFFER);

	// 여기서 stride 값은 사실 0이어도 된다.
	GLsizei stride = sizeof(float) * 7;

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	attrPosition.Stream(GL_FLOAT, 3, stride);

	Render(PRIMITIVE_METHODS::TRIANGLES, 0, 6);

	attrPosition.DisableVertexArray();
}

void Renderer::Test()
{
	auto& pipeline = plSolidRect;
	pipeline.Use();
	pipeline.UseBuffer(vbSolidRect, GL_ARRAY_BUFFER);

	// 여기서 stride 값은 사실 0이어도 된다.
	GLsizei stride = sizeof(float) * 3;

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	attrPosition.Stream(GL_FLOAT, 3, stride);

	Render(PRIMITIVE_METHODS::TRIANGLES, 0, 6);

	attrPosition.DisableVertexArray();
}

void Renderer::Initialize(int width, int height)
{
	// Set window size
	windowWidth = width;
	windowHeight = height;

	// 
	plSolidRect.AssignProgram(CreatePipeline());
	plLecture2.AssignProgram(CreatePipeline());
	plLecture3.AssignProgram(CreatePipeline());
	plLecture3Particle.AssignProgram(CreatePipeline());
	plLecture4.AssignProgram(CreatePipeline());

	// Load shaders
	plSolidRect.LoadShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	plLecture2.LoadShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	plLecture3.LoadShaders("./Shaders/Lecture3.vs", "./Shaders/Lecture3.fs");
	plLecture3Particle.LoadShaders("./Shaders/Lecture3Particle.vs", "./Shaders/Lecture3Particle.fs");
	plLecture4.LoadShaders("./Shaders/Lecture4Sandbox.vs", "./Shaders/Lecture4Sandbox.fs");

	// Ready
	plSolidRect.Readymade();
	plLecture2.Readymade();
	plLecture3.Readymade();
	plLecture3Particle.Readymade();
	plLecture4.Readymade();

	// Create VBOs
	CreateVertexBufferObjects();
	CreateLecture3Particle(1000);

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
	constexpr float rect[] =
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
	vbSolidRect.Attach(&rect_vbo, 1);

	constexpr float rect_lecture2[] =
	{
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.f,
	};
	VertexBuffer lecture2_vbo(GL_ARRAY_BUFFER);
	lecture2_vbo.Bind(rect_lecture2, sizeof(rect_lecture2), GL_STATIC_DRAW);
	vbLecture2.Attach(&lecture2_vbo, 1);
	
	constexpr float rect_lecture3[] =
	{
		0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
	};
	VertexBuffer lecture3_vbo(GL_ARRAY_BUFFER);
	lecture3_vbo.Bind(rect_lecture3, sizeof(rect_lecture3), GL_STATIC_DRAW);
	vbLecture3.Attach(&lecture3_vbo, 1);

	constexpr float part_size = 1.0f;
	constexpr float rect_lecture3part[] =
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
	vbQuadParticle.Attach(&lecture3part_vbo, 1);

	constexpr float rect_lecture4_sz = 0.5f;
	// (x, y, z, r, g, b, a)
	constexpr float rect_lecture4[] =
	{
		// Triangle1
		-rect_lecture4_sz, -rect_lecture4_sz, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
		-rect_lecture4_sz,  rect_lecture4_sz, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 rect_lecture4_sz, -rect_lecture4_sz, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

		// Triangle2
		  rect_lecture4_sz, -rect_lecture4_sz, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 -rect_lecture4_sz,  rect_lecture4_sz, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		  rect_lecture4_sz,  rect_lecture4_sz, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	};
	VertexBuffer lecture4_vbo(GL_ARRAY_BUFFER);
	lecture4_vbo.Bind(rect_lecture4, sizeof(rect_lecture4), GL_STATIC_DRAW);
	vbQuadLecture4.Attach(&lecture4_vbo, 1);
}

void Renderer::CreateLecture3Particle(const int count)
{
	const int vertexCount = count * 3 * 2; // 정점 3개 * 삼각형 2개

	// (x, y, z, sx, sy, sz, et, lt, amp, period, sphere, r, g, b, a)
	const int floatCount = vertexCount * (3 + 3 + 2 + 2 + 1 + 4);
	float* particleVertices = new float[floatCount];

	int index = 0;

	// 반지름
	float particleSize = 0.1f;
	float randomValueX = 0.0f;
	float randomValueY = 0.0f;
	float randomValueZ = 0.0f;

	float randomValueVX = 0.0f;
	float randomValueVY = 0.0f;
	float randomValueVZ = 0.0f;

	float randomTimeEmit = 0.0f;
	float randomTimeDuration = 0.0f;

	float randomCurveAmplify = 0.0f;
	float randomCurvePeriod = 0.0f;

	float randomSphere = 0.0f;

	float randomR = 0.0f;
	float randomG = 0.0f;
	float randomB = 0.0f;
	float randomA = 0.0f;

	for (int i = 0; i < count; i++)
	{
		randomValueX = 0.f;
		randomValueY = 0.f;
		randomValueZ = 0.f;

		randomValueVX = Random_NegDistribution(Random_Engine) * 0.4f;
		randomValueVY = Random_NegDistribution(Random_Engine) * 0.5f;
		randomValueVZ = 0.0f;

		randomTimeEmit = Random_Distribution(Random_Engine) * 3.0f;
		randomTimeDuration = Random_Distribution(Random_Engine) * 4.0f;

		randomCurveAmplify = Random_NegDistribution(Random_Engine) * 0.12f;
		randomCurvePeriod = Random_Distribution(Random_Engine) * 2.0f;

		randomSphere = Random_Distribution(Random_Engine) * 1.0f;

		randomR = Random_Distribution(Random_Engine) * 1.0f;
		randomG = Random_Distribution(Random_Engine) * 1.0f;
		randomB = Random_Distribution(Random_Engine) * 1.0f;
		randomA = Random_Distribution(Random_Engine) * 1.0f;

		// v0
		// Position XYZ
		particleVertices[index++] = -particleSize + randomValueX;
		particleVertices[index++] = -particleSize + randomValueY;
		particleVertices[index++] = 0.1f;
		// Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		// Lifetimes
		particleVertices[index++] = randomTimeEmit;
		particleVertices[index++] = randomTimeDuration;
		// Curve
		particleVertices[index++] = randomCurveAmplify;
		particleVertices[index++] = randomCurvePeriod;
		// 
		particleVertices[index++] = randomSphere;
		// RGBA
		particleVertices[index++] = randomR;
		particleVertices[index++] = randomG;
		particleVertices[index++] = randomB;
		particleVertices[index++] = randomA;

		// v1
		particleVertices[index++] = particleSize + randomValueX;
		particleVertices[index++] = -particleSize + randomValueY;
		particleVertices[index++] = 0.f;
		// Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		// Lifetimes
		particleVertices[index++] = randomTimeEmit;
		particleVertices[index++] = randomTimeDuration;
		// Curve
		particleVertices[index++] = randomCurveAmplify;
		particleVertices[index++] = randomCurvePeriod;
		// 
		particleVertices[index++] = randomSphere;
		// RGBA
		particleVertices[index++] = randomR;
		particleVertices[index++] = randomG;
		particleVertices[index++] = randomB;
		particleVertices[index++] = randomA;

		// v2
		particleVertices[index++] = particleSize + randomValueX;
		particleVertices[index++] = particleSize + randomValueY;
		particleVertices[index++] = 0.f;
		// Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		// Lifetimes
		particleVertices[index++] = randomTimeEmit;
		particleVertices[index++] = randomTimeDuration;
		// Curve
		particleVertices[index++] = randomCurveAmplify;
		particleVertices[index++] = randomCurvePeriod;
		// 
		particleVertices[index++] = randomSphere;
		// RGBA
		particleVertices[index++] = randomR;
		particleVertices[index++] = randomG;
		particleVertices[index++] = randomB;
		particleVertices[index++] = randomA;

		//v3
		particleVertices[index++] = -particleSize + randomValueX;
		particleVertices[index++] = -particleSize + randomValueY;
		particleVertices[index++] = 0.f;
		//Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		// Lifetimes
		particleVertices[index++] = randomTimeEmit;
		particleVertices[index++] = randomTimeDuration;
		// Curve
		particleVertices[index++] = randomCurveAmplify;
		particleVertices[index++] = randomCurvePeriod;
		// 
		particleVertices[index++] = randomSphere;
		// RGBA
		particleVertices[index++] = randomR;
		particleVertices[index++] = randomG;
		particleVertices[index++] = randomB;
		particleVertices[index++] = randomA;

		// v4
		particleVertices[index++] = particleSize + randomValueX;
		particleVertices[index++] = particleSize + randomValueY;
		particleVertices[index++] = 0.f;
		// Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		// Lifetimes
		particleVertices[index++] = randomTimeEmit;
		particleVertices[index++] = randomTimeDuration;
		// Curve
		particleVertices[index++] = randomCurveAmplify;
		particleVertices[index++] = randomCurvePeriod;
		// 
		particleVertices[index++] = randomSphere;
		// RGBA
		particleVertices[index++] = randomR;
		particleVertices[index++] = randomG;
		particleVertices[index++] = randomB;
		particleVertices[index++] = randomA;

		// v5
		particleVertices[index++] = -particleSize + randomValueX;
		particleVertices[index++] = particleSize + randomValueY;
		particleVertices[index++] = 0.f;
		//Velocity XYZ
		particleVertices[index++] = randomValueVX;
		particleVertices[index++] = randomValueVY;
		particleVertices[index++] = 0.f;
		// Lifetimes
		particleVertices[index++] = randomTimeEmit;
		particleVertices[index++] = randomTimeDuration;

		particleVertices[index++] = randomCurveAmplify;
		particleVertices[index++] = randomCurvePeriod;
		// 
		particleVertices[index++] = randomSphere;
		// RGBA
		particleVertices[index++] = randomR;
		particleVertices[index++] = randomG;
		particleVertices[index++] = randomB;
		particleVertices[index++] = randomA;
	}

	vbQuadParticle.Assign(GL_ARRAY_BUFFER, 1);
	//vbQuadParticle.SetBufferType(GL_ARRAY_BUFFER);
	vbQuadParticle.BindData(particleVertices, sizeof(float) * floatCount, GL_STATIC_DRAW);

	countParticleVertex = vertexCount;

	delete[] particleVertices;
}

void Renderer::Render(PRIMITIVE_METHODS method, GLint first, GLsizei count)
{
	glDrawArrays(static_cast<GLenum>(method), first, count);
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

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
std::uniform_real<float> Random_Distribution{ 0.0f, 1.0f };
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

	GLsizei stride = sizeof(float) * 7;

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	attrPosition.Stream(GL_FLOAT, 3, stride);

	Render(PRIMITIVE_METHODS::TRIANGLES, 0, 6);

	attrPosition.DisableVertexArray();
}

const float g_ptLecture4[] = {
	Random_Distribution(Random_Engine) - 0.5f
		, Random_Distribution(Random_Engine)
		, 0.01f
		, Random_Distribution(Random_Engine)
		, Random_Distribution(Random_Engine)
		, 0.01f
		, Random_Distribution(Random_Engine)
		, Random_Distribution(Random_Engine)
		, 0.01f
		, Random_Distribution(Random_Engine)
		, Random_Distribution(Random_Engine)
		, 0.01f
		, Random_Distribution(Random_Engine)
		, Random_Distribution(Random_Engine)
		, 0.01f
		, Random_Distribution(Random_Engine)
		, Random_Distribution(Random_Engine)
		, 0.01f
		, Random_Distribution(Random_Engine)
		, Random_Distribution(Random_Engine)
		, 0.01f
		, Random_Distribution(Random_Engine)
		, Random_Distribution(Random_Engine)
		, 0.01f
		, Random_Distribution(Random_Engine)
		, Random_Distribution(Random_Engine)
		, 0.01f
		, Random_Distribution(Random_Engine)
		, Random_Distribution(Random_Engine)
		, 0.01f
};

void Renderer::Lecture4Raindrop()
{
	auto& pipeline = plLecture4;
	pipeline.Use();
	pipeline.UseBuffer(vbQuadLecture4, GL_ARRAY_BUFFER);

	GLsizei stride = sizeof(float) * 7;

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	attrPosition.Stream(GL_FLOAT, 3, stride);

	auto uniformPoints = pipeline.GetUniform("u_Points");
	glUniform3fv(uniformPoints.Self, 10, g_ptLecture4);

	auto uniformTime = pipeline.GetUniform("u_Time");
	uniformTime.Stream(Time);
	Time += 0.001f;

	Render(PRIMITIVE_METHODS::TRIANGLES, 0, 6);

	attrPosition.DisableVertexArray();
}

void Renderer::Lecture4RaderCircle()
{
	auto& pipeline = plLecture4;
	pipeline.Use();

	// 원본
	pipeline.UseBuffer(vbQuadLecture4, GL_ARRAY_BUFFER);
	GLsizei stride = sizeof(float) * 7;
	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	attrPosition.Stream(GL_FLOAT, 3, stride);

	/*
	//* Package 0
	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();

	// (x, y, z)
	pipeline.UseBuffer(vboPackaged0Pos, GL_ARRAY_BUFFER);
	// 0이어도 된다.
	GLsizei pos_stride = sizeof(float) * 3;
	attrPosition.Stream(GL_FLOAT, 3, pos_stride);

	auto attrColour = pipeline.GetAttribute("a_Colour");
	attrColour.EnableVertexArray();

	// (r, g, b, a)
	pipeline.UseBuffer(vboPackaged0Color, GL_ARRAY_BUFFER);
	// 0이어도 된다.
	GLsizei col_stride = sizeof(float) * 4;
	attrColour.Stream(GL_FLOAT, 4, col_stride);
	//*/

	//*
	/* Packge 1
	// (x, y, z, r, g, b, a)
	pipeline.UseBuffer(vboPackaged1, GL_ARRAY_BUFFER);

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	auto attrColour = pipeline.GetAttribute("a_Colour");
	attrColour.EnableVertexArray();

	GLsizei pos_stride = sizeof(float) * 7;
	attrPosition.Stream(GL_FLOAT, 3, pos_stride);
	GLsizei col_stride = sizeof(float) * 7;
	attrColour.Stream(GL_FLOAT, 4, col_stride, (GLvoid*)(sizeof(float) * 3));
	//*/

	/* Packge 2
	// (x, y, z, ..., r, g, b, a)
	pipeline.UseBuffer(vboPackaged2, GL_ARRAY_BUFFER);

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	auto attrColour = pipeline.GetAttribute("a_Colour");
	attrColour.EnableVertexArray();

	// 위치는 3개 간격
	GLsizei pos_stride = sizeof(float) * 3;
	attrPosition.Stream(GL_FLOAT, 3, pos_stride);

	// 색은 4개 간격
	GLsizei col_stride = sizeof(float) * 4;
	// 색상은 맨 뒤로 밀려나있다.
	attrColour.Stream(GL_FLOAT, 4, col_stride, (GLvoid*)(sizeof(float) * 18));
	//*/

	auto uniformPoints = pipeline.GetUniform("u_Points");
	glUniform3fv(uniformPoints.Self, 10, g_ptLecture4);

	auto uniformTime = pipeline.GetUniform("u_Time");
	uniformTime.Stream(Time);
	Time += 0.001f;

	Render(PRIMITIVE_METHODS::TRIANGLES, 0, 6);

	attrPosition.DisableVertexArray();
}

void Renderer::Lecture5LineSegment()
{
	auto& pipeline = plLecture5Curve;
	pipeline.Use();

	// (x, y, z)
	pipeline.UseBuffer(vboLecture5LineSegment, GL_ARRAY_BUFFER);

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();

	// 3개 간격
	GLsizei pos_stride = sizeof(float) * 3;
	attrPosition.Stream(GL_FLOAT, 3, pos_stride);

	auto uniformTime = pipeline.GetUniform("u_Time");
	uniformTime.Stream(Time);
	Time += 0.01f;

	Render(PRIMITIVE_METHODS::LINE_STRIP, 0, vboLecture5SegCount);

	attrPosition.DisableVertexArray();
}

void Renderer::Lecture5Fullfil()
{
	auto& pipeline = plLecture5Fullfil;
	pipeline.Use();

	pipeline.UseBuffer(vboLecture5Fullfil, GL_ARRAY_BUFFER);

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();

	GLsizei pos_stride = sizeof(float) * 3;
	attrPosition.Stream(GL_FLOAT, 3, pos_stride);

	auto uniformTime = pipeline.GetUniform("u_Time");
	uniformTime.Stream(Time);
	Time += 0.01f;

	Render(PRIMITIVE_METHODS::TRIANGLES, 0, 6);

	attrPosition.DisableVertexArray();
}

void Renderer::Lecture6Texture()
{
	auto& pipeline = plLecture6Tex;
	pipeline.Use();
	pipeline.UseBuffer(vboLecture6Positions, GL_ARRAY_BUFFER);

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();

	auto attrTexCoord = pipeline.GetAttribute("a_TexCoord");
	attrTexCoord.EnableVertexArray();

	GLsizei pos_stride = sizeof(float) * 5;
	attrPosition.Stream(GL_FLOAT, 3, pos_stride);
	auto begin = (GLvoid*)(sizeof(float) * 3);
	attrTexCoord.Stream(GL_FLOAT, 2, pos_stride, begin);

	auto uniformTex = pipeline.GetUniform("u_TexRGB");
	auto index_uniformTex = uniformTex.Self;
	glUniform1i(index_uniformTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texLecture6RGB);

	uniformTex = pipeline.GetUniform("u_TexChecker");
	index_uniformTex = uniformTex.Self;
	glUniform1i(index_uniformTex, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texLecture6Checker);

	auto uniformTime = pipeline.GetUniform("u_Time");
	uniformTime.Stream(Time);
	Time += 0.001f;

	Render(PRIMITIVE_METHODS::TRIANGLES, 0, 6);

	attrPosition.DisableVertexArray();
	attrTexCoord.DisableVertexArray();
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::Lecture8MultiTextures()
{}

void Renderer::Lecture9GridMesh()
{
	auto& pipeline = plLecture9Flag;
	pipeline.Use();
	pipeline.UseBuffer(vboLecture9GridMesh, GL_ARRAY_BUFFER);

	auto attrPosition = pipeline.GetAttribute("a_Position");
	attrPosition.EnableVertexArray();
	attrPosition.Stream(GL_FLOAT, 3, 0, 0);

	auto uniformTime = pipeline.GetUniform("u_Time");
	uniformTime.Stream(Time);
	Time += 0.01f;

	// TRIANGLES
	Render(PRIMITIVE_METHODS::LINE_STRIP, 0, gDummyVertexCount);

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
	plLecture5Curve.AssignProgram(CreatePipeline());
	plLecture5Fullfil.AssignProgram(CreatePipeline());
	plLecture6Tex.AssignProgram(CreatePipeline());
	plLecture8MultipleTextures.AssignProgram(CreatePipeline());
	plLecture9Flag.AssignProgram(CreatePipeline());

	// Load shaders
	plSolidRect.LoadShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	plLecture2.LoadShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	plLecture3.LoadShaders("./Shaders/Lecture3.vs", "./Shaders/Lecture3.fs");
	plLecture3Particle.LoadShaders("./Shaders/Lecture3Particle.vs", "./Shaders/Lecture3Particle.fs");
	plLecture4.LoadShaders("./Shaders/Lecture4Sandbox.vs", "./Shaders/Lecture4Sandbox.fs");
	plLecture5Curve.LoadShaders("./Shaders/Lecture5Curve.vs", "./Shaders/Lecture5Curve.fs");
	plLecture5Fullfil.LoadShaders("./Shaders/FullRect.vs", "./Shaders/FullRect.fs");
	plLecture6Tex.LoadShaders("./Shaders/Lecture6TexV.glsl", "./Shaders/Lecture6TexP.glsl");
	plLecture8MultipleTextures.LoadShaders("./Shaders/Lecture8TexsV.glsl", "./Shaders/Lecture8TexsP.glsl");
	plLecture9Flag.LoadShaders("./Shaders/Lecture9GridV.glsl", "./Shaders/Lecture9GridP.glsl");

	// Ready
	plSolidRect.Readymade();
	plLecture2.Readymade();
	plLecture3.Readymade();
	plLecture3Particle.Readymade();
	plLecture4.Readymade();
	plLecture5Curve.Readymade();
	plLecture5Fullfil.Readymade();
	plLecture6Tex.Readymade();
	plLecture8MultipleTextures.Readymade();
	plLecture9Flag.Readymade();

	// Create VBOs
	CreateVertexBufferObjects();
	CreateLecture3Particle(1000);
	CreateLecture4Objects();
	CreateLecture5Line(40);
	CreateLecture6Textures();
	CreateLecture8Textures();
	CreateLecture9Flag();

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

	std::cout << "초기화 완료" << std::endl;
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

	constexpr float part_size = 0.7f;
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

void Renderer::CreateLecture4Objects()
{
	constexpr float rect_lecture4_sz = 0.8f;
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

	// (x, y, z)
	constexpr float rect4_packaged0_pos[] =
	{
		-rect_lecture4_sz, -rect_lecture4_sz, 0.0f, //
		-rect_lecture4_sz, +rect_lecture4_sz, 0.0f,
		 rect_lecture4_sz, -rect_lecture4_sz, 0.0f,

		 +rect_lecture4_sz, -rect_lecture4_sz, 0.0, //
		 -rect_lecture4_sz, +rect_lecture4_sz, 0.0,
		 +rect_lecture4_sz, +rect_lecture4_sz, 0.0,
	};

	// glGenBuffer(1, lecture4_pack0_pos_vbo);
	// glGenBuffer(BL_ARRAY_BUFFER, lecture4_pack0_pos_vbo);
	VertexBuffer lecture4_pack0_pos_vbo(GL_ARRAY_BUFFER);
	lecture4_pack0_pos_vbo.Bind(rect4_packaged0_pos, sizeof(rect4_packaged0_pos), GL_STATIC_DRAW);
	vboPackaged0Pos.Attach(&lecture4_pack0_pos_vbo, 1);

	// (r, g, b, a)
	constexpr float rect4_packaged0_color[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f, //
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f, //
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	VertexBuffer lecture4_pack0_col_vbo(GL_ARRAY_BUFFER);
	lecture4_pack0_col_vbo.Bind(rect4_packaged0_color, sizeof(rect4_packaged0_color), GL_STATIC_DRAW);
	vboPackaged0Color.Attach(&lecture4_pack0_col_vbo, 1);

	// (x, y, z, r, g, b, a)
	constexpr float rec4t_packaged1[] =
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
	VertexBuffer lecture4_pack1_vbo(GL_ARRAY_BUFFER);
	lecture4_pack1_vbo.Bind(rec4t_packaged1, sizeof(rec4t_packaged1), GL_STATIC_DRAW);
	vboPackaged1.Attach(&lecture4_pack1_vbo, 1);

	// (x0, y0, z0, x1, y1, z1, ... , r0, g0, b0, a0, ...)
	constexpr float rec4t_packaged2[] =
	{
		// Triangle1 pos
		-rect_lecture4_sz, -rect_lecture4_sz, 0.0f,
		-rect_lecture4_sz,  rect_lecture4_sz, 0.0f,
		 rect_lecture4_sz, -rect_lecture4_sz, 0.0f,

		// Triangle2 pos
		  rect_lecture4_sz, -rect_lecture4_sz, 0.0f,
		 -rect_lecture4_sz,  rect_lecture4_sz, 0.0f,
		  rect_lecture4_sz,  rect_lecture4_sz, 0.0f,

		// Triangle1 color
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

		// Triangle2 color
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	};
	VertexBuffer lecture4_pack2_vbo(GL_ARRAY_BUFFER);
	lecture4_pack2_vbo.Bind(rec4t_packaged2, sizeof(rec4t_packaged2), GL_STATIC_DRAW);
	vboPackaged2.Attach(&lecture4_pack2_vbo, 1);
}

void Renderer::CreateLecture5Line(int seg_count)
{
	const int float_count = seg_count * 3;
	auto line_vertices = new float[float_count];

	vboLecture5SegCount = seg_count;

	int index = 0;
	for (int i = 0; i < seg_count; ++i)
	{
		// x
		line_vertices[index++] = -1.0f + float(i) / float(seg_count - 1) * 2.0f;
		// y
		line_vertices[index++] = 0.0f;
		// z
		line_vertices[index++] = 0.0f;
	}

	VertexBuffer lecture5_seg(GL_ARRAY_BUFFER);
	lecture5_seg.Bind(line_vertices, sizeof(float) * float_count, GL_STATIC_DRAW);
	vboLecture5LineSegment.Attach(&lecture5_seg, 1);

	constexpr float rect_sz = 1.0f;

	// (x, y, z) * 6개
	constexpr float ptLecture5[] = {
		-rect_sz, -rect_sz, 0.0f,
		+rect_sz, +rect_sz, 0.0f,
		-rect_sz, +rect_sz, 0.0f,
		-rect_sz, -rect_sz, 0.0f,
		+rect_sz, -rect_sz, 0.0f,
		+rect_sz, +rect_sz, 0.0f
	};

	VertexBuffer lecture5_filler(GL_ARRAY_BUFFER);
	lecture5_filler.Bind(ptLecture5, sizeof(ptLecture5), GL_STATIC_DRAW);
	vboLecture5Fullfil.Attach(&lecture5_filler, 1);
}

void Renderer::CreateLecture6Textures()
{
	texLecture6RGB = CreatePngTexture("image1.png");

	constexpr GLulong checkerboard[] =
	{
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	};

	glGenTextures(1, &texLecture6Checker);
	glBindTexture(GL_TEXTURE_2D, texLecture6Checker);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	constexpr float rect_sz = 0.5f;
	// (x, y, z, u, v)
	constexpr float myrect[] =
	{
		// Triangle1 pos
		-rect_sz, -rect_sz, 0.0f, 0.0f, 0.0f,
		+rect_sz, -rect_sz, 0.0f, 1.0f, 0.0f,
		+rect_sz, +rect_sz, 0.0f, 1.0f, 1.0f,

		// Triangle2 pos
		-rect_sz, -rect_sz, 0.0f, 0.0f, 0.0f,
		-rect_sz, +rect_sz, 0.0f, 0.0f, 1.0f,
		+rect_sz, +rect_sz, 0.0f, 1.0f, 1.0f,
	};

	VertexBuffer lecture6_rect_vbo(GL_ARRAY_BUFFER);
	lecture6_rect_vbo.Bind(myrect, sizeof(myrect), GL_STATIC_DRAW);
	vboLecture6Positions.Attach(&lecture6_rect_vbo, 1);
}

void Renderer::CreateLecture8Textures()
{
	constexpr GLulong textureSmile[] =
	{
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};

	glGenTextures(1, &vboLecture8Tex0);
	glBindTexture(GL_TEXTURE_2D, vboLecture8Tex0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSmile);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	constexpr GLulong textureSmile1[] =
	{
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};

	glGenTextures(1, &vboLecture8Tex1);
	glBindTexture(GL_TEXTURE_2D, vboLecture8Tex1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSmile1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	constexpr GLulong textureSmile2[] =
	{
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};

	glGenTextures(1, &vboLecture8Tex2);
	glBindTexture(GL_TEXTURE_2D, vboLecture8Tex2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSmile2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	constexpr GLulong textureSmile3[] =
	{
	0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};

	glGenTextures(1, &vboLecture8Tex3);
	glBindTexture(GL_TEXTURE_2D, vboLecture8Tex3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSmile3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	constexpr GLulong textureSmile4[] =
	{
	0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};

	glGenTextures(1, &vboLecture8Tex4);
	glBindTexture(GL_TEXTURE_2D, vboLecture8Tex4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSmile4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	constexpr GLulong textureSmile5[] =
	{
	0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};

	glGenTextures(1, &vboLecture8Tex5);
	glBindTexture(GL_TEXTURE_2D, vboLecture8Tex5);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSmile5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Renderer::CreateLecture9Flag()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	gDummyVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
		//Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;

			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	const auto size_rect = sizeof(float) * 2 * 3;
	const auto size_frag = size_rect * (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	VertexBuffer lecture9_flag_vbo(GL_ARRAY_BUFFER);
	lecture9_flag_vbo.Bind(vertices, size_frag, GL_STATIC_DRAW);
	vboLecture9GridMesh.Attach(&lecture9_flag_vbo, 1);
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

GLuint Renderer::CreatePngTexture(const char* file)
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

GLuint Renderer::CreateBmpTexture(const char* file)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char* bmp = loadBMPRaw(file, width, height);

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

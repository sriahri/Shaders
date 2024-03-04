#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <MouseCamera.h>
#include <Mesh.h>
#include <Shader.h>
#include <Texture.h>

const char* WINDOW_TITLE = "[CSCI-4800/5800] Shader and GPU Programming";
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
GLint g_glutWindowIdentifier;

using namespace sgpu;

std::shared_ptr<MouseCameraf> camera = nullptr;
std::shared_ptr<Mesh> mesh_0 = nullptr;
std::shared_ptr<Mesh> mesh_1 = nullptr;

float phi = 0.001f;
float r = 18.0f;
float x, y;

struct ADSColor {
	Color3<float> ambient;
	Color3<float> diffuse;
	Color3<float> specular;
};

ADSColor light;
ADSColor material_0, material_1;


void g_init() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glewInit();

	camera = std::make_shared<MouseCameraf>(3.0f);

	mesh_0 = std::make_shared<Mesh>();
	mesh_0->load("models/teapot.obj");
	mesh_0->loadShader("shaders/GouraudShading.vert", "shaders/GouraudShading.frag");

	mesh_1 = std::make_shared<Mesh>();
	mesh_1->load("models/teapot.obj");
	mesh_1->loadShader("shaders/GouraudShadingGold.vert", "shaders/GouraudShading.frag");

	mesh_0->getShader()->uniformColor("light_ambient", light.ambient);
	mesh_0->getShader()->uniformColor("light_diffuse", light.diffuse);
	mesh_0->getShader()->uniformColor("light_specular", light.specular);
	mesh_0->getShader()->uniformColor("material_ambient", material_0.ambient);
	mesh_0->getShader()->uniformColor("material_diffuse", material_0.diffuse);
	mesh_0->getShader()->uniformColor("material_specular", material_0.specular);

	mesh_1->getShader()->uniformColor("light_ambient", light.ambient);
	mesh_1->getShader()->uniformColor("light_diffuse", light.diffuse);
	mesh_1->getShader()->uniformColor("light_specular", light.specular);
	mesh_1->getShader()->uniformColor("material_ambient", material_1.ambient);
	mesh_1->getShader()->uniformColor("material_diffuse", material_1.diffuse);
	mesh_1->getShader()->uniformColor("material_specular", material_1.specular);

	camera->setPosition(18.0f, 1.5707f, 1.570f * 0.7f);
}

void g_glutReshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
	camera->setPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	glutPostRedisplay();
}

void g_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	x = r * std::cos(phi);
	y = r * std::sin(phi);

	mesh_0->setPosition(-8.0f, -2.0f, 0.0f);
	mesh_1->setPosition(6.0f, -2.0f, 0.0f);

	Matrix4f model = mesh_0->getTransform().toMatrix();
	Matrix4f view = camera->getViewMatrix();
	Matrix4f modelViewMatrix = model * camera->getViewMatrix();
	Matrix4f normalMatrix = Matrix4f::Transpose(camera->getViewMatrix().toInverse());
	Matrix4f projectionMatrix = camera->getProjectionMatrix();

	mesh_0->beginRender();
	mesh_0->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix);
	mesh_0->getShader()->uniformMatrix("projectionMatrix", projectionMatrix);
	mesh_0->getShader()->uniformMatrix("normalMatrix", normalMatrix);
	mesh_0->getShader()->uniformVector("lightPosition", Vector3f(x, 0.0f, y));
	mesh_0->endRender();

	Matrix4f model_1 = mesh_1->getTransform().toMatrix();
	Matrix4f view_1 = camera->getViewMatrix();
	Matrix4f modelViewMatrix_1 = model_1 * camera->getViewMatrix();
	Matrix4f normalMatrix_1 = Matrix4f::Transpose(camera->getViewMatrix().toInverse());
	Matrix4f projectionMatrix_1 = camera->getProjectionMatrix();

	mesh_1->beginRender();
	mesh_1->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix_1);
	mesh_1->getShader()->uniformMatrix("projectionMatrix", projectionMatrix_1);
	mesh_1->getShader()->uniformMatrix("normalMatrix", normalMatrix_1);
	mesh_1->getShader()->uniformVector("lightPosition", Vector3f(x, 0.0f, y));
	mesh_1->endRender();

	glutSwapBuffers();
	glFlush();
}

void g_glutMotionFunc(int x, int y) {
	camera->onMouseMove(x, y);
	glutPostRedisplay();
}

void g_glutIdleFunc() {
	phi += 0.008363f;
	glutPostRedisplay();
	std::this_thread::sleep_for(std::chrono::milliseconds(16));
}

void g_glutMouseFunc(int button, int state, int x, int y) {
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) camera->onMouseButton(LB_DOWN, x, y);
	if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN ) camera->onMouseButton(RB_DOWN, x, y);
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP ) camera->onMouseButton(LB_UP, x, y);
	if ( button == GLUT_RIGHT_BUTTON && state == GLUT_UP ) camera->onMouseButton(RB_UP, x, y);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	g_glutWindowIdentifier = glutCreateWindow(WINDOW_TITLE);

	glutDisplayFunc(g_glutDisplayFunc);
	glutReshapeFunc(g_glutReshapeFunc);
	glutMotionFunc(g_glutMotionFunc);
	glutMouseFunc(g_glutMouseFunc);
	glutIdleFunc(g_glutIdleFunc);

	g_init();

	glutMainLoop();
	return 0;
}


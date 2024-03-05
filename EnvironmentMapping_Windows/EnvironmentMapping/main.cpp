#include <iostream>
#include <memory>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <MouseCamera.h>
#include <Mesh.h>
#include <Shader.h>
#include <Texture.h>
#include <EnvironmentMap.h>

using namespace sgpu;

const char* WINDOW_TITLE = "[CSCI-4800/5800] Shader and GPU Programming";
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
GLint g_glutWindowIdentifier;

std::shared_ptr<MouseCameraf> camera = nullptr;
std::shared_ptr<Mesh> mesh = nullptr;
std::shared_ptr<EnvironmentMap> map = nullptr;

float phi = 0.0f;
float r = 18.0f;
float x, y;

void g_init() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glewInit();

    mesh = std::make_shared<Mesh>();
    mesh->load("models/teapot.obj");
    mesh->loadShader("shaders/EnvironmentMapping.vert", "shaders/EnvironmentMapping.frag");
    
    camera = std::make_shared<MouseCameraf>(3.0f);
    camera->setPosition(18.0f, 1.5707f, 1.570f * 0.7f);

    map = std::make_shared<EnvironmentMap>();
    map->load("images/river");
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

	Matrix4f model = mesh->getTransform().toMatrix();
	Matrix4f view = camera->getViewMatrix();
	Matrix4f modelViewMatrix = model * camera->getViewMatrix();
	Matrix4f normalMatrix = Matrix4f::Transpose(camera->getViewMatrix().toInverse());
	Matrix4f projectionMatrix = camera->getProjectionMatrix();

    mesh->beginRender();
    mesh->getShader()->uniformMatrix("projectionMatrix", projectionMatrix);
    mesh->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix);
    mesh->getShader()->uniformMatrix("normalMatrix", normalMatrix);
    mesh->getShader()->uniformVector("lightPosition", Vector3f(x, 2.0f, y));
    mesh->getShader()->uniformVector("cameraPosition", camera->getEye());
    mesh->endRender();

	glutSwapBuffers();
	glFlush();
}

void g_glutMotionFunc(int x, int y) {
	camera->onMouseMove(x, y);
	glutPostRedisplay();
}

void g_glutIdleFunc() {
    phi += 0.004363323;
    glutPostRedisplay();
    Sleep(16);
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


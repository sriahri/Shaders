#include <iostream>
#include <memory>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <MouseCamera.h>
#include <Mesh.h>
#include <Shader.h>
#include <Texture.h>

using namespace sgpu;

const char* WINDOW_TITLE = "[CSCI-4800/5800] Shader and GPU Programming";
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
GLint g_glutWindowIdentifier;

const static unsigned int TIMERMSECS = 33;

std::shared_ptr<MouseCameraf> camera = nullptr;
std::shared_ptr<Mesh> mesh = nullptr;
std::shared_ptr<Shader> phongShader = nullptr;
std::shared_ptr<GeometryShader> normalShader = nullptr;

float normalScale = 1.0f;

void g_init() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glewInit();

    camera = std::make_shared<MouseCameraf>(3.0f);
    mesh = std::make_shared<Mesh>();

    mesh->load("models/teapot.obj");

    phongShader = std::make_shared<Shader>();
    phongShader->load("shaders/PhongShading.vert", "shaders/PhongShading.frag");
    phongShader->compile();
    phongShader->link();

    normalShader = std::make_shared<GeometryShader>();
    normalShader->load("shaders/NormalVisualization.vert", "shaders/NormalVisualization.geom", "shaders/NormalVisualization.frag");
    normalShader->compile();
    normalShader->link();

    camera->setPosition(18.0f, 1.5707f, 1.570f * 0.7f);
}

void g_glutReshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
    camera->setPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	glutPostRedisplay();
}

void g_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix4f model = mesh->getTransform().toMatrix();
	Matrix4f view = camera->getViewMatrix();
	Matrix4f modelViewMatrix = model * camera->getViewMatrix();
	Matrix4f normalMatrix = Matrix4f::Transpose(camera->getViewMatrix().toInverse());
	Matrix4f projectionMatrix = camera->getProjectionMatrix();

    /* First Pass: Phong Surface */
    mesh->setShader(phongShader);
    mesh->beginRender();
        mesh->getShader()->uniformMatrix("projectionMatrix", projectionMatrix);
        mesh->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix);
        mesh->getShader()->uniformMatrix("normalMatrix", normalMatrix);
        mesh->getShader()->uniformVector("lightPosition", Vector3f(0.0f, 1.0f, 20.0f));
    mesh->endRender();

    /* Second Pass: Normals */
    mesh->setShader(normalShader);
    mesh->beginRender();
        mesh->getShader()->uniformMatrix("projectionMatrix", projectionMatrix);
        mesh->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix);
        mesh->getShader()->uniformMatrix("normalMatrix", normalMatrix);
        mesh->getShader()->uniform1f("normalScale", normalScale);
    mesh->endRender();

    glutSwapBuffers();
	glFlush();
}

void g_glutMotionFunc(int x, int y) {
	camera->onMouseMove(x, y);
	glutPostRedisplay();
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

	g_init();

	glutMainLoop();
	return 0;
}


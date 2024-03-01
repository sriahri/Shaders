#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <MouseCamera.h>
#include <Mesh.h>
#include <Shader.h>

using namespace sgpu;

const char* WINDOW_TITLE = "[CSCI-4800/5800] Shader and GPU Programming";
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
GLint g_glutWindowIdentifier;

std::shared_ptr<MouseCameraf> camera = nullptr;
std::shared_ptr<Mesh> mesh = nullptr;

void g_init() {
	glClearColor(1.1f, 1.1f, 1.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glewInit();

    camera = std::make_shared<MouseCameraf>(3.0f);
    mesh = std::make_shared<Mesh>();

    if ( !mesh->load("models/Teapot.obj") ) {
        std::cerr << "[Main] Error: Could not load mesh." << std::endl;
        std::cin.get();
        std::exit(1);
    }

    if ( !mesh->loadShader("shaders/PassThrough.vert", "shaders/PassThrough.frag") ) {
        std::cerr << "[Main] Error: Could not load mesh." << std::endl;
        std::cin.get();
        std::exit(1);
    }

    camera->setPosition(18.0f, 1.5707f, 1.570f * 0.7f);
}

void g_glutReshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
    camera->setPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	glutPostRedisplay();
}

void g_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    mesh->beginRender();
    mesh->getShader()->uniformMatrix("projectionMatrix", camera->getProjectionMatrix());
    mesh->getShader()->uniformMatrix("modelViewMatrix", camera->getViewMatrix());
    mesh->getShader()->uniformMatrix("normalMatrix", Matrix4f::Transpose(camera->getViewMatrix().toInverse()));
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
	//glutInitContextVersion(4, 5);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	
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


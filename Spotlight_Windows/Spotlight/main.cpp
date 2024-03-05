#include <iostream>
#include <memory>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <MouseCamera.h>
#include <Mesh.h>
#include <Shader.h>

using namespace sgpu;

const char* WINDOW_TITLE = "[CSCI-4800/5800] Shader and GPU Programming";
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
GLint g_glutWindowIdentifier;

std::shared_ptr<MouseCameraf> camera = nullptr;
std::shared_ptr<Mesh> plane = nullptr;

/* Spotlight Structure */
struct SpotLight {
    Vector3f position;
    Vector3f target;
    Vector3f ambient;
    Vector3f diffuse;
    Vector3f specular;
    float exponent;
    float cutoff;
};

SpotLight light;

void g_init() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glewInit();

    camera = std::make_shared<MouseCameraf>(3.0f);
    camera->setPosition(18.0f, 1.5707f, 1.570f * 0.7f);

    plane = std::make_shared<Mesh>();
	plane->load("models/plane.obj");
	plane->loadShader("shaders/SpotLight.vert", "shaders/SpotLight.frag");
	plane->setDiffuseTexture("textures/grid_diffuse.png");
	plane->setNormalTexture("textures/grid_normal.png");
	plane->setSpecularTexture("textures/grid_specular.png");

	light.position = Vector3f(4.0f, 4.0f, 4.0f);
	light.target = Vector3f(0.0f, 0.0f, 0.0f);
	light.ambient = Vector3f(0.1f, 0.1f, 0.1f);
	light.diffuse = Vector3f(0.5f, 0.5f, 0.5f);
	light.specular = Vector3f(0.6f, 0.6f, 0.6f);
	light.exponent = 4.0f;
	light.cutoff = 30.0f;
}

void g_glutReshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
    camera->setPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	glutPostRedisplay();
}

void g_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	Matrix4f model = plane->getTransform().toMatrix();
	Matrix4f view = camera->getViewMatrix();
	Matrix4f modelViewMatrix = model * camera->getViewMatrix();
	Matrix4f normalMatrix = Matrix4f::Transpose(camera->getViewMatrix().toInverse());
	Matrix4f projectionMatrix = camera->getProjectionMatrix();

	plane->beginRender();
	auto shader = plane->getShader();
		shader->uniformMatrix("projectionMatrix", projectionMatrix);
		shader->uniformMatrix("modelViewMatrix", modelViewMatrix);
		shader->uniformMatrix("normalMatrix", normalMatrix);
		shader->uniformVector("light.position", light.position);
		shader->uniformVector("light.target", light.target);
		shader->uniformVector("light.ambient", light.ambient);
		shader->uniformVector("light.diffuse", light.diffuse);
		shader->uniformVector("light.specular", light.specular);
		shader->uniform1f("light.exponent", light.exponent);
		shader->uniform1f("light.cutoff", light.cutoff);
	plane->endRender();

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

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
std::shared_ptr<Mesh> plane = nullptr;
std::shared_ptr<Mesh> mesh = nullptr;
std::shared_ptr<EnvironmentMap> map = nullptr;

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

/* Three Spotlights */
const int LIGHT_COUNT = 4;
SpotLight lights[LIGHT_COUNT];

void g_init() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glewInit();

    camera = std::make_shared<MouseCameraf>(3.0f);
    camera->setPosition(18.0f, 1.5707f, 1.570f * 0.7f);

    plane = std::make_shared<Mesh>();
	plane->load("models/plane.obj");
	plane->loadShader("shaders/SpotLights.vert", "shaders/SpotLights.frag");
	// TODO: Load textures for plane
	plane->setDiffuseTexture("textures/grid_diffuse.png");
	plane->setNormalTexture("textures/grid_normal.png");
	plane->setSpecularTexture("textures/grid_specular.png");

	mesh = std::make_shared<Mesh>();
	mesh->load("models/teapot.obj");
	mesh->loadShader("shaders/CubemapSpotlights.vert", "shaders/CubemapSpotlights.frag");

	// TODO: Fill in light properties. 
	// Recall that spot lights have both position and target.
	lights[0].position = Vector3f(-3.0f, 4.0f, 3.0f);
	lights[0].target = Vector3f(0.0f, 0.0f, 0.0f);
	lights[0].ambient = Vector3f(0.1f, 0.0f, 0.0f);
	lights[0].diffuse = Vector3f(0.7f, 0.0f, 0.0f);
	lights[0].specular = Vector3f(1.0f, 0.0f, 0.0f);
	lights[0].exponent = 4.0f;
	lights[0].cutoff = 30.0f;

	lights[1].position = Vector3f(3.0f, 4.0f, 3.0f);
	lights[1].target = Vector3f(0.0f, 0.0f, 0.0f);
	lights[1].ambient = Vector3f(0.0f, 0.1f, 0.0f);
	lights[1].diffuse = Vector3f(0.0f, 0.7f, 0.0f);
	lights[1].specular = Vector3f(0.0f, 1.0f, 0.0f);
	lights[1].exponent = 4.0f;
	lights[1].cutoff = 30.0f;

	lights[2].position = Vector3f(-3.0f, 4.0f, -3.0f);
	lights[2].target = Vector3f(0.0f, 0.0f, 0.0f);
	lights[2].ambient = Vector3f(0.0f, 0.0f, 0.1f);
	lights[2].diffuse = Vector3f(0.0f, 0.0f, 0.7f);
	lights[2].specular = Vector3f(0.0f, 0.0f, 1.0f);
	lights[2].exponent = 4.0f;
	lights[2].cutoff = 30.0f;

	lights[3].position = Vector3f(3.0f, 4.0f, -3.0f);
	lights[3].target = Vector3f(0.0f, 0.0f, 0.0f);
	lights[3].ambient = Vector3f(0.1f, 0.1f, 0.1f);
	lights[3].diffuse = Vector3f(0.7f, 0.7f, 0.7f);
	lights[3].specular = Vector3f(1.0f, 1.0f, 1.0f);
	lights[3].exponent = 4.0f;
	lights[3].cutoff = 30.0f;
	// ...
	
	// TODO: Load environment map. Be sure to include
	// the required images directory with the cube-map.
	map = std::make_shared<EnvironmentMap>();
	map->load("images/river", "png");

}

void g_glutReshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
    camera->setPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	glutPostRedisplay();
}

void UniformSpotlight(const std::shared_ptr<Mesh>& mesh, int index) {
	auto light = lights[index];
	auto shader = mesh->getShader();

	shader->uniformVector("lights[" + std::to_string(index) + "].position", light.position);
	shader->uniformVector("lights[" + std::to_string(index) + "].target", light.target);
	shader->uniformVector("lights[" + std::to_string(index) + "].ambient", light.ambient);
	shader->uniformVector("lights[" + std::to_string(index) + "].diffuse", light.diffuse);
	shader->uniformVector("lights[" + std::to_string(index) + "].specular", light.specular);
	shader->uniform1f("lights[" + std::to_string(index) + "].exponent", light.exponent);
	shader->uniform1f("lights[" + std::to_string(index) + "].cutoff", light.cutoff);
}

void RenderGround() {
	Matrix4f model = plane->getTransform().toMatrix();
	Matrix4f view = camera->getViewMatrix();
	Matrix4f modelViewMatrix = model * camera->getViewMatrix();
	Matrix4f normalMatrix = Matrix4f::Transpose(camera->getViewMatrix().toInverse());
	Matrix4f projectionMatrix = camera->getProjectionMatrix();

	plane->beginRender();
		plane->getShader()->uniformMatrix("projectionMatrix", projectionMatrix);
		plane->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix);
		plane->getShader()->uniformMatrix("normalMatrix", normalMatrix);
		// TODO: Uniform the spotlight properties
		for (int i = 0; i < LIGHT_COUNT; i++) {
			UniformSpotlight(plane, i);
		}

	plane->endRender();
}

void RenderModel() {
	Matrix4f model = mesh->getTransform().toMatrix();
	Matrix4f view = camera->getViewMatrix();
	Matrix4f modelViewMatrix = model * camera->getViewMatrix();
	Matrix4f normalMatrix = Matrix4f::Transpose(camera->getViewMatrix().toInverse());
	Matrix4f projectionMatrix = camera->getProjectionMatrix();

	mesh->beginRender();
		mesh->getShader()->uniformMatrix("projectionMatrix", projectionMatrix);
		mesh->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix);
		mesh->getShader()->uniformMatrix("normalMatrix", normalMatrix);
		// Environment mapping also requires the camera eye position.
		mesh->getShader()->uniformVector("cameraPosition", camera->getEye());
		// TODO: Uniform the spotlight properties
		for (int i = 0; i < LIGHT_COUNT; i++) {
			UniformSpotlight(mesh, i);
		}

	mesh->endRender();
}

void g_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	RenderGround();
	RenderModel();

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

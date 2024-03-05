#include <iostream>
#include <memory>
#include <vector> 
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

typedef std::shared_ptr<Mesh> SharedMesh;
typedef std::vector<std::string> MeshResourceList;

struct Light {
	Vector3f position;
	Vector3f ambient;
	Vector3f diffuse;
	Vector3f specular;
};

// 3 Different color lights
const int LIGHT_COUNT = 3;

// 4 Meshes with mesh[0] being the ground mesh, meshes[1-3] are the spheres.
const int MESH_COUNT = 4;

std::shared_ptr<MouseCameraf> camera = nullptr;
std::vector<SharedMesh> meshes;
std::vector<Light> lights;

enum MeshResource {
	MODEL_NAME,
	DIFFUSE_NAME,
	NORMAL_NAME,
	SPECULAR_NAME
};

// TODO: Modify the SpecularMapping example shader
const std::string VERTEX_SHADER = "shaders/SpecularMapping.vert";
const std::string FRAGMENT_SHADER = "shaders/SpecularMapping.frag";

// The ground grid texture does not have to be changed.
MeshResourceList Ground_Mesh_Resources = {
	"models/plane.obj",
	"textures/grid_diffuse.png",
	"textures/grid_normal.png",
	"textures/grid_specular.png"
};

// TODO: default.png is a blank square. Fill with your own textures
MeshResourceList Mesh_0_Resources = {
	"models/geosphere.obj",
	"textures/smooth_albedo.png",
	"textures/smooth_normal.png",
	"textures/smooth_specular.png"
};

MeshResourceList Mesh_1_Resources = {
	"models/geosphere.obj",
	"textures/brushed_albedo.png",
	"textures/brushed_normal.png",
	"textures/brushed_specular.png"
};

MeshResourceList Mesh_2_Resources = {
	"models/geosphere.obj",
	"textures/stone_albedo.png",
	"textures/stone_normal.png",
	"textures/stone_specular.png"
};

/* Quck function for loading a mesh given the shader/texture file names. */
std::shared_ptr<Mesh> LoadMesh(const MeshResourceList& res) {
	if ( res.empty() ) return nullptr;
	auto mesh = std::make_shared<Mesh>();
	mesh->load(res[MODEL_NAME]);
	mesh->loadShader(VERTEX_SHADER, FRAGMENT_SHADER);
	mesh->setDiffuseTexture(res[DIFFUSE_NAME]);
	mesh->setNormalTexture(res[NORMAL_NAME]);
	mesh->setSpecularTexture(res[SPECULAR_NAME]);
	return mesh;
}

void SetupMeshes() {
	meshes.resize(MESH_COUNT);
	meshes[0] = LoadMesh(Ground_Mesh_Resources);
	meshes[1] = LoadMesh(Mesh_0_Resources);
	meshes[2] = LoadMesh(Mesh_1_Resources);
	meshes[3] = LoadMesh(Mesh_2_Resources);

	// To match the positions shown in the guide:
	meshes[0]->setPosition(0.0f, 0.0f, 0.0f);
	meshes[1]->setPosition(-8.0f, 3.0f, 0.0f);
	meshes[2]->setPosition(0.0f, 3.0f, 0.0f);
	meshes[3]->setPosition(8.0f, 3.0f, 0.0f);
}

void SetupLights() {
	lights.resize(LIGHT_COUNT);
	lights[0].position = Vector3f(6.0f, 6.0f, 6.0f);
	lights[0].ambient = Vector3f(0.9f, 0.1f, 0.1f);
	lights[0].diffuse = Vector3f(1.0f, 0.1f, 0.1f);
	lights[0].specular = Vector3f(0.9f, 0.5f, 0.5f);

	// Light 1 (Green light on the left)
	lights[1].position = Vector3f(-8.0f, 2.0f, 12.0f);
	lights[1].ambient = Vector3f(0.1f, 0.9f, 0.1f);
	lights[1].diffuse = Vector3f(0.2f, 0.7f, 0.2f);
	lights[1].specular = Vector3f(0.5f, 1.0f, 0.5f);

	// Light 2 (Blue light at the back)
	lights[2].position = Vector3f(2.0f, 8.0f, 8.0f);
	lights[2].ambient = Vector3f(0.1f, 0.1f, 0.9f);
	lights[2].diffuse = Vector3f(0.2f, 0.2f, 0.7f);
	lights[2].specular = Vector3f(0.5f, 0.5f, 1.0f);

}

void g_init() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glewInit();

    camera = std::make_shared<MouseCameraf>(3.0f);
    camera->setPosition(18.0f, 1.5707f, 1.570f * 0.7f);

	SetupMeshes();
	SetupLights();
}

void g_glutReshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
    camera->setPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	glutPostRedisplay();
}

void g_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix4f projectionMatrix = camera->getProjectionMatrix();
	Matrix4f view = camera->getViewMatrix();
	Matrix4f normalMatrix = Matrix4f::Transpose(view.toInverse());
	Matrix4f model, modelViewMatrix;

	// TODO: Main render pass
	// (1) for each mesh -> uniformMatrix for projection, modelView, normal matrices
	//		(2) for each light -> uniform position, ambient, diffuse, specular

	for ( int mesh_index = 0; mesh_index < meshes.size(); mesh_index++ ) {
		auto mesh = meshes[mesh_index];

		model = mesh->getTransform().toMatrix();
		modelViewMatrix = model * camera->getViewMatrix();
	
		mesh->beginRender();
		mesh->getShader()->uniformMatrix("projectionMatrix", projectionMatrix);
		mesh->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix);
		mesh->getShader()->uniformMatrix("normalMatrix", normalMatrix);

		for ( int light_index = 0; light_index < LIGHT_COUNT; light_index++ ) {
			// TODO
			std::string lightPrefix = "lights[" + std::to_string(light_index) + "].";
			mesh->getShader()->uniformVector(lightPrefix + "position", lights[light_index].position);
			mesh->getShader()->uniformVector(lightPrefix + "ambient", lights[light_index].ambient);
			mesh->getShader()->uniformVector(lightPrefix + "diffuse", lights[light_index].diffuse);
			mesh->getShader()->uniformVector(lightPrefix + "specular", lights[light_index].specular);
		}
		mesh->endRender();
	}

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


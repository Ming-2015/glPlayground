#include "TestTriangle.h"

TestTriangle::TestTriangle(const GameResources& resources)
  : GameState(resources),
  _mClearColor(0.5, 0.7, 0.3, 1.0),
  cameraController(nullptr),
  mat(nullptr),
  model(nullptr),
  pointLights(),
  lightBox(nullptr)
{}

TestTriangle::~TestTriangle()
{}

// resouce allocation/deallocation
void TestTriangle::_onLoad()
{
  // our model
  model = new Box(_mResources.primitiveManager);
  mat = new PhongMaterial(&_mResources.shaderProgramManager);
  model->material = mat;

  // set the textures for the material
  TextureInfo wallTex(
    "assets/wall.jpg",
    true
  );
  mat->specularTex = _mResources.textureManager.getOrCreate(wallTex);

  TextureInfo fujiwaraTex(
    "assets/fujiwara.jpg",
    true
  );
  mat->diffuseTex = _mResources.textureManager.getOrCreate(fujiwaraTex);
  mat->ambientTex = _mResources.textureManager.getOrCreate(fujiwaraTex);
  mat->shininess = 64.f;
  _mScene.addChild(model);

  for (int i = -60; i < 60; i++)
  {
    for (int j = -60; j < 60; j++) 
    {
      if (i == 0 && j == 0)  continue;
      Model* clone = model->clone();
      clone->setPosition(glm::vec3(i * 3, 0, j * 3));
      _mScene.addChild(clone);
    }
  }

  _mCamera = new FreeCamera();
  _mCamera->setPosition(glm::vec3(0, 0, 2));
  _mCamera->setMaxZ(1000000.0f);
  FirstPersonFreeCameraController* c = new FirstPersonFreeCameraController(_mResources.window, _mCamera);
  c->moveSpeed *= 10;
  cameraController = c;
  _mScene.setActiveCamera(_mCamera, true);

  PointLight* pointLight = new PointLight();
  pointLights.push_back(pointLight);
  pointLight->ambient = glm::vec3(0.05f);
  pointLight->specular = glm::vec3(0.3f);
  pointLight->diffuse = glm::vec3(0.5f);
  pointLight->attenuationVal = 0.25;
  _mScene.addLight(pointLight, true);

  Box* lightBox = new Box(_mResources.primitiveManager, .3f, .3f, .3f, true);
  PhongMaterial* mat = new PhongMaterial(&_mResources.shaderProgramManager);
  lightBox->material = mat;
  mat->diffuse = pointLight->diffuse * .3f;
  mat->specular = pointLight->specular * .1f;
  mat->ambient = pointLight->ambient * .1f;
  pointLight->addChild(lightBox);

  pointLight = new PointLight();
  pointLights.push_back(pointLight);
  pointLight->ambient = glm::vec3(.05f);
  pointLight->specular = glm::vec3(1.f);
  pointLight->diffuse = glm::vec3(.7f, .6f, .1f);
  _mScene.addLight(pointLight, true);

  lightBox = new Box(_mResources.primitiveManager, .3f, .3f, .3f, true);
  mat = new PhongMaterial(&_mResources.shaderProgramManager);
  lightBox->material = mat;
  mat->diffuse = pointLight->diffuse * .3f;
  mat->specular = pointLight->specular * .1f;
  mat->ambient = pointLight->ambient * .1f;
  pointLight->addChild(lightBox);

  DirLight* dirLight = new DirLight();
  dirLights.push_back(dirLight);
  dirLight->diffuse = glm::vec3(.3f);
  dirLight->specular = glm::vec3(.2f);
  dirLight->ambient = glm::vec3(.1f);
  dirLight->direction = glm::normalize(glm::vec3(1.f, -2.f, -3.f));
  _mScene.addLight(dirLight);

  // some global settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void TestTriangle::_onUpdate(float deltaT)
{
  cameraController->update(deltaT);
  
  currentAngle += deltaT / 1000.0f * rotateSpeed;
  model->setRotationQuaternion(glm::angleAxis(currentAngle, glm::vec3(0, 1, 0)));

  lightAngle += deltaT / 1000.f * lightRotateSpeed;
  float x = -glm::cos(lightAngle) * distFromCenter;
  float z = glm::sin(lightAngle) * distFromCenter;
  pointLights[0]->setPosition(glm::vec3(x, .5f, z));

  //pointLight->diffuse = glm::normalize(glm::vec3(glm::abs(x), glm::abs(x/z), glm::abs(z)));
  //pointLight->specular = glm::vec3(0.5);

  pointLights[1]->setPosition(glm::vec3(x, z, 0));
}

void TestTriangle::_onDraw()
{
  glClearColor(_mClearColor.r, _mClearColor.g, _mClearColor.b, _mClearColor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TestTriangle::_onDestroy()
{
  _mCamera = nullptr;
  model = nullptr;

  delete mat;
  mat = nullptr;

  delete cameraController;
  cameraController = nullptr;

  _mResources.primitiveManager.erase(PrimitiveInfo("test"));
  _mResources.textureManager.erase(TextureInfo(
    "assets/fujiwara.jpg",
    true
  ));

  _mResources.textureManager.erase(TextureInfo(
    "assets/wall.jpg",
    true
  ));
}

// returns a non-empty string if the next state should be rendered
const std::string& TestTriangle::nextState() const
{
  return "";
}

void TestTriangle::onKey(int key, int scancode, int action, int mods)
{

  bool flag = false;
  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    flag = true;
  }
  else if (action == GLFW_RELEASE)
  {
    flag = false;
  }
}

void TestTriangle::onCursorPos(double xPos, double yPos)
{}

void TestTriangle::onMouseButton(int key, int action, int mods)
{}

void TestTriangle::onResize(int width, int height)
{
  glViewport(0, 0, width, height);
}
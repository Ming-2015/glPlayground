#include "TestTriangle.h"

TestTriangle::TestTriangle(const GameResources& resources)
  : GameState(resources),
  _mClearColor(0.5, 0.7, 0.3, 1.0),
  cameraController(nullptr),
  mat(nullptr),
  model(nullptr),
  pointLights()
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
  std::string wallTexPath = "assets/wall.jpg";
  TextureData wallTex(
    wallTexPath,
    true
  );
  mat->specularTex = _mResources.textureManager.insert(wallTexPath, wallTex);

  std::string fujiwaraTexPath = "assets/fujiwara.jpg";
  TextureData fujiwaraTex(
    fujiwaraTexPath,
    true
  );

  mat->diffuseTex = _mResources.textureManager.insert(fujiwaraTexPath, fujiwaraTex);
  mat->ambientTex = mat->diffuseTex;
  mat->shininess = 64.f;
  _mScene.addChild(model);
  model->setPosition(glm::vec3(2, 0.5f, 0));

  _mCamera = new FreeCamera();
  _mCamera->setPosition(glm::vec3(0, 0, -2.f));
  _mCamera->setMaxZ(1000000.0f);
  _mCamera->setForwardDirection(glm::vec3(0, 1, 0));
  FirstPersonFreeCameraController* c = new FirstPersonFreeCameraController(_mResources.window, _mCamera);
  cameraController = c;
  _mScene.setActiveCamera(_mCamera, true);

  PointLight* pointLight = new PointLight();
  pointLights.push_back(pointLight);
  pointLight->ambient = glm::vec3(0.05f);
  pointLight->specular = glm::vec3(0.3f);
  pointLight->diffuse = glm::vec3(0.7f);
  // _mScene.addLight(pointLight, true);

  Box* lightBox = new Box(_mResources.primitiveManager, .3f, .3f, .3f, true);
  PhongMaterial* mat = new PhongMaterial(&_mResources.shaderProgramManager);
  lightBox->material = mat;
  mat->diffuse = glm::vec4(pointLight->diffuse * .3f, 1.f);
  mat->specular = glm::vec4(pointLight->specular * .1f, 1.f);
  mat->ambient = glm::vec4(pointLight->ambient * .1f, 1.f);
  pointLight->addChild(lightBox);

  pointLight = new PointLight();
  pointLights.push_back(pointLight);
  pointLight->ambient = glm::vec3(.05f);
  pointLight->specular = glm::vec3(1.f);
  pointLight->diffuse = glm::vec3(.7f, .6f, .1f);
  // _mScene.addLight(pointLight, true);

  Model* boxClone = lightBox->clone();
  pointLight->addChild(boxClone);

  DirLight* dirLight = new DirLight();
  dirLights.push_back(dirLight);
  dirLight->diffuse = glm::vec3(0.6f);
  dirLight->specular = glm::vec3(0.4f);
  dirLight->ambient = glm::vec3(.1f);
  dirLight->direction = glm::normalize(glm::vec3(1.f, -.5f, 1.f));
  _mScene.addLight(dirLight);

  dirLight = dirLight->clone();
  dirLight->direction = glm::normalize(glm::vec3(0, 0, -1.f));
  _mScene.addLight(dirLight);

  dirLight = dirLight->clone();
  dirLight->direction = glm::normalize(glm::vec3(-1.f, -0.2f, 0));
  _mScene.addLight(dirLight);

  // import the hell of this shit!
  importer = new AssetImporter(_mResources, "./assets/crysis_nano_suit_2/scene.gltf");
  importer->load();
  Asset* asset = importer->getOriginal();
  asset->setScale(glm::vec3(0.2f));
  asset->setPosition(glm::vec3(-4, 0, 0));
  _mScene.addChild(asset);

  importer = new AssetImporter(_mResources, "./assets/miku_gltf/scene.gltf");
  importer->load();
  asset = importer->getOriginal();
  asset->setPosition(glm::vec3(-2, 0, 0));
  _mScene.addChild(asset);

  importer = new AssetImporter(_mResources, "./assets/sponza/sponza.obj");
  importer->load();
  asset = importer->getOriginal();
  asset->setScale(glm::vec3(0.02f));
  _mScene.addChild(asset);

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
  auto pos = pointLights[1]->getAbsolutePosition();
}

void TestTriangle::_onDraw()
{
  glClearColor(_mClearColor.r, _mClearColor.g, _mClearColor.b, _mClearColor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TestTriangle::_onDestroy()
{
  _mCamera = nullptr;
  model = nullptr;

  delete mat;
  mat = nullptr;

  delete cameraController;
  cameraController = nullptr;

  _mResources.textureManager.erase("assets/fujiwara.jpg");
  _mResources.textureManager.erase("assets/wall.jpg");
}

// returns a non-empty string if the next state should be rendered
std::string TestTriangle::nextState() const
{
  return "";
}

void TestTriangle::onKey(int key, int scancode, int action, int mods)
{}

void TestTriangle::onCursorPos(double xPos, double yPos)
{}

void TestTriangle::onMouseButton(int key, int action, int mods)
{}
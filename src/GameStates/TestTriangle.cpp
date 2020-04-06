#include "TestTriangle.h"

TestTriangle::TestTriangle(const GameResources& resources)
  : GameState(resources),
  _mClearColor(0.5, 0.7, 0.3, 1.0),
  _mCamera(nullptr),
  mat(nullptr),
  model(nullptr),
  pointLight(nullptr),
  lightBox(nullptr)
{}

TestTriangle::~TestTriangle()
{}

// resouce allocation/deallocation
void TestTriangle::_onLoad()
{
  // our model
  model = new Box(_mResources.primitiveManager);
  mat = new PhoonMaterial(&_mResources.shaderProgramManager);
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

  _mCamera = new FreeCamera();
  _mCamera->setPosition(glm::vec3(0, 0, 2));

  _mScene.addChild(model);
  _mScene.setActiveCamera(_mCamera, true);

  pointLight = new PointLight();
  pointLight->ambient = glm::vec3(0.1f);
  pointLight->specular = glm::vec3(0.7f);
  _mScene.addLight(pointLight, true);
  lightBox = new Box(_mResources.primitiveManager, .3f, .3f, .3f, true);
  lightBox->material = new PhoonMaterial(&_mResources.shaderProgramManager);
  pointLight->addChild(lightBox);

  // some global settings
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void TestTriangle::_onUpdate(float deltaT)
{
  if (goUp)
  {
    glm::vec3 forward = deltaT / 1000.f * _mMoveSpeed * _mCamera->getForwardDirection();
    _mCamera->setPosition(_mCamera->getPosition() + forward);
  }

  if (goDown)
  {
    glm::vec3 backward = deltaT / 1000.f * -_mMoveSpeed * _mCamera->getForwardDirection();
    _mCamera->setPosition(_mCamera->getPosition() + backward);
  }

  if (goLeft)
  {
    glm::vec3 left = deltaT / 1000.f * -_mMoveSpeed * glm::normalize(
      glm::cross(_mCamera->getForwardDirection(), _mCamera->getUp())
    );
    _mCamera->setPosition(_mCamera->getPosition() + left);
  }

  if (goRight)
  {
    glm::vec3 right = deltaT / 1000.f * _mMoveSpeed * glm::normalize(
      glm::cross(_mCamera->getForwardDirection(), _mCamera->getUp())
    );
    _mCamera->setPosition(_mCamera->getPosition() + right);
  }

  currentAngle += deltaT / 1000.0f * rotateSpeed;
  model->setRotationQuaternion(glm::angleAxis(currentAngle, glm::vec3(0, 1, 0)));

  lightAngle += deltaT / 1000.f * lightRotateSpeed;
  float x = -glm::cos(lightAngle) * distFromCenter;
  float z = glm::sin(lightAngle) * distFromCenter;
  pointLight->setPosition(glm::vec3(x, distFromCenter, z));

  //pointLight->diffuse = glm::normalize(glm::vec3(glm::abs(x), glm::abs(x/z), glm::abs(z)));
  //pointLight->specular = glm::vec3(0.5);
}

void TestTriangle::_onDraw()
{
  glClearColor(_mClearColor.r, _mClearColor.g, _mClearColor.b, _mClearColor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  PhoonMaterial* lightBoxMat = dynamic_cast<PhoonMaterial*> (lightBox->material);
  if (lightBoxMat)
  {
    lightBoxMat->diffuse = glm::vec4(pointLight->diffuse, 1.f);
    lightBoxMat->specular = glm::vec4(pointLight->specular, 1.f);
    lightBoxMat->ambient = glm::vec4(pointLight->ambient * .1f, 1.f);
  }
}

void TestTriangle::_onDestroy()
{
  _mCamera = nullptr;
  model = nullptr;

  delete mat;
  mat = nullptr;

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

  // key events here...
  switch (key)
  {
  case GLFW_KEY_W:
  case GLFW_KEY_UP:
    goUp = flag;
    break;

  case GLFW_KEY_S:
  case GLFW_KEY_DOWN:
    goDown = flag;
    break;

  case GLFW_KEY_A:
  case GLFW_KEY_LEFT:
    goLeft = flag;
    break;

  case GLFW_KEY_D:
  case GLFW_KEY_RIGHT:
    goRight = flag;
    break;
  }
}

void TestTriangle::onCursorPos(double xPos, double yPos)
{
  if (firstTime)
  {
    cursorX = xPos;
    cursorY = yPos;
    firstTime = false;
    return;
  }

  cursorMoveX = xPos - cursorX;
  cursorMoveY = cursorY - yPos;
  cursorX = xPos;
  cursorY = yPos;


  if (cursorMoveX)
  {
    float factor = cursorMoveX * _mHorizontalSpeed;
    yaw += factor;
    cursorMoveX = 0;
  }

  if (cursorMoveY)
  {
    float factor = cursorMoveY * _mVerticalSpeed;
    pitch += factor;
    if (pitch > 89.9f)
    {
      pitch = 89.9f;
    }
    if (pitch < -89.9f)
    {
      pitch = -89.9f;
    }
    cursorMoveY = 0;
  }

  if (!_mCamera->isUsingTarget())
  {
    glm::vec3 forward;
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    _mCamera->setForwardDirection(forward);
  }
}

void TestTriangle::onMouseButton(int key, int action, int mods)
{

}

void TestTriangle::onResize(int width, int height)
{
  glViewport(0, 0, width, height);

  if (_mCamera) {
    _mCamera->setAspectRatio(float(width) / float(height));
    Log.print<Severity::debug>("Managed to change aspect ratio!");
  }
}
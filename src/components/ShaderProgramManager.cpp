#include "ShaderProgramManager.h"

// ShaderProgramInfo
ShaderProgramInfo::ShaderProgramInfo(
  const ShaderInfo& pVertexShaderInfo,
  const ShaderInfo& pFragmentShader
) : vertexShaderInfo(pVertexShaderInfo),
    fragmentShaderInfo(pFragmentShader),
    geometryShaderInfo()
{}

ShaderProgramInfo::ShaderProgramInfo(
  const ShaderInfo& pVertexShader,
  const ShaderInfo& pFragmentShader,
  const ShaderInfo& pGeometryShader
) : vertexShaderInfo(pVertexShader),
    fragmentShaderInfo(pFragmentShader),
    geometryShaderInfo(pGeometryShader)
{}

ShaderProgramInfo::ShaderProgramInfo()
  : vertexShaderInfo(), 
  fragmentShaderInfo(), 
  geometryShaderInfo()
{}

bool ShaderProgramInfo::operator< (const ShaderProgramInfo& other) const {
  std::vector< std::pair<const ShaderInfo*, const ShaderInfo*> > pairs = {
    { &vertexShaderInfo, &other.vertexShaderInfo },
    { &fragmentShaderInfo, &other.fragmentShaderInfo },
    { &geometryShaderInfo, &other.geometryShaderInfo }
  };

  for (auto pair : pairs)
  {
    const ShaderInfo& first = *(pair.first);
    const ShaderInfo& second = *(pair.second);

    if (first < second) {
      return true;
    }
    else if (first == second) {
      continue;
    }
    else
    {
      return false;
    }
  }

  return false;
}

bool ShaderProgramInfo::operator== (const ShaderProgramInfo& other) const {
  bool eq1 = vertexShaderInfo == other.vertexShaderInfo;
  bool eq2 = fragmentShaderInfo == other.fragmentShaderInfo;
  bool eq3 = geometryShaderInfo == other.geometryShaderInfo;

  return eq1 && eq2 && eq3;
}

bool ShaderProgramInfo::isValid() const
{
  return vertexShaderInfo.isValid() && fragmentShaderInfo.isValid();
}

const std::string ShaderProgramInfo::toString() const
{
  return "{ vs: " + vertexShaderInfo.toString() + ",\n"
    " fs: " + fragmentShaderInfo.toString() + ",\n" 
    " gs: " + geometryShaderInfo.toString() + "\n" + " }";
}

bool ShaderProgramInfo::hasGeometryShader() const
{
  return geometryShaderInfo.isValid();
}

// ShaderProgram
ShaderProgram::ShaderProgram()
  : _mShaderProgramId(0), _mIsLoaded(false)
{}

ShaderProgram::~ShaderProgram()
{
  deleteShaderProgram();
}

void checkProgramLinkingError(unsigned int programId)
{
  int success;
  char infoLog[512];
  glGetProgramiv(programId, GL_LINK_STATUS, &success);
  if (!success) {
    glDeleteProgram(programId);

    glGetProgramInfoLog(programId, 512, NULL, infoLog);
    std::cerr << "ERROR: Linking of shader program failed!" << std::endl;
    std::cerr << infoLog << std::endl;
    throw "Failed to link shader program";
  }
}

void ShaderProgram::initShaderProgram(
  const Shader& vertexShader,
  const Shader& fragmentShader
)
{
  if (_mIsLoaded) {
    throw "Shader Program is already loaded!";
  }

  // attach shaders
  _mShaderProgramId = glCreateProgram();
  glAttachShader(_mShaderProgramId, vertexShader.getShaderId());
  glAttachShader(_mShaderProgramId, fragmentShader.getShaderId());
  glLinkProgram(_mShaderProgramId);

  // detach shaders
  glDetachShader(_mShaderProgramId, vertexShader.getShaderId());
  glDetachShader(_mShaderProgramId, fragmentShader.getShaderId());

  // check for linking error
  try {
    checkProgramLinkingError(_mShaderProgramId);
  }
  catch (const char* msg) {
    _mShaderProgramId = 0;
    throw msg;
  }

  _mIsLoaded = true;
  std::cout << "Shader Program successfully loaded!" << std::endl;
}

void ShaderProgram::initShaderProgram(
  const Shader& vertexShader,
  const Shader& fragmentShader,
  const Shader& geometryShader
)
{
  if (_mIsLoaded) {
    throw "Shader Program is already loaded!";
  }

  // create program
  _mShaderProgramId = glCreateProgram();
  glAttachShader(_mShaderProgramId, vertexShader.getShaderId());
  glAttachShader(_mShaderProgramId, fragmentShader.getShaderId());
  glAttachShader(_mShaderProgramId, geometryShader.getShaderId());
  glLinkProgram(_mShaderProgramId);

  // detach shaders
  glDetachShader(_mShaderProgramId, vertexShader.getShaderId());
  glDetachShader(_mShaderProgramId, fragmentShader.getShaderId());
  glDetachShader(_mShaderProgramId, geometryShader.getShaderId());

  // check for linking error
  try {
    checkProgramLinkingError(_mShaderProgramId);
  }
  catch (const char* msg) {
    _mShaderProgramId = 0;
    throw msg;
  }

  _mIsLoaded = true;
  std::cout << "Shader Program successfully loaded!" << std::endl;
}

void ShaderProgram::deleteShaderProgram()
{
  if (_mIsLoaded)
  {
    glDeleteProgram(_mShaderProgramId);
    std::cout << "Shader Program successfully deleted!" << std::endl;
    std::cout << _mShaderProgramInfo.toString() << std::endl;

    _mShaderProgramId = 0;
    _mIsLoaded = false;
    _mShaderProgramInfo = ShaderProgramInfo();
  }
}

unsigned int ShaderProgram::getShaderProgramId() const
{
  return _mShaderProgramId;
}

bool ShaderProgram::isLoaded() const
{
  return _mIsLoaded;
}

void ShaderProgram::useProgram() const
{
  glUseProgram(_mShaderProgramId);
}

const ShaderProgramInfo& ShaderProgram::getShaderProgramInfo() const
{
  return _mShaderProgramInfo;
}

// Shader Program Manager
ShaderProgramManager::ShaderProgramManager(ShaderManager& shaderManager)
  : _mShaderManager(shaderManager)
{}

ShaderProgram* const ShaderProgramManager::create(const ShaderProgramInfo& key)
{
  ShaderProgram* program = new ShaderProgram();

  auto vs = _mShaderManager.getOrCreate(key.vertexShaderInfo);
  auto fs = _mShaderManager.getOrCreate(key.fragmentShaderInfo);
  if (key.hasGeometryShader()) {
    auto gs = _mShaderManager.getOrCreate(key.geometryShaderInfo);
    program->initShaderProgram(*vs, *fs, *gs);
  }
  else {
    program->initShaderProgram(*vs, *fs);
  }

  return program;
}
void ShaderProgramManager::destroy(ShaderProgram* const value)
{
  delete value;
}
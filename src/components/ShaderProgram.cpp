#include "ShaderProgram.h"

// ShaderProgramInfo
ShaderProgramData::ShaderProgramData(
  const std::string& pVertexShader,
  const std::string& pFragmentShader
) : vertexShaderKey(pVertexShader),
    fragmentShaderKey(pFragmentShader),
    geometryShaderKey()
{}

ShaderProgramData::ShaderProgramData(
  const std::string& pVertexShader,
  const std::string& pFragmentShader,
  const std::string& pGeometryShader
) : vertexShaderKey(pVertexShader),
    fragmentShaderKey(pFragmentShader),
    geometryShaderKey(pGeometryShader)
{}

ShaderProgramData::ShaderProgramData()
  : vertexShaderKey(), 
  fragmentShaderKey(), 
  geometryShaderKey()
{}

const std::string ShaderProgramData::toString() const
{
  return "{ vs: " + vertexShaderKey + ",\n"
    " fs: " + fragmentShaderKey + ",\n" 
    " gs: " + geometryShaderKey + "\n" + " }";
}

bool ShaderProgramData::hasGeometryShader() const
{
  return geometryShaderKey.size() > 0;
}

// ShaderProgram
ShaderProgram::ShaderProgram()
  : _mId(0), _mIsLoaded(false)
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

    glGetProgramInfoLog(programId, 512, NULL, infoLog);
    Log.print<Severity::error>("Linking of shader program failed!");
    Log.print<Severity::error>(infoLog);

    glDeleteProgram(programId);
    throw std::exception("Failed to link shader program");
  }
}

void ShaderProgram::parseProgramInfo()
{
  int count;
  glGetProgramiv(_mId, GL_ACTIVE_UNIFORMS, &count);

  for (int idx = 0; idx < count; idx++)
  {
    char name[32];
    GLsizei length; // length of name
    GLsizei size;   // size of uniform variable
    GLenum type;    // data type
    glGetActiveUniform(_mId, idx, sizeof(name), &length, &size, &type, name);

    std::string sName(name);
    Uniform* uniform = new Uniform(_mId, idx, type, size, sName);
    _mUniforms.push_back(uniform);

    // handle all maps
    _mUniformMap.insert({ sName, uniform });
    if (sName.length() > 3) {
      std::string last = sName.substr(sName.length() - 3, 3);
      if (last == "[0]")
      {
        std::string first = sName.substr(0, sName.length() - 3);
        Log.print<Severity::debug>("Found an array uniform: ", first);

        _mUniformMap.insert({ first, uniform });
      }
    }
  }
}

void ShaderProgram::initShaderProgram(
  const Shader& vertexShader,
  const Shader& fragmentShader
)
{
  if (_mIsLoaded) {
    throw std::exception("Shader Program is already loaded!");
  }

  // attach shaders
  _mId = glCreateProgram();
  glAttachShader(_mId, vertexShader.getShaderId());
  glAttachShader(_mId, fragmentShader.getShaderId());
  glLinkProgram(_mId);

  // detach shaders
  glDetachShader(_mId, vertexShader.getShaderId());
  glDetachShader(_mId, fragmentShader.getShaderId());

  // check for linking error
  try {
    checkProgramLinkingError(_mId);
  }
  catch (std::exception msg) {
    _mId = 0;
    throw msg;
  }

  _mIsLoaded = true;
  parseProgramInfo();
  Log.print<Severity::info>("Shader Program successfully loaded!");
}

void ShaderProgram::initShaderProgram(
  const Shader& vertexShader,
  const Shader& fragmentShader,
  const Shader& geometryShader
)
{
  if (_mIsLoaded) {
    throw std::exception("Shader Program is already loaded!");
  }

  // create program
  _mId = glCreateProgram();
  glAttachShader(_mId, vertexShader.getShaderId());
  glAttachShader(_mId, fragmentShader.getShaderId());
  glAttachShader(_mId, geometryShader.getShaderId());
  glLinkProgram(_mId);

  // detach shaders
  glDetachShader(_mId, vertexShader.getShaderId());
  glDetachShader(_mId, fragmentShader.getShaderId());
  glDetachShader(_mId, geometryShader.getShaderId());

  // check for linking error
  try {
    checkProgramLinkingError(_mId);
  }
  catch (std::exception msg) {
    _mId = 0;
    throw msg;
  }

  _mIsLoaded = true;
  parseProgramInfo();
  Log.print<Severity::info>("Shader Program successfully loaded!");
}

void ShaderProgram::deleteShaderProgram()
{
  if (_mIsLoaded)
  {
    glDeleteProgram(_mId);
    Log.print<Severity::info>("Shader Program successfully deleted!");

    for (auto it : _mUniforms)
    {
      delete it;
    }
    _mUniforms.clear();
    _mUniformMap.clear();

    _mId = 0;
    _mIsLoaded = false;
  }
  else 
  {
    Log.print<Severity::warning>("Trying to delete Shader Program before load");
  }
}

unsigned int ShaderProgram::getShaderProgramId() const
{
  return _mId;
}

Uniform* ShaderProgram::getUniformByName(const std::string& name) const
{
  // Check the cache
  auto it = _mUniformMap.find(name);
  if (it != _mUniformMap.end())
  {
    // Already in cache, return it
    return it->second;
  }
  else return nullptr;
}

Uniform* ShaderProgram::getUniformByIndex(unsigned int index) const
{
  if (index < _mUniforms.size())
  {
    return _mUniforms[index];
  }
  else return nullptr;
}

bool ShaderProgram::isLoaded() const
{
  return _mIsLoaded;
}

void ShaderProgram::use() const
{
  glUseProgram(_mId);
}

// Shader Program Manager
ShaderProgramManager::ShaderProgramManager(ShaderManager& shaderManager)
  : _mShaderManager(shaderManager), _mProgramInUse(nullptr)
{}

ShaderProgram* const ShaderProgramManager::create(const std::string& key, const ShaderProgramData& data)
{
  ShaderProgram* program = nullptr;

  auto vs = _mShaderManager.find(data.vertexShaderKey);
  auto fs = _mShaderManager.find(data.fragmentShaderKey);

  if (!vs)
  {
    Log.print<Severity::error>("Failed to retrieve Vertex Shader: ", data.vertexShaderKey);
    throw std::runtime_error("Unable to find Vertex Shader: " + data.vertexShaderKey);
  }

  if (!fs)
  {
    Log.print<Severity::error>("Failed to retrieve Fragment Shader: ", data.fragmentShaderKey);
    throw std::runtime_error("Unable to find Fragment Shader: " + data.fragmentShaderKey);
  }

  if (data.hasGeometryShader()) 
  {
    auto gs = _mShaderManager.find(data.geometryShaderKey);

    if (!gs)
    {
      Log.print<Severity::error>("Failed to retrieve Geometry Shader: ", data.geometryShaderKey);
      throw std::runtime_error("Unable to find Geometry Shader: " + data.geometryShaderKey);
    }

    program = new ShaderProgram();
    program->initShaderProgram(*vs, *fs, *gs);
  }
  else 
  {
    program = new ShaderProgram();
    program->initShaderProgram(*vs, *fs);
  }

  return program;
}

void ShaderProgramManager::destroy(ShaderProgram* const value)
{
  delete value;
}

void ShaderProgramManager::useProgram(ShaderProgram* program)
{
  if (program == _mProgramInUse) return;
  if (!program->isLoaded())
  {
    Log.print<Severity::warning>("Trying to use shader program before linking!");
    return;
  }

  program->use();
  _mProgramInUse = program;
}

ShaderProgram* ShaderProgramManager::getProgramInUse() const
{
  return _mProgramInUse;
}
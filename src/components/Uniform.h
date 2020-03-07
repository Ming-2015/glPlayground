#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <string>
#include <vector>
#include <cstring>
#include "../utils/Logger.h"

class Uniform
{
protected:
  int _mProgramId;
  int _mIndex;
  int _mLocation;
  GLenum _mType;
  GLsizei _mArraySize;
  std::string _mName;
  std::vector<unsigned char> _mDataBuffer;

  // convert data to a printable hex string
  std::string _dataToHex() const
  {
    std::stringstream ss2;
    for (auto data : _mDataBuffer)
    {
      ss2 << std::hex << (int)data;
      ss2 << " ";
    }
    return ss2.str();
  }

  // write out the data to some data type
  template<typename T>
  void _dataToValue(T* output, unsigned int index = 0) const
  {
    unsigned int dataEnd = sizeof(T) * (index + 1);
    if (dataEnd > _mDataBuffer.size()) {
      Log.print<Severity::error>("Trying to read uniform data from bad location");
      throw std::out_of_range("Bad opengl uniform memory write");
    }

    unsigned int valSize = sizeof(T);
    unsigned int startByte = index * valSize;
    const unsigned char* src = _mDataBuffer.data() + startByte;

    unsigned char* casted = reinterpret_cast<unsigned char*>(output);
    std::memcpy(casted, src, valSize);
  }

  // saving a raw data: data should be checked beforehand!
  // returns true if the data needs to be changed
  template<typename T>
  bool _registerData(T f, unsigned int index = 0)
  {
    if (index >= _mArraySize)
    {
      Log.print<Severity::error>("Trying to write uniform data in bad location");
      throw std::out_of_range("Bad opengl uniform memory write");
    }

    unsigned int valSize = sizeof(T);
    unsigned int endData = (index + 1) * valSize;
    if (_mDataBuffer.size() < endData)
    {
      _mDataBuffer.resize(endData, unsigned char(0));
    }
    
    // get the pointers to memory location
    unsigned int startByte = index * valSize;
    unsigned char* dest = _mDataBuffer.data() + startByte;
    T* src = &f;

    // check if memory needs to be updated!
    int cmpResult = std::memcmp(dest, src, valSize);
    if (cmpResult == 0) {
      return false;
    }

    // copy memory directly!
    std::memcpy(dest, src, valSize);

    return true;
  }

public:
  Uniform(int programId, int index, GLenum type, GLenum size, const std::string& name);
  virtual ~Uniform();

  // uniform location
  int getLocation() const { return _mLocation; }

  // shader program id
  int getProgramId() const { return _mProgramId; }

  // data type of the uniform (float, vec2, vec3, etc)
  GLenum getType() const { return _mType; }

  // size of the array if this uniform is an array
  GLsizei getSize() const{ return _mArraySize; }

  // name of the uniform
  const std::string& getName() const{ return _mName; }

  // write a float into the uniform location
  void setUniform(float f, unsigned int index = 0);

  // write a int into the uniform location
  void setUniform(int i, unsigned int index = 0);

  // write a vec2 into the uniform location
  void setUniform(glm::vec2 vector, unsigned int index = 0);

  // write a vec3 into the uniform location
  void setUniform(glm::vec3 vector, unsigned int index = 0);

  // write a vec4 into the uniform location
  void setUniform(glm::vec4 vector, unsigned int index = 0);

  // write a mat4 into the uniform location
  void setUniform(glm::mat4 mat, unsigned int index = 0);

  // write a mat3 into the uniform location
  void setUniform(glm::mat3 mat, unsigned int index = 0);

  // write a mat2 into the uniform location
  void setUniform(glm::mat2 mat, unsigned int index = 0);

  // Return the data that the user has set.
  // NOTE: this will not check for type of data, 
  //   and will thus return the raw data filled in the data type provided
  // 
  // Note: Returns the default value of the type of the data is not yet set 
  //   as opposed to the default 0 in shader
  template<typename T>
  T getData(int index = 0) const {
    T ret;
    _dataToValue(&ret, index);
    return ret;
  }
};
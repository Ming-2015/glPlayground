#pragma once
#include <vector>
#include <glm/glm.hpp>

class VertexBoneData {
private:
  std::vector<unsigned int> boneIds;
  std::vector<float> weights;

public:
  static const int MAX_BONES_PER_VERTEX = 4;

  void addBoneData(unsigned int boneId, float weight);
  void normalizeBoneWeights();

  glm::uvec4 getBoneIds();
  glm::vec4 getWeights();
};
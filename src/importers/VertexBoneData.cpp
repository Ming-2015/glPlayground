#include "VertexBoneData.h"
#include "../utils/Logger.h"

void VertexBoneData::addBoneData(unsigned int boneId, float weight)
{
  if (boneIds.size() < MAX_BONES_PER_VERTEX) 
  {
    boneIds.push_back(boneId);
    weights.push_back(weight);
  }
  else
  {
    Log.print<Severity::warning>("Replacing bone with lowest weight for vertex with more than 4 bones");

    // find the bone with the smallest weight
    int minIndex = 0;
    float minWeight = weights[0];
    for (int i = 1; i < MAX_BONES_PER_VERTEX; i++)
    {
      if (weights[i] < minWeight)
      {
        minIndex = i;
        minWeight = weights[i];
      }
    }

    // replace with new bone if the new bone has greater weight
    if (weight > minWeight)
    {
      boneIds[minIndex] = boneId;
      weights[minIndex] = weight;
    }
  }
}

void VertexBoneData::normalizeBoneWeights()
{
  float totalWeight = 0;
  for (int i = 0; i < weights.size(); i++)
  {
    totalWeight += weights[i];
  }

  if (totalWeight == 0) return;

  for (int i = 0; i < weights.size(); i++)
  {
    weights[i] /= totalWeight;
  }
}

glm::uvec4 VertexBoneData::getBoneIds()
{
  glm::uvec4 ret(0);
  for (int i = 0; i < boneIds.size(); i++)
  {
    ret[i] = boneIds[i];
  }
  return ret;
}

glm::vec4 VertexBoneData::getWeights()
{
  glm::vec4 ret(0);
  for (int i = 0; i < weights.size(); i++)
  {
    ret[i] = weights[i];
  }

  // If no bone is available...
  if (weights.size() == 0) 
  {
    Log.print<Severity::warning>("there's no bone in this vertex!");
    ret[0] = 1.f;
  }

  return ret;
}
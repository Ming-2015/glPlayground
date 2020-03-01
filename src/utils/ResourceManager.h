#pragma once
#include <map>

// Intended for avoiding duplicate resource allocation
// K must be a struct or class that contains all info needed to create V
// K must implement operator< and operator==
template<typename K, typename V>
class ResourceManager
{
protected:

  // a resource storage map
  std::map<K, V> resources;
public:

  // get the resource, or create it if not found
  virtual V& getOrCreate(const K& key) = 0;
  virtual void remove(const K& key) = 0;
  virtual void removeAll() = 0;

  ResourceManager() : resources() {}
  virtual ~ResourceManager() {}
};
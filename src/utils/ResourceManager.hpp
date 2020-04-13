#pragma once
#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <type_traits>

// Intended for avoiding duplicate resource allocation. 
// K must be a struct or class that contains all info needed to create V. 
// K must implement operator< and operator==. 
// Main difference from std::map is that it requires a 
//   pointer key to an object that inherits from ResourceInfo and
//   will automatically deallocate the heap resource when done. 
// Key and Value will be exactly one-to-one mapping
// Use Public Inheritance!
template<typename Key, typename Data, typename Resource>
class ResourceManager
{
protected:
  // a resource storage map
  std::map<Key, Resource*> resources;
  std::mutex resourceMutex;

  // define how the resource V is created from key K
  virtual Resource* const create(const Key& key, const Data& data) = 0;

  // define how the resource V is destroyed
  virtual void destroy(Resource* const value) = 0;

public:

  // get the resource, or create it if not found
  virtual Resource* const insert(const Key& key, const Data& data)
  {
    resourceMutex.lock();
    auto it = resources.find(key);
    bool found = it != resources.end();

    if (found)
    {
      resourceMutex.unlock();
      return it->second;
    }
    else {
      try 
      {
        Resource* const resource = create(key, data);
        auto it = resources.insert({ key, resource });
        resourceMutex.unlock();
        return resource;
      }
      catch (std::exception e) 
      {
        Log.print<Severity::error>("Failed to create resource: ", key);
        resourceMutex.unlock();
        throw e;
      }
    }
  }

  // find the resource without creating it
  virtual Resource* const find(const Key& key)
  {
    resourceMutex.lock();
    auto it = resources.find(key);
    bool found = it != resources.end();

    resourceMutex.unlock();
    if (!found) return nullptr;
    else return it->second;
  }

  // remove one resource
  virtual bool erase(const Key& key)
  {
    resourceMutex.lock();
    auto it = resources.find(key);
    bool found = it != resources.end();

    if (found)
    {
      try 
      {
        destroy(it->second);
        resources.erase(key);
        resourceMutex.unlock();
        return true;
      }
      catch (std::exception e)
      {
        Log.print<Severity::error>("Failed to destroy resource: ", key);
        resourceMutex.unlock();
        throw e;
      }
    }
    else {
      resourceMutex.unlock();
      return false;
    }
  }

  // remove all resources
  virtual void clear() 
  {
    resourceMutex.lock();
    
    bool thrown = false;
    std::exception err;
    for (auto it : resources)
    {
      try
      {
        destroy(it.second);
      }
      catch (std::exception e)
      {
        thrown = true;
        err = e;
        Log.print<Severity::error>(
          "Failed to clear resources: ", 
          it.first
        );
      }
    }

    resources.clear();
    resourceMutex.unlock();

    if (thrown)
    {
      throw err;
    }
  }

  const std::map<Key, Resource*>& getAllResources() {
    return resources; 
  }

  // constructor
  ResourceManager() : resources() { }
};
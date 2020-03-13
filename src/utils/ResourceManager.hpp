#pragma once
#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <type_traits>

// Information that can be used to initialize some kind of resource
// This should be extended for any resource class that extends ResourceManager
template<typename Derived>
class ResourceInfo
{
public:
  virtual bool operator< (const Derived& other) const = 0;
  virtual bool operator== (const Derived& other) const = 0;
  virtual const std::string toString() const = 0;
  virtual bool isValidForCreation() const = 0;
};

// Intended for avoiding duplicate resource allocation. 
// K must be a struct or class that contains all info needed to create V. 
// K must implement operator< and operator==. 
// Main difference from std::map is that it requires a 
//   pointer key to an object that inherits from ResourceInfo and
//   will automatically deallocate the heap resource when done. 
// Key and Value will be exactly one-to-one mapping
// Use Public Inheritance!
template<typename K, typename V>
class ResourceManager
{
  // ensure inheritance
  static_assert(std::is_base_of<ResourceInfo<K>, K>::value, "K must be inherit ResourceInfo");

protected:
  // a resource storage map
  std::map<K, V*> resources;
  std::mutex resourceMutex;

  // define how the resource V is created from key K
  virtual V* const create(const K& key) = 0;

  // define how the resource V is destroyed
  virtual void destroy(V* const value) = 0;

public:

  // get the resource, or create it if not found
  virtual V* const getOrCreate(const K& key)
  {
    resourceMutex.lock();
    auto it = resources.find(key);
    bool found = it != resources.end();

    if (found)
    {
      resourceMutex.unlock();
      return it->second;
    }
    else if (!key.isValidForCreation())
    {
      return nullptr;
    }
    else {
      try 
      {
        V* const resource = create(key);
        auto it = resources.insert({ key, resource });
        resourceMutex.unlock();
        return resource;
      }
      catch (std::exception e) 
      {
        Log.print<Severity::error>("Failed to create resource: ", key.toString());
        resourceMutex.unlock();
        throw e;
      }
    }
  }

  // find the resource without creating it
  virtual V* const find(const K& key)
  {
    resourceMutex.lock();
    auto it = resources.find(key);
    bool found = it != resources.end();

    resourceMutex.unlock();
    if (!found) return nullptr;
    else return it->second;
  }

  // remove one resource
  virtual bool erase(const K& key)
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
        Log.print<Severity::error>("Failed to destroy resource: ", key.toString());
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
          it.first.toString()
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

  // constructor
  ResourceManager() : resources() { }
};
#ifndef HashExtras_h__
#define HashExtras_h__

#include "utils/hashmap.h"
#include <string>
using namespace std;

// Cannot specialize hash template from outside of the std namespace.
namespace HASH_NAMESPACE {

// Provide a hash function for arbitrary pointers...
template <class T> struct hash<T *> {
  inline size_t operator()(const T *Val) const {
    return reinterpret_cast<size_t>(Val);
  }
};

template <> struct hash<string> {
  size_t operator()(string const &str) const {
    return hash<char const *>()(str.c_str());
  }
};

}  // End namespace std

#endif // HashExtras_h__

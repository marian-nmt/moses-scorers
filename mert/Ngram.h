#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <unordered_map>

namespace MosesTuning {

template <class T>
inline void hash_combine(std::size_t& seed, const T& v) {
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template <typename Container> // we can make this generic for any container [1]
struct ContainerHash {
    std::size_t operator()(Container const& c) const {
      std::size_t hash = 0;
      for(auto& v : c)
        hash_combine(hash, v);
      return hash;
    }
};

/** A simple STL-std::map based n-gram counts. Basically, we provide
 * typical accessors and mutaors, but we intentionally does not allow
 * erasing elements.
 */
class NgramCounts {
public:
  // Used to construct the ngram map
  struct NgramComparator {
    bool operator()(const std::vector<int>& a, const std::vector<int>& b) const {
      std::size_t i;
      const std::size_t as = a.size();
      const std::size_t bs = b.size();
      for(i = 0; i < as && i < bs; ++i) {
        if(a[i] < b[i]) {
          return true;
        }
        if(a[i] > b[i]) {
          return false;
        }
      }
      // entries are equal, shortest wins
      return as < bs;
    }
  };

  typedef std::vector<int> Key;
  typedef int Value;
  typedef std::unordered_map<Key, Value, ContainerHash<Key>>::iterator iterator;
  typedef std::unordered_map<Key, Value, ContainerHash<Key>>::const_iterator const_iterator;

  NgramCounts() : kDefaultCount(1) {}
  virtual ~NgramCounts() {}

  /**
   * If the specified "ngram" is found, we add counts.
   * If not, we insert the default count in the container. */
  inline void Add(const Key& ngram) { m_counts[ngram]++; }

  /**
   * Adds "ngram" with given counts.
   */
  inline void Add(const Key& ngram, Value v) { m_counts[ngram] += v; }

  /**
   * Return true iff the specified "ngram" is found in the container.
   */
  bool Lookup(const Key& ngram, Value* v) const {
    const_iterator it = m_counts.find(ngram);
    if(it == m_counts.end())
      return false;
    *v = it->second;
    return true;
  }

  /**
   * Clear all elments in the container.
   */
  void clear() { m_counts.clear(); }

  /**
   * Return true iff the container is empty.
   */
  bool empty() const { return m_counts.empty(); }

  /**
   * Return the the number of elements in the container.
   */
  std::size_t size() const { return m_counts.size(); }

  std::size_t max_size() const { return m_counts.max_size(); }

  // Note: This is mainly used by unit tests.
  int get_default_count() const { return kDefaultCount; }

  iterator find(const Key& ngram) { return m_counts.find(ngram); }
  const_iterator find(const Key& ngram) const { return m_counts.find(ngram); }

  Value& operator[](const Key& ngram) { return m_counts[ngram]; }

  iterator begin() { return m_counts.begin(); }
  const_iterator begin() const { return m_counts.begin(); }
  iterator end() { return m_counts.end(); }
  const_iterator end() const { return m_counts.end(); }

private:
  const int kDefaultCount;
  std::unordered_map<Key, Value, ContainerHash<Key>> m_counts;
};

}  // namespace MosesTuning

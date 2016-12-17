#pragma once

#include <vector>
#include <algorithm>
#include "searcher.h"

template<typename T>
class StlSearcher: public Searcher<T> {
 public:
  StlSearcher(const std::vector<T>& vec):vec_(vec) {}

  bool search(const T& elem) {
    auto it = std::lower_bound(vec_.begin(), vec_.end(), elem);
    return (it != vec_.end() && *it == elem);
  }
 private:
  std::vector<T> vec_;
};

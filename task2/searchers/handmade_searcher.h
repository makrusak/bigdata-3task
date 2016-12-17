#pragma once

#include <vector>
#include <algorithm>
#include "searcher.h"

template<typename T>
class HandmadeSearcher: public Searcher<T> {
 public:
  HandmadeSearcher(const std::vector<T>& vec): vec_(vec) {}

  bool search(const T& elem) {
    if (vec_.empty()) {
      return false;
    }
    int l = -1, r = vec_.size(), m;
    while (r - l > 1) {
      m = (l + r) / 2;
      if (vec_[m] >= elem) {
        r = m;
      } else {
        l = m;
      }
    }
    return (r != vec_.size() && vec_[r] == elem);
  }
 private:
  std::vector<T> vec_;
};

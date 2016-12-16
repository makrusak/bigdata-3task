#pragma once

#include <vector>
#include <algorithm>
#include "searcher.h"

template<typename T>
class SqrtSearcher: public Searcher<T> {
  public:
    SqrtSearcher(const std::vector<T>& vec):vec_(vec) {
      sqrt_ = (int)(sqrt(vec_.size()));
      if (sqrt_ >= 1) {
        for (int pos = sqrt_; pos<vec_.size(); pos+=sqrt_) {
          vec_sqrt_.push_back(vec_[pos]);
        }
      }
    }

    bool search(const T& elem) {
      if (vec_sqrt_.empty() || vec_.empty()) {
        return false;
      }

      auto it_sqrt = std::lower_bound(vec_sqrt_.begin(), vec_sqrt_.end(), elem);
      if (*it_sqrt == elem) {
        return true;
      }
      if (it_sqrt == vec_sqrt_.end()) {
        return search_vec(vec_sqrt_.size()*sqrt_, vec_.size(), elem);
      } else {
        int pos = it_sqrt - vec_sqrt_.begin();
        return search_vec(pos*sqrt_, (pos+1)*sqrt_, elem);
      }
    }
  private:
    bool search_vec(int pos_begin, int pos_end, const T& elem) {
      auto it = std::lower_bound(vec_.begin()+pos_begin, vec_.begin()+pos_end, elem);
      return (it != vec_.end() && *it == elem);
    }

    int sqrt_;
    std::vector<T> vec_sqrt_;
    std::vector<T> vec_;
};

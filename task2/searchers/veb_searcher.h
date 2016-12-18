#pragma once

#include <vector>
#include <algorithm>
#include "searcher.h"
#include <iostream>

template<typename T, T MAX_VAL>
class VebSearcher: public Searcher<T> {
 public:
  VebSearcher(const std::vector<T>& vec) {
    if (vec.size() == 0) {
      return;
    }
    int from_size = (int)vec.size();
    int height = tree_height(from_size);

    vec_.resize(1<<height);
    for (int i=0; i<from_size; i++) {
      int pos = pos_from_sorted_pos(i+1, height);
      vec_[pos] = vec[i];
    }
    //Dirty trick: fill array with infs up to vec_ size
    for (int i=0;i<(int)vec_.size()-1-vec.size();i++) {
      int pos = pos_from_sorted_pos(from_size+i+1, height);
      vec_[pos] = MAX_VAL;
    }
  }

  bool search(const T& elem) {
    if (vec_.size() == 0) {
      return false;
    }
    int res = veb_search(1, vec_.size()-1, elem);
    return res == -1;
  }
 private:
  int veb_search(int current_pos, int current_tree_size, const T& elt) {
    int height = tree_height(current_tree_size);
    int bottom_height = height/2;
    int top_height = height - bottom_height;
    int top_size = tree_size(top_height);
    int bottom_size = tree_size(bottom_height);
    int leaf_count = 1<<bottom_height;

    if (height > 1) {
      int top_index = veb_search(current_pos, top_size, elt);
      if (top_index < 0) return top_index;

      int offset = top_index * bottom_size + top_size;

      int bottom_index = veb_search(current_pos + offset, bottom_size, elt);
      if (bottom_index < 0) return bottom_index;
      return leaf_count*top_index + bottom_index;
    } else {
      const T& root = vec_[current_pos];
      if (elt == root) {
        return -1;
      }

      return (elt < root) ? 0 : 1;
    }
  }

  int pos_from_sorted_pos(int pos, int height) {
    if (height <= 1) return pos;

    int bottom_half = height / 2;
    int top_half = height - bottom_half;

    int top_n = pos >> bottom_half;
    int bottom_n = pos & ((1<<bottom_half) - 1);

    if (bottom_n == 0) return pos_from_sorted_pos(top_n, top_half);

    int top_address = top_n * tree_size(bottom_half) + tree_size(top_half);
    int bot_address = pos_from_sorted_pos(bottom_n, bottom_half);

    return top_address + bot_address;
  }

  inline int tree_size(int height) {
    return (1<<height)-1;
  }

  inline int tree_height(int n) {
    return (int)ceil(log2((float)n+1));
  }

  std::vector<T> vec_;
};

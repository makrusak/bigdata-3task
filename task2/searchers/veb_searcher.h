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
//      std::cout << i+1 << " to " << pos << "\n";
    }
    //Dirty trick: fill array with infs up to vec_ size
    for (int i=0;i<(int)vec_.size()-1-vec.size();i++) {
      int pos = pos_from_sorted_pos(from_size+i+1, height);
      vec_[pos] = MAX_VAL;
    }
    /*
        std::cout << "Resulted struct: " << "\n";
        std::cout << height << "\n";
        for (const T&c : vec_) {
          std::cout << c << " ";
        }
        std::cout << "\n";
        */
  }
/*
  bool search(const T& elem) {
    bool real_ans = false;
    for (const T& el: vec_) {
      real_ans |= (el == elem);
    }
    if (!real_ans) {
      return real_ans;
    }
    std::cout << "Trying to find: " << elem << "\n";
    bool ans = search_bad(elem);
    if (real_ans != ans) {
      exit(0);
    }
    return ans;
  } */

  bool search(const T& elem) {
    if (vec_.size() == 0) {
      return false;
    }
    int res = veb_search(1, vec_.size()-1, elem);
    return res == -1;
  }
 private:
  void hyper_compute(int n, int& d, int& D, int& subtree_size, int& subtree_leaf_count) {
    int h = tree_height(n);
    int bottom_height = h / 2;
    d = h - bottom_height;
    D = tree_size(d);
    subtree_size = tree_size(bottom_height);
    subtree_leaf_count = 1<<bottom_height;
  }

  int veb_search(int current_pos, int length, const T& elt) {
//    std::cout << "  Current_pos: " << current_pos << " In array of size: " << length << "\n";
    int d, D, subtree_size, subtree_leaf_count;
    hyper_compute(length, d, D, subtree_size, subtree_leaf_count);

 //   std::cout << "    top_h: " << d << " top_size: " << D << " subtree_size: " << subtree_size << " subtree_leaf_count: " << subtree_leaf_count << "\n";

    if (length > 1) {
      // Recurse on top half of tree
 //     std::cout << "    TOP tree\n";
      int subtree_index = veb_search(current_pos, D, elt);
      if (subtree_index < 0) return subtree_index;

      int offset = subtree_index * subtree_size + D;

  //    std::cout << "    BOTTOM tree with offset(" << subtree_index << " " << subtree_size << " " << D << ") " << offset << "\n";
      // If not in top half, use subtree index to find place in bottom half
      int bottom_subtree_index = veb_search(current_pos + offset, subtree_size, elt);
      if (bottom_subtree_index == -1) {
        return -1;
      }
      int ret_val = subtree_leaf_count*subtree_index + bottom_subtree_index;
//      std::cout << "Returned value for " << current_pos << " = " << ret_val << "( " << subtree_leaf_count << " " << subtree_index << " " << bottom_subtree_index << ")\n";
      return ret_val;
    } else {
      const T& root = vec_[current_pos];
//      std::cout << "Comparation: " << root << "\n";
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

  int tree_size(int height) {
    return (1<<height)-1;
  }

  int tree_height(int n) {
    return (int)ceil(log2((float)n+1));
  }

  std::vector<T> vec_;
};

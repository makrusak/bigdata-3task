#pragma once

template<typename T>
class Searcher {
  public:
    virtual bool search(const T& elem) = 0;
};

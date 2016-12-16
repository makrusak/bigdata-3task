/*
An implementation of the cache-oblivious implicit van Emde Boas binary
tree data structure.
Thanks to Jeff Erickson for suggesting the problem, and to John
Fischer and Yasutaka Furukawa for pointing out how to encode an
element's position using its bitstring as a traversal code.
Leslie Wu
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>

#include "searchers/veb_searcher.h"

int main()
{	
  std::vector<long long> vec = {1, 5, 8, 9, 10, 11, 12, 13, 23, 28, 31, 35, 38, 39, 41};
  auto *s = new VebSearcher<long long>(vec);
  for (;;) {
    int q; std::cin >> q;
    std::cout << s->search(q) << "\n";
  }
	return 0;
}

#include <iostream>
#include <vector>
#include <climits>
#include <sys/time.h>
#include <cassert>
#include <memory>
#include <cstdio>
#include <cstring>
#include <chrono>

#include "searchers/stl_searcher.h"
#include "searchers/sqrt_searcher.h"
#include "searchers/handmade_searcher.h"
#include "searchers/prefetch_searcher.h"
#include "searchers/veb_searcher.h"

typedef std::unique_ptr<Searcher<long long> > searcher_ptr;

std::vector<long long> generate_rand_vector_sorted(int n, long long max_num) {
  std::vector<long long> vec(n);
  for (int i=0; i<n; i++) {
    vec[i] = rand()%max_num;
  }
  std::sort(vec.begin(), vec.end());
  return vec;
}

std::vector<long long> generate_rand_queries(const std::vector<long long>& from, int n, long long max_num) {
  std::vector<long long> vec(n);
  for (int i=0; i<n; i++) {
    if (rand()%2 && from.size() != 0) {
      vec[i] = from[rand()%from.size()];
    } else {
      vec[i] = rand()%max_num;
    }
  }
  return vec;
}

struct bench_res {
  float calc_time_ns;
  std::vector<char> answers;
};

bench_res benchmark(searcher_ptr searcher, std::vector<long long> queries) {
  bench_res br;
  br.answers = std::vector<char>(queries.size());

  //struct timespec start, end;
  //clock_gettime(CLOCK_REALTIME,&start);
  auto begin = std::chrono::high_resolution_clock::now();

  for (int i=0;i<queries.size();i++) {
    br.answers[i] = searcher->search(queries[i]);
  }

  auto end = std::chrono::high_resolution_clock::now();
  //clock_gettime(CLOCK_REALTIME,&end);

  //br.calc_time_ns = end.tv_nsec - start.tv_nsec;
  br.calc_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
  return br;
}

std::vector<std::pair<std::string, bench_res> > results_all_algo(const std::vector<long long>& vec, const std::vector<long long>& queries) {
  std::vector<std::pair<std::string, bench_res> > results;
  results.push_back(std::make_pair("Handmade Searcher", benchmark(\
          searcher_ptr(new HandmadeSearcher<long long>(vec)), queries)));
  results.push_back(std::make_pair("Stl Searcher", benchmark(\
          searcher_ptr(new StlSearcher<long long>(vec)), queries)));
  results.push_back(std::make_pair("Prefetch Searcher", benchmark(\
          searcher_ptr(new PrefetchSearcher<long long>(vec)), queries)));
  results.push_back(std::make_pair("Sqrt Searcher", benchmark(\
          searcher_ptr(new SqrtSearcher<long long>(vec)), queries)));
  results.push_back(std::make_pair("VEB Searcher", benchmark(\
          searcher_ptr(new VebSearcher<long long>(vec)), queries)));
  return results;
}

void stress_test() {
  std::vector<std::pair<int, int> > tests = { \
//    std::make_pair(0, 5), \
//      std::make_pair(15, 300000), 
      std::make_pair(65535, 60000), \
      //std::make_pair(3e6, 20000), \
      //std::make_pair(3e7, 20000)
  };

  for (const auto& test : tests) {
    auto vec = generate_rand_vector_sorted(test.first, (long long)(1e12));
    auto queries = generate_rand_queries(vec, test.second, (long long)(1e12));

    auto results = results_all_algo(vec, queries);
    std::cout << "Time. Vec size: " << test.first << ". Queries: " << test.second << "\n";

    for (const auto& res : results) {
      std::cout << res.first << ". Time per op: " << res.second.calc_time_ns / queries.size() << "ns\n";
    }

    //check correctness
    for (int i=0;i<queries.size();i++) {
      for (const auto& res : results) {
        assert(results[0].second.answers[i] == res.second.answers[i]);
      }
    }
    std::cout << "Correct\n\n";
  }
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  if (argc == 2 && strcmp(argv[1], "-stress")==0) {
    stress_test();
    return 0;
  }
  /* Test:
     4
     1 2 5 8
     8
     1 5 3 8 2 4 -1 100
     */

  int n;
  std::cin >> n;
  std::vector<long long> vec(n);
  for (int i=0;i<n;i++) {
    std::cin >> vec[i];
  }

  int m;
  std::cin >> m;
  std::vector<long long> queries(m);
  for (int i=0;i<m;i++) {
    std::cin >> queries[i];
  }

  auto results = results_all_algo(vec, queries);

  for (int i=0;i<queries.size();i++) {
    for (const auto& res : results) {
      assert(results[0].second.answers[i] == res.second.answers[i]);
    }
  }
  std::cerr << "Correct\n";

  for (const auto& res: results) {
    std::cerr << res.first << ": \n";
    std::cerr << "  Calculation time: " << res.second.calc_time_ns << "ns\n";
    std::cerr << "\n";
  }

  for (int a : results[0].second.answers) {
    std::cout << a << " ";
  }
  std::cout << "\n";
  return 0;
}

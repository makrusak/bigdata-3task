#include <iostream>
#include <vector>
#include <climits>
#include <sys/time.h>
#include <sstream>
#include <algorithm>

std::vector<int> generate_rand_vector(int n, int max_num) {
  std::vector<int> vec(n);
  for (int i=0; i<n; i++) {
    vec[i] = rand()%max_num;
  }
  return vec;
}

float ts_diff_ns_op(const timespec &a, const timespec &b, int op) {
  return float(b.tv_nsec - a.tv_nsec)/op;
}

float random_access(const std::vector<int> &vec, int op_count) {
  int sz = vec.size();
  int val, i;

  auto sequence = generate_rand_vector(op_count, sz);

  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME,&start);

  for (int pos : sequence) {
    __builtin_prefetch(&vec[pos]);
  }
  clock_gettime(CLOCK_REALTIME,&end);
  return ts_diff_ns_op(start, end, op_count);
}

std::string nice_memory_int(int bytes) {
  std::ostringstream buf;
  if (bytes >= (int)(1e6)) {
    int count = bytes / (int)(1e6);
    bytes = bytes - count*(int)(1e6);
    buf << count << "MB";
  }

  if (bytes >= (int)(1e3)) {
    int count = bytes / (int)(1e3);
    bytes = bytes - count*(int)(1e3);
    if (buf.rdbuf()->in_avail() != 0) {
      buf << " ";
    }
    buf << count << "KB";
  }
  if (bytes != 0) {
    if (buf.rdbuf()->in_avail() != 0) {
      buf << " ";
    }
    buf << bytes << "B";
  }

  return buf.str();
}

void benchmark(int bytes) {
  auto vec = generate_rand_vector(bytes/4, INT_MAX);

  int op_count = 1000000;
  auto time = random_access(vec, op_count);
  std::cout << "Memory size: " << nice_memory_int(bytes) << "; Time: " << time << "\n";
}

int main() {
  std::cout << "Hello\n";
  std::vector<int> bench_consts_kb = {0, 32, 256, 3072, (int)(3e5)};
  std::vector<int> all_costs;
  for (int i = 1;i<bench_consts_kb.size();i++) {
    int prev_bytes = bench_consts_kb[i-1]*1000;
    int bytes = bench_consts_kb[i]*1000;
    int diff = bytes - prev_bytes;
    all_costs.push_back(prev_bytes + diff/3);
    all_costs.push_back(prev_bytes + diff*2/3);
    all_costs.push_back(prev_bytes + diff);
    all_costs.push_back(prev_bytes + 2*diff);
    all_costs.push_back(prev_bytes + 3*diff);
    all_costs.push_back(prev_bytes + 5*diff);
  }
  std::sort(all_costs.begin(), all_costs.end());
  for (int c: all_costs) {
    benchmark(c);
  }
  return 0;
}

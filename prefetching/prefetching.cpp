// This program shows how the prefetching impacts performance in C++
// By: Nick from CoffeeBeforeArch

#include <benchmark/benchmark.h>
#include <cstdlib>
#include <numeric>
#include <random>
#include <vector>

// Accesses an array sequentially in row-major fashion
static void rowMajor(benchmark::State &s) {
  // Input/output vector size
  int N = 1 << s.range(0);

  // Create our input indices
  std::vector<int> v_in(N);
  std::iota(begin(v_in), end(v_in), 0);

  // Create an output vector
  std::vector<int> v_out;
  v_out.resize(N);

  // Profile a simple traversal with simple additions
  while (s.KeepRunning()) {
    for (int i = 0; i < N; i++) {
      v_out[v_in[i]] += i;
    }
  }
}
// Register the benchmark
BENCHMARK(rowMajor)->DenseRange(20, 24, 2)->Unit(benchmark::kMillisecond);

// Accesses an array sequentially in row-major fashion
static void columnMajor(benchmark::State &s) {
  // Input/output vector size
  int N = 1 << s.range(0);

  // Create our input indices
  int stride = 1 << (N / 2);
  std::vector<int> v_in(N);
  for(int i = 0; i < stride; i++ ) {
    for(int j = 0; j < stride; j++) {
      v_in[i * stride + j] = j * stride + i;
    }
  }

  // Create an output vector
  std::vector<int> v_out;
  v_out.resize(N);

  // Profile a simple traversal with simple additions
  while (s.KeepRunning()) {
    for (int i = 0; i < N; i++) {
      v_out[v_in[i]] += i;
    }
  }
}
// Register the benchmark
BENCHMARK(columnMajor)->DenseRange(20, 24, 2)->Unit(benchmark::kMillisecond);

// Accesses an array sequentially in row-major fashion
static void random(benchmark::State &s) {
  // Input/output vector size
  int N = 1 << s.range(0);

  // Create our input indices
  std::vector<int> v_in(N);
  std::iota(begin(v_in), end(v_in), 0);

  // Now shuffle the vector
  std::random_device rng;
  std::mt19937 urng(rng());
  std::shuffle(begin(v_in), end(v_in), urng);

  // Create an output vector
  std::vector<int> v_out;
  v_out.resize(N);

  // Profile a simple traversal with simple additions
  while (s.KeepRunning()) {
    for (int i = 0; i < N; i++) {
      v_out[v_in[i]] += i;
    }
  }
}
// Register the benchmark
BENCHMARK(random)->DenseRange(20, 24, 2)->Unit(benchmark::kMillisecond);

// Accesses an array sequentially in row-major fashion
static void randomPrefetch(benchmark::State &s) {
  // Input/output vector size
  int N = 1 << s.range(0);

  // Create our input indices
  std::vector<int> v_in(N);
  std::iota(begin(v_in), end(v_in), 0);

  // Now shuffle the vector
  std::random_device rng;
  std::mt19937 urng(rng());
  std::shuffle(begin(v_in), end(v_in), urng);

  // Create an output vector
  std::vector<int> v_out;
  v_out.resize(N);

  // Profile a simple traversal with simple additions
  while (s.KeepRunning()) {
    for (int i = 0; i < N; i++) {
      __builtin_prefetch(&v_out[v_in[i + 1]]);
      v_out[v_in[i]] += i;
    }
  }
}
// Register the benchmark
BENCHMARK(randomPrefetch)->DenseRange(20, 24, 2)->Unit(benchmark::kMillisecond);


// Benchmark main functions
BENCHMARK_MAIN();

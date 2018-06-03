#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dense_ordered_map.hpp"

#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <unordered_map>


namespace {
  struct LongRange {
    long low;
    long high;

    LongRange(long low, long high)
      : low(low), high(high)
    {
    }
  };

  template<class MapType>
  long BenchSearch(MapType const& map, LongRange const& range, unsigned repeats, char const* message)
  {
    std::cout << "benchmarking " << message << "\n";

    long res = 0;

    auto start = std::chrono::high_resolution_clock::now();
    
    for (unsigned repeatIx = 0; repeatIx < repeats; ++repeatIx)
      {
        for (long ix = range.low; ix < range.high; ++ix)
          {
            auto p = map.find(ix);
            if (p != map.cend())
              {
                res += p->second;
              }
          }
      }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;

    std::cout << "benchmarking " << message << " took " << diff.count() << " seconds\n";

    return res;
  }
  
  template<class MapType>
  long BenchSum(MapType const& map, unsigned const repeats, char const* msg)
  {
    std::cout << "benchmarking " << msg << "\n";

    long res = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (unsigned repeatIx = 0; repeatIx < repeats; ++repeatIx)
      {
        for (auto it = map.cbegin(); it != map.cend(); ++it)
          {
            res += it->second;
          }
      }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;

    std::cout << "summing " << msg << " took " << diff.count() << " seconds\n";
    
    return res;
  }
  
  std::map<long, long> RandomMap(std::size_t size, LongRange range, std::size_t seed = 0)
  {
    std::mt19937 generator(seed);
    std::uniform_int_distribution<long> distribution(range.low, range.high);

    std::map<long, long> res;
    
    for (std::size_t ix = 0; ix < size; ++ix)
      {
        long k = distribution(generator);
        long v = distribution(generator);
        res[k] = v;
      }

    return res;
  }    
}


TEST_CASE("DenseOrderedMap", "[DenseOrderedMap]")
{
  SECTION("correctness", "1")
    {
      std::initializer_list<std::pair<long, float>> init
      {{1,1.0}, {2, 2.0}, {3,3.0}, {4,4.0}};
      
      DenseOrderedMap<long, float> dense_m(init);
      std::map<long, float> tree_m(init.begin(), init.end());

      for (long ix = -10; ix < 10; ++ix)
        {
          auto tree_map_it = tree_m.find(ix);
          auto dense_map_it = dense_m.find(ix);

          if (tree_map_it == tree_m.end())
            {
              CHECK(dense_map_it == dense_m.cend());
            }
          else {
            REQUIRE(dense_map_it != dense_m.cend());
            CHECK(tree_map_it->second == dense_map_it->second);
          }
        }
    }

  SECTION("lookup performance 1")
    {
      std::cout << "LOOKUP PERF 1\n";
      
      std::map<long, long> tree_map{{1000,0}, {1010, 1}, {1020, 2}, {1034, 3}, {1047, 4}, {1059, 5}, {1066, 6}, {1077, 7}, {1088, 8}, {1099, 9}}; 
      auto dense_map = DenseOrderedMap<long, long>(tree_map.cbegin(), tree_map.cend());
      std::unordered_map<long, long> unordered_map{tree_map.cbegin(), tree_map.cend()};
      
      auto const rangeToSearch = LongRange(1000, 2000);
      unsigned const repeats = 10000;
      
      long res1 = BenchSearch(tree_map, rangeToSearch, repeats, "tree map");
      long res2 = BenchSearch(dense_map, rangeToSearch, repeats, "dense map");
      long res3 = BenchSearch(unordered_map, rangeToSearch, repeats, "unordered map");

      CHECK(res1 != 0);
      CHECK(res1 == res2);
      CHECK(res2 == res3);
    }
  
  SECTION("lookup performance 2")
    {
      std::cout << "LOOKUP PERF 2\n";
      
      std::map<long, long> tree_map;
      tree_map[1000] = 1;
      tree_map[1001];
      tree_map[1002];
      tree_map[1003];
      tree_map[1004];
      tree_map[1005];
      tree_map[1006];
      tree_map[1007];
      tree_map[1008];
      tree_map[1009];
      auto dense_map = DenseOrderedMap<long, long>(tree_map.cbegin(), tree_map.cend());
      std::unordered_map<long, long> unordered_map{tree_map.cbegin(), tree_map.cend()};
      
      auto const rangeToSearch = LongRange(1000, 1020);
      unsigned const repeats = 1000000;
      
      long res1 = BenchSearch(tree_map, rangeToSearch, repeats, "tree map");
      long res2 = BenchSearch(dense_map, rangeToSearch, repeats, "dense map");
      long res3 = BenchSearch(unordered_map, rangeToSearch, repeats, "unordered map");

      CHECK(res1 != 0);
      CHECK(res1 == res2);
      CHECK(res2 == res3);
    }

  SECTION("iteration perf")
    {
      std::cout << "ITERATION PERF\n";
      
      auto tree_map = RandomMap(100, LongRange(-1000, 1000));
      auto dense_map = DenseOrderedMap<long, long>(tree_map.cbegin(), tree_map.cend());
      std::unordered_map<long, long> unordered_map{tree_map.cbegin(), tree_map.cend()};

      unsigned const repeats = 100000;
      
      long sum1 = BenchSum(tree_map, repeats, "tree map");
      long sum2 = BenchSum(dense_map, repeats, "dense map");
      long sum3 = BenchSum(unordered_map, repeats, "unordered map");

      CHECK(sum1 == sum2);
      CHECK(sum2 == sum3);
    }
}

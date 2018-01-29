#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cstdlib>

#include "sglog.h"
SG_LOG SmaugLog("logs/");

int main(int args, char*[])
{
  using namespace std::chrono;
  using clock = steady_clock;

  //check_dir("a/b/c");

  SDEBUG("test DEBUG{}", 121113);
  SERROR("test ERROR{}{}", 121113,"its error");
  SWARN("test WARN{}", 121113);
  SINFO("test INFO{}", 121113);
  SCR("test CRITICAl{}", 121113);

  int thread_count = 10;
  int howmany = 1000000;

  //std::ios::sync_with_stdio(false);
  std::atomic<int > msg_counter{ 0 };
  std::vector<std::thread> threads;
  auto start = clock::now();

  for (int t = 0; t < thread_count; ++t)
  {
    threads.push_back(std::thread([&]()
    {
      while (true)
      {
        int counter = ++msg_counter;
        if (counter > howmany) break;
        SINFO("spdlog message #{}:  for your pleasure", counter);
        //rotating_mt2->info("Hello logger: msg number {}", counter);
      }
    }));
  }

  for (auto &t : threads)
  {
    t.join();
  };


  /*
  std::thread demo =  std::thread([&]()
  {
    while (true)
    {
      int counter = ++msg_counter;
      if (counter > howmany) break;
      CCC("spdlog message #{}:  for your pleasure", counter);
    }
  });

  demo.join();
  */

  duration<float> delta = clock::now() - start;
  float deltaf = delta.count();
  auto rate = howmany / deltaf;

  std::cout << "Total: "   << howmany      << std::endl;
  std::cout << "Threads: " << thread_count << std::endl;
  std::cout << "Delta = "  << deltaf       << " seconds" << std::endl;
  std::cout << "Rate = "   << rate         << "/sec"     << std::endl;
  return 0;

}

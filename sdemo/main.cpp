#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cstdlib>

#include "m_log.h"
M_LOG_LIB OutLog;

int main(int args, char*[])
{
  using namespace std::chrono;
  using clock = steady_clock;

  //check_dir("a/b/c");
  //OutLog.getLogger()->set_level(spdlog::level::info);
  OutLog.getLogger()->trace("test trace {:6.6f}",12321.345);

  TTT("test TTT {}", 121113);
  DDD("test DDD {}", 121113);
  EEE("test EEE {}", 121113);
  WWW("test WWW {}", 121113);
  III("test III {}", 121113);
  CCC("test CCC {}", 121113);

  int thread_count = 10;
  int howmany = 1000000;
  int file_size = 30 * 1024 * 1024;
  int rotating_files = 5;
  spdlog::set_async_mode(1048576);
  auto rotating_mt2 = spdlog::rotating_logger_mt("rotating_mt2", "logs/rotating_mt2", file_size, rotating_files);

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
        //CCC("spdlog message #{}:  for your pleasure", counter);
        JJJ("spdlog message #{}:  for your pleasure", counter);
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

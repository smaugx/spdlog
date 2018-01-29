#include"s.h"


static bool check_dir(const char* dir)
{
  if (access(dir, 0) == -1)
  {
    int flag = mkdir(dir, 0777);
    return (flag == 0);
  }
  return true;
};


SG_LOG::SG_LOG()
{
  //std::string logdir= SConfig::Instance().get_logpath();
  std::string logdir;
  if(logdir.empty())
  {
    logdir = "logs";
  }
  std::string dir;
  if(logdir[logdir.size()-1] == '/')
  {
    dir = logdir.substr(0,logdir.size()-1);
  }
  else
  {
    dir = logdir;
  }
  check_dir(dir.c_str());
  //设置为异步日志
  spdlog::set_async_mode(1048576);  // 必须为 2 的幂
  std::string info = dir + "/info";
  std::string err = dir + "/err";

  //single file max_size is 30MB
  rotating_logger = spdlog::rotating_logger_mt("rotation_info",info, 30 * 1024 * 1024,5);
  err_logger = spdlog::daily_logger_mt("err_daily_logger",err);
  //critical_logger = spdlog::color_logger_mt("console");
  critical_logger = spdlog::stdout_color_mt("console");

  //register it if you need to access it globally
  /*
  spdlog::register_logger(rotating_logger);
  spdlog::register_logger(err_logger);
  spdlog::register_logger(critical_logger);
  */


  // 设置日志记录级别
  //rotating_file_logger->set_level(spdlog::level::debug);

  //设置 logger 格式
  spdlog::set_pattern("%T.%e [thread %t]  [%L]  %v");
  //设置当出发 err 或更严重的错误时立刻刷新日志到  disk
  err_logger->flush_on(spdlog::level::err);

}


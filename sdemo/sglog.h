#ifndef _H_SG_LOG_
#define _H_SG_LOG_
//static function check dir
#ifndef _CHECK_DIR_
#define _CHECK_DIR_
#endif


#include <unistd.h>
#include <sys/types.h>
#include<cstring>
#include <sys/stat.h>

static bool check_dir(const char* dir)
{
  if (access(dir, 0) == -1)
  {
    int flag = mkdir(dir, 0777);
    return (flag == 0);
  }
  return true;
};



//*****************************************************
//注意：
//文件名 __FILE__ ,函数名 __func__ ，行号__LINE__ 是编译器实现的
//并非C++头文件中定义的
//前两个变量是string类型，且__LINE__是整形，所以需要转为string类型
//******************************************************

//整数类型文件行号 ->转换为string类型  std::to_string 处理
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)

//定义一个在日志后添加 文件名 函数名 行号 的宏定义
#ifndef suffix
#define suffix(msg)  std::string(msg).append("  //")\
        .append(__FILENAME__).append(":").append(__func__)\
        .append("()#").append(std::to_string(__LINE__))\
        .append(".").c_str()
#endif

//在  spdlog.h   之前定义，才有效
#ifndef SPDLOG_TRACE_ON
#define SPDLOG_TRACE_ON
#endif

#ifndef SPDLOG_DEBUG_ON
#define SPDLOG_DEBUG_ON
#endif


//引入 log 头文件
#include <spdlog/spdlog.h>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/file_sinks.h>
#include <spdlog/sinks/null_sink.h>


using namespace spdlog::sinks;

// spdlog 封装

class SG_LOG
{

public:
  SG_LOG(std::string logdir)
  {
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
    rotating_logger->set_level(spdlog::level::debug);

    //设置 logger 格式
    spdlog::set_pattern("%T.%e [thread %t]  [%L]  %v");
    //设置当出发 err 或更严重的错误时立刻刷新日志到  disk
    err_logger->flush_on(spdlog::level::err);

  };

  inline std::shared_ptr<spdlog::logger> getRotatingLogger()
  {
    return rotating_logger;
  }

  inline  std::shared_ptr<spdlog::logger> getErrLogger()
  {
    return err_logger;
  }

  inline  std::shared_ptr<spdlog::logger> getCriticalLogger()
  {
    return critical_logger;
  }
  /*
  void Out(const char * fmt, ...)
  {
    char tmp[2048] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsprintf(tmp, fmt, args);
    va_end(args);
    nml_logger->info(tmp);
  };
  */

public:
  SG_LOG(const SG_LOG&) = delete;
  SG_LOG& operator=(const SG_LOG&)  = delete;

public:
  virtual ~SG_LOG()
  {
    spdlog::drop_all();
  };

private:
  std::shared_ptr<spdlog::logger> critical_logger;
  std::shared_ptr<spdlog::logger> err_logger;
  std::shared_ptr<spdlog::logger> rotating_logger;
};

//如果没有生成对象，则生成并定义相关宏
//否则标记其为外部对象，直接使用
//可在整个工程里面  每个 cpp 里面包含该头文件
#ifndef _MY_OUTLOG_OBJECT_
#define _MY_OUTLOG_OBJECT_
extern SG_LOG SmaugLog;
#define SDEBUG(msg,...) SmaugLog.getRotatingLogger()->debug(suffix(msg),__VA_ARGS__)
#define SINFO(msg,...) SmaugLog.getRotatingLogger()->info(suffix(msg),__VA_ARGS__)
#define SWARN(msg,...) SmaugLog.getErrLogger()->warn(suffix(msg),__VA_ARGS__)
#define SERROR(msg,...) SmaugLog.getErrLogger()->error(suffix(msg),__VA_ARGS__)
#define SCR(msg,...) SmaugLog.getCriticalLogger()->critical(suffix(msg),__VA_ARGS__)
#endif


#endif


#include <iostream>
#include <exception>
#include <cstdlib>

void customTerminate()
{
  std::cout << "Custom Global Exception Handler called" << std::endl;
  std::abort(); // 强制终止程序
}

int main()
{
  // 设置自定义的全局异常处理器
  std::set_terminate(customTerminate);

  try
  {
    // 模拟抛出异常
    throw std::runtime_error("Custom exception message");
  }
  catch (const std::exception& e)
  {
    // 捕获异常
    std::cerr << "Caught Exception: " << e.what() << std::endl;
  }

  return 0;
}
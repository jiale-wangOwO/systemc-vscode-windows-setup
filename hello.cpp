#include <systemc.h>

int sc_main(int argc, char* argv[]) {
  sc_start(); // 启动SystemC仿真
  std::cout << "Hello World!" << std::endl;
  return 0;
}
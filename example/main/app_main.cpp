#include "board/m5stack_core_s3_cpp/m5stack_core_s3.hpp"

extern "C" void app_main()
{
  M5StackCoreS3& m5 = M5StackCoreS3::GetInstance();
  m5.Init();
};
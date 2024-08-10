#include "Context.h"
#include "Type.h"

#include <iostream>

int main() {
  auto ctx = tgen::Context::createContext();

  auto classTy = tgen::ClassType::getClassType("Pat", *ctx);

  SPDLOG_INFO("Ty = {}", *classTy);

  return 0;
}

#include "Context.h"

#include "ContextImpl.h"

namespace tgen {

Context::Context() = default;

Context::~Context() = default;

std::unique_ptr<Context> Context::createContext() {
  return std::make_unique<ContextImpl>();
}

} // tgen
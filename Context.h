#ifndef TGEN_CONTEXT_H
#define TGEN_CONTEXT_H
#include <memory>

namespace tgen {

class Context {
public:
  Context();
  virtual ~Context();

  Context(const Context &) = delete;
  Context &operator =(const Context &) = delete;

  static std::unique_ptr<Context> createContext();
};

} // tgen

#endif //TGEN_CONTEXT_H

#ifndef TYPECASTING_H
#define TYPECASTING_H

#include <stdexcept>

namespace tgen {

template <typename From, typename To>
To do_cast_impl(From from) {
  return dynamic_cast<To>(from);
}

template <typename To, typename From> bool is_a(From from) {
  return do_cast_impl<From, To>(from) != nullptr;
}

template <typename To, typename From> To must_cast(From from) {
  To result = do_cast_impl<From, To>(from);
  if (result == nullptr)
    throw std::runtime_error("Can't cast between types");
  return result;
}

template <typename To, typename From> To cast(From *from) {
  return do_cast_impl<From, To>(from);
}

}

#endif //TYPECASTING_H
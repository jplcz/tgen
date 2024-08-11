#ifndef VALUECAST_H
#define VALUECAST_H

#include "EvaluationScope.h"

namespace tgen {

class ValueCast {
public:
  ValueCast(EvaluationScope &scope) :
    Scope(scope) {
  }

  Value *castTo(Value *from, Type *toType) const;

private:
  Value *castToString(Value * from) const;
  Value *castToInt(Value * from) const;
  Value *castToBit(Value * from) const;
  Value *castToBits(Value * from, Type * to) const;

private:
  EvaluationScope &Scope;
};

}

#endif //VALUECAST_H
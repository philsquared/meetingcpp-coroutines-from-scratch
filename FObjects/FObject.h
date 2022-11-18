//
// Common base class for all "Financial Objects"
//

#ifndef NANOQUANT_FOBJECT_H
#define NANOQUANT_FOBJECT_H

#include <memory>

namespace nq {

struct FObject {
    virtual ~FObject() = default;
};

using FObjectPtr = std::shared_ptr<FObject>;

} // namespace nq

#endif //NANOQUANT_FOBJECT_H

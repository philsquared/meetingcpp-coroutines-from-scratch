#ifndef NANOQUANT_SERIALISATION_H
#define NANOQUANT_SERIALISATION_H

#include "../FObjects/FObject.h"
#include "Repo.h"

#include <string>

namespace nq {
    auto deserialise( Repo& repo, std::string const& id ) -> FObjectPtr;
}

#endif //NANOQUANT_SERIALISATION_H

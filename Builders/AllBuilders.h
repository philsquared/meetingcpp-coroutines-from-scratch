#ifndef QATEST_ALLBUILDERS_H
#define QATEST_ALLBUILDERS_H

#include "../Infrastructure/Deserialiser.h"
#include "../Infrastructure/BuildTask.h"

namespace nq {

    auto buildObject( Deserialiser&& in ) -> BuildTask;

} // namespace nq

#endif //QATEST_ALLBUILDERS_H

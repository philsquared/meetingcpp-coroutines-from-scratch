//
// Created by Phil Nash on 21/04/2022.
//
#ifndef NANOQUANT_FLAT_FORWARD_H
#define NANOQUANT_FLAT_FORWARD_H

#include "Curve.h"
#include "../Enums/Compounding.h"
#include "../Enums/Frequency.h"

namespace nq {

    struct FlatForward : Curve {
        Compounding compounding;
        Frequency frequency;
        // !TBD ...
    };

} // namespace nq

#endif //NANOQUANT_FLAT_FORWARD_H

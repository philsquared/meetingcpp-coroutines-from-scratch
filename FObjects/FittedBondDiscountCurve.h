//
// Created by Phil Nash on 21/04/2022.
//

#ifndef NANOQUANT_FITTEDBONDDISCOUNTCURVE_H
#define NANOQUANT_FITTEDBONDDISCOUNTCURVE_H

#include "Curve.h"

namespace nq {

    struct FittedBondDiscountCurve : Curve {
        double accuracy;
        unsigned int max_evaluations;
        double simplex_lambda;
        unsigned int max_stationary_state_iterations;
        // !TBD...
    };

} // namespace nq

#endif //NANOQUANT_FITTEDBONDDISCOUNTCURVE_H

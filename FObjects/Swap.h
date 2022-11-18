//
// Created by Phil Nash on 27/04/2022.
//

#ifndef NANOQUANT_SWAP_H
#define NANOQUANT_SWAP_H

#include "FObject.h"
#include "../SimpleTypes/Leg.h"

#include <vector>

namespace nq {

    struct Swap : FObject {
        enum Type { Receiver = -1, Payer = 1 };

        std::vector<Leg> legs;
        std::vector<double> payer;

        // Computed
        std::vector<double> leg_NPV;
        std::vector<double> leg_BPS;
        std::vector<double> start_discounts, end_discounts;
        double npv_date_discount;
    };

} // namespace nq

#endif //NANOQUANT_SWAP_H

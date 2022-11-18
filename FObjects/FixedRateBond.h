//
// Created by Phil Nash on 21/04/2022.
//
#ifndef NANOQUANT_FIXEDRATEBOND_H
#define NANOQUANT_FIXEDRATEBOND_H

#include "Bond.h"
#include "../Enums/Frequency.h"
#include "../SimpleTypes/DayCounter.h"

namespace nq {

    struct FixedRateBond : Bond {
        Frequency frequency;
        DayCounter day_counter;
        DayCounter first_period_day_counter;
    };

} // namespace nq

#endif //NANOQUANT_FIXEDRATEBOND_H

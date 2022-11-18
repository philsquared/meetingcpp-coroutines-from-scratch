//
// Created by Phil Nash on 27/04/2022.
//

#ifndef NANOQUANT_YEARONYEARINFLATIONSWAP_H
#define NANOQUANT_YEARONYEARINFLATIONSWAP_H

#include "Swap.h"
#include "../SimpleTypes/DayCounter.h"
#include "../SimpleTypes/Calendar.h"
#include "../Enums/BusinessDayConvention.h"
#include "../SimpleTypes/Period.h"
#include "YoYInflationIndex.h"

namespace nq {

    struct Schedule {};
    struct Spread{};

    struct YearOnYearInflationSwap : Swap {
        Type type;
        double nominal;
        Schedule fixed_schedule;
        double fixedRate;
        DayCounter fixed_day_count;
        Schedule yoy_schedule;
        std::shared_ptr<YoYInflationIndex> yoy_index;
        Period observation_lag;
        Spread spread;
        DayCounter yoy_day_count;
        Calendar payment_calendar;
        BusinessDayConvention payment_convention;

        // computed
        double fair_rate;
        Spread fair_spread;
    };

} // namespace nq

#endif //NANOQUANT_YEARONYEARINFLATIONSWAP_H

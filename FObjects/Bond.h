//
// Created by Phil Nash on 21/04/2022.
//
#ifndef NANOQUANT_BOND_H
#define NANOQUANT_BOND_H

#include "FObject.h"
#include "../SimpleTypes/Calendar.h"
#include "../SimpleTypes/Date.h"
#include "../SimpleTypes/Leg.h"

#include <vector>

namespace nq {

    struct Bond : FObject {
        int settlement_days;
        Calendar calendar;
        std::vector<Date> notional_schedule;
        std::vector<double> notionals;
        Leg cashflows;
        Leg redemptions;
        Date maturity_date;
        double settlement_value;
    };

} // namespace nq

#endif //NANOQUANT_BOND_H

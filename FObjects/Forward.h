#ifndef NANOQUANT_FORWARD_H
#define NANOQUANT_FORWARD_H

#include "FObject.h"
#include "Curve.h"
#include "../SimpleTypes/DayCounter.h"
#include "../SimpleTypes/Calendar.h"
#include "../Enums/BusinessDayConvention.h"
#include "../SimpleTypes/Payoff.h"
#include "../SimpleTypes/Date.h"

namespace nq {

    struct Forward : FObject {
        DayCounter day_counter;
        Calendar calendar;
        BusinessDayConvention business_day_convention;
        int settlement_days;
        Payoff payoff;
        Date value_date;
        Date maturity_date;
        std::shared_ptr<Curve> discount_curve;
        std::shared_ptr<Curve> income_discount_curve;
    };

} // namespace nq

#endif //NANOQUANT_FORWARD_H

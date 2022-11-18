//
// Created by Phil Nash on 27/04/2022.
//

#ifndef NANOQUANT_INFLATIONCURVE_H
#define NANOQUANT_INFLATIONCURVE_H

#include "../SimpleTypes/Period.h"
#include "../Enums/Frequency.h"

namespace nq {

    struct Rate{};
    struct Seasonality {}; // Polymorphic..

    struct InflationCurve {
        std::shared_ptr<Seasonality> seasonality;
        Period observation_lag;
        Frequency frequency;
        Rate base_rate;
    };

} // namespace nq

#endif //NANOQUANT_INFLATIONCURVE_H

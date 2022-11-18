//
// Created by Phil Nash on 27/04/2022.
//

#ifndef NANOQUANT_INFLATIONINDEX_H
#define NANOQUANT_INFLATIONINDEX_H

#include "Index.h"
#include "YearOnYearInflationSwap.h"

#include "../SimpleTypes/Date.h"
#include "../Enums/Frequency.h"
#include "../SimpleTypes/Region.h"
#include "../SimpleTypes/Currency.h"

#include <string>

namespace nq {

    struct InflationIndex : Index {
        Date reference_date;
        std::string family_name;
        Region region;
        bool revised;
        bool interpolated;
        Frequency frequency;
        Period availability_lag;
        Currency currency;
    private:
        std::string name;
    };

} // namespace nq

#endif //NANOQUANT_INFLATIONINDEX_H

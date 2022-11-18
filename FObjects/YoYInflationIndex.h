//
// Created by Phil Nash on 27/04/2022.
//

#ifndef NANOQUANT_YOYINFLATIONINDEX_H
#define NANOQUANT_YOYINFLATIONINDEX_H

#include "InflationIndex.h"
#include "YoYInflationCurve.h"

namespace nq {

    struct YoYInflationIndex : InflationIndex {
        bool ratio;
        std::shared_ptr<YoYInflationCurve> yoy_inflation;
    };

} // namespace nq

#endif //NANOQUANT_YOYINFLATIONINDEX_H

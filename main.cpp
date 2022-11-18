#include "FObjects/FixedRateBondForward.h"
#include "FObjects/FittedBondDiscountCurve.h"

#include "Infrastructure/Repo.h"
#include "Infrastructure/Log.h"

#include <chrono>
#include <cassert>

int main() {
    nq::Repo repo;

    auto start = std::chrono::high_resolution_clock::now();

    // Request objects
    auto objects = repo.loadAndBuildObjects({ "FixedRateBondForward/1", "FixedRateBondForward/2" });

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end-start;
    LOG( "Completed in " << elapsed.count() << "ms" );

    // Check we actually loaded stuff
    auto fixedRateBondForward = dynamic_pointer_cast<nq::FixedRateBondForward>(objects[1]);
    auto curve = std::dynamic_pointer_cast<nq::FittedBondDiscountCurve>(fixedRateBondForward->discount_curve);
    assert( curve->simplex_lambda == 1.23 );
}

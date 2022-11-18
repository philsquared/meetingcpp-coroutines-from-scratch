#include "../FObjects/FixedRateBondForward.h"
#include "../FObjects/FlatForward.h"
#include "../FObjects/FittedBondDiscountCurve.h"

#include "Serialisation.h"
#include "Deserialiser.h"
#include "Log.h"

#include <fstream>
#include <chrono>
#include <thread>

namespace nq {

    template<typename T>
    auto build( Repo& repo, Deserialiser& in );

    template<>
    auto build<FixedRateBondForward>( Repo& repo, Deserialiser& in ) {
        auto obj = std::make_shared<FixedRateBondForward>();
        obj->settlement_days << in["settlement_days"];
        // !TBD: other fields

        // Load dependencies
        auto deps = repo.loadAndBuildObjects( {
              in.get<std::string>( "fixed_coupon_bond" ),
              in.get<std::string>( "discount_curve" ),
              in.get<std::string>( "income_discount_curve" ) } );

        // Set dependencies
        obj->fixed_coupon_bond = std::dynamic_pointer_cast<FixedRateBond>( deps[0]);
        obj->discount_curve = std::dynamic_pointer_cast<Curve>(deps[1]);
        obj->income_discount_curve = std::dynamic_pointer_cast<Curve>(deps[2]);

        return obj;
    }

    template<>
    auto build<FixedRateBond>( Repo& repo, Deserialiser& in ) {
        auto obj = std::make_shared<FixedRateBond>();
        // ...
        return obj;
    }

    template<>
    auto build<FlatForward>( Repo& repo, Deserialiser& in ) {
        auto obj = std::make_shared<FlatForward>();
        // ...
        return obj;
    }

    template<>
    auto build<FittedBondDiscountCurve>( Repo& repo, Deserialiser& in ) {
        auto obj = std::make_shared<FittedBondDiscountCurve>();

        obj->accuracy << in["accuracy"];
        obj->max_evaluations << in["max_evaluations"];
        obj->simplex_lambda << in["simplex_lambda"];
        obj->max_stationary_state_iterations << in["max_stationary_state_iterations"];

        return obj;
    }

    auto loadAsJson( std::string const& id ) {

        // Sleep for a bit to simulate datastore access latency
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10ms);

        // Open file
        auto path = "repo/" + std::string(id) + ".json";
        std::ifstream f(path );
        if( !f )
            throw std::domain_error("Missing data for " + std::string(id) );

        // read in as JSON
        nlohmann::json json;
        f >> json;

        return json;
    }

    auto deserialise( Repo& repo, std::string const& id ) -> FObjectPtr {
        Deserialiser in( loadAsJson( id ) );
        auto type = in.get<std::string>("type");

        // Create object of appropriate type and deserialise into it from JSON
        if( type == "FixedRateBondForward" )
            return build<FixedRateBondForward>( repo, in );
        if( type == "FlatForward" )
            return build<FlatForward>( repo, in );
        if( type == "FittedBondDiscountCurve" )
            return build<FittedBondDiscountCurve>( repo, in );
        if( type == "FixedRateBond" )
            return build<FixedRateBond>( repo, in );

        // Polymorphic types: extract derived id and recurse
        if( type == "Curve" )
            return deserialise( repo, in.get<std::string>( "instance" ) );

        throw std::domain_error("Unrecognised type: " + std::string(type) );
    }

}

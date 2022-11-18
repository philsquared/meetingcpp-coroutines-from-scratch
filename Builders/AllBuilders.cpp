#include "AllBuilders.h"

#include "../FObjects/Forward.h"
#include "../FObjects/FlatForward.h"
#include "../FObjects/FixedRateBond.h"
#include "../FObjects/FixedRateBondForward.h"
#include "../FObjects/FittedBondDiscountCurve.h"

#include "../Infrastructure/Dependencies.h"

namespace nq {

    template<typename T>
    auto build( Deserialiser in ) -> BuildTask;

    template<>
    auto build<FixedRateBondForward>( Deserialiser in ) -> BuildTask {

        auto obj = std::make_shared<FixedRateBondForward>();
        obj->settlement_days << in["settlement_days"];

        Dependencies dependencies(in);
        dependencies
                .require( obj->discount_curve, "discount_curve" )
                .require( obj->income_discount_curve, "income_discount_curve" )
                .require( obj->fixed_coupon_bond, "fixed_coupon_bond" );

        BuildTask task;
        task.continueWithDependencies( std::move(dependencies), [obj=std::move(obj)] {
                LOG( "Resuming" );

                // build phase ...

                return obj;
            } );
        return task;
    }

    template<>
    auto build<FixedRateBond>( Deserialiser in ) -> BuildTask {
        auto obj = std::make_shared<FixedRateBond>();
        // ...
        return obj;
    }

    template<>
    auto build<FlatForward>( Deserialiser in ) -> BuildTask {
        auto obj = std::make_shared<FlatForward>();
        // ...
        return obj;
    }

    template<>
    auto build<FittedBondDiscountCurve>( Deserialiser in ) -> BuildTask {
        auto obj = std::make_shared<FittedBondDiscountCurve>();

        obj->accuracy << in["accuracy"];
        obj->max_evaluations << in["max_evaluations"];
        obj->simplex_lambda << in["simplex_lambda"];
        obj->max_stationary_state_iterations << in["max_stationary_state_iterations"];

        return obj;
    }

    template<>
    auto build<Curve>( Deserialiser in ) -> BuildTask {
        auto curveHolder = std::make_shared<std::shared_ptr<Curve>>();
        Dependencies dependencies( in );
        dependencies.require( *curveHolder, "instance" );

        BuildTask task;
        task.continueWithDependencies( std::move(dependencies), [curveHolder=std::move(curveHolder)]() {
            assert( *curveHolder.get() );

            LOG( "Curve resumed - returning the object" );
            return *curveHolder;
        } );
        return task;
    }


    auto buildObject( Deserialiser&& in ) -> BuildTask {
        auto type = in.get<std::string>("type");

        if( type == "FixedRateBondForward" )
            return build<FixedRateBondForward>( std::move(in) );
        if( type == "FixedRateBond" )
            return build<FixedRateBond>( std::move(in) );
        if( type == "FlatForward" )
            return build<FlatForward>( std::move(in) );
        if( type == "FittedBondDiscountCurve" )
            return build<FittedBondDiscountCurve>( std::move(in) );
        if( type == "Curve" )
            return build<Curve>( std::move(in) );

        throw std::domain_error( "Unrecognised type: " + std::string(type) );
    }

} // namespace nq
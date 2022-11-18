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
        auto frame = makeFrame<BuildTask::Data> ( [in=std::move(in)](auto co) mutable {

            auto obj = std::make_shared<FixedRateBondForward>();
            obj->settlement_days << in["settlement_days"];

            Dependencies dependencies(in);
            dependencies
                    .require( obj->discount_curve, "discount_curve" )
                    .require( obj->income_discount_curve, "income_discount_curve" )
                    .require( obj->fixed_coupon_bond, "fixed_coupon_bond" );

            return co.await( std::move(dependencies) ).then( [obj=std::move(obj)](auto co) {
                    LOG( "Resumed" );

                    // build phase ...

                    co.return_( obj );
                } );
        } );
        return frame->data.get_return_object();
    }

    template<>
    auto build<FixedRateBond>( Deserialiser in ) -> BuildTask {
        auto frame = makeFrame<BuildTask::Data> ( [in=std::move(in)](auto co) mutable {
            auto obj = std::make_shared<FixedRateBond>();
            // ...
            co.return_( obj );
        } );
        return frame->data.get_return_object();
    }

    template<>
    auto build<FlatForward>( Deserialiser in ) -> BuildTask {
        auto frame = makeFrame<BuildTask::Data> ( [in=std::move(in)](auto co) mutable {
            auto obj = std::make_shared<FlatForward>();
            // ...
            co.return_( obj );
        } );
        return frame->data.get_return_object();
    }

    template<>
    auto build<FittedBondDiscountCurve>( Deserialiser in ) -> BuildTask {
        auto frame = makeFrame<BuildTask::Data> ( [in=std::move(in)](auto co) mutable {
            LOG( "build<FittedBondDiscountCurve>" );
            auto obj = std::make_shared<FittedBondDiscountCurve>();

            obj->accuracy << in["accuracy"];
            obj->max_evaluations << in["max_evaluations"];
            obj->simplex_lambda << in["simplex_lambda"];
            obj->max_stationary_state_iterations << in["max_stationary_state_iterations"];

            co.return_( obj );
        } );
        return frame->data.get_return_object();
    }

    template<>
    auto build<Curve>( Deserialiser in ) -> BuildTask {
        auto frame = makeFrame<BuildTask::Data> ( [in=std::move(in)](auto co) mutable {
            auto curveHolder = std::make_shared<std::shared_ptr<Curve>>();
            Dependencies dependencies( in );
            dependencies.require( *curveHolder, "instance" );

            return co.await( std::move(dependencies) ).then( [curveHolder=std::move(curveHolder)](auto co) {
                assert( *curveHolder.get() );

                LOG( "Curve resumed - returning the object" );
                co.return_( *curveHolder );
            } );
        } );
        return frame->data.get_return_object();
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
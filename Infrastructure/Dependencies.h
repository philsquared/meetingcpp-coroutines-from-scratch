#ifndef NANOQUANT_DEPENDENCIES_H
#define NANOQUANT_DEPENDENCIES_H

#include "Deserialiser.h"
#include "Dependency.h"
#include "Log.h"

#include <vector>
#include <coroutine>

namespace nq {

    class Dependencies {
        friend class BuildTask;

        std::vector<Dependency> unmetDependencies;
        Deserialiser const& in;

    public:
        explicit Dependencies( Deserialiser const& in ) : in(in) {}

        template<typename T>
        auto require( std::shared_ptr<T>& field, std::string const& fieldName ) -> Dependencies& {
            auto setter = [&field]( FObjectPtr const& dep ) {
                field = std::dynamic_pointer_cast<T>( dep );
            };
            auto dependentId = in.get<std::string>( fieldName );
            LOG_VAR( dependentId );
            unmetDependencies.emplace_back( std::move( dependentId), std::move( setter ) );
            return *this;
        }
    };

} // namespace nq

#endif //NANOQUANT_DEPENDENCIES_H

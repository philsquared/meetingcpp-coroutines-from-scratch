#ifndef NANOQUANT_BUILDTASK_H
#define NANOQUANT_BUILDTASK_H

#include "../FObjects/FObject.h"

#include "Deserialiser.h"
#include "Dependency.h"
#include "Log.h"
#include "Dependencies.h"

#include <span>

namespace nq {

    class BuildTask {
        using Continuation = std::function<FObjectPtr()>;
        struct Data {
            std::string id;
            std::vector<Dependency> dependencies;
            FObjectPtr obj = nullptr;
            Continuation continuation;
        };
        std::unique_ptr<Data> data = std::make_unique<Data>();

    public:
        BuildTask() = default;

        template<typename T>
        explicit(false) BuildTask( std::shared_ptr<T> obj ) {
            data->obj = std::move( obj );
        }

        void continueWithDependencies( Dependencies&& dependencies, Continuation continuation ) {
            data->dependencies = std::move( dependencies.unmetDependencies );
            data->continuation = std::move( continuation );
        }

        void setId( std::string const& id ) {
            data->id = id;
        }
        auto getId() const {
            return data->id;
        }

        auto getDependencies() const -> std::span<Dependency> {
            return data->dependencies;
        }

        auto operator <=> ( BuildTask const& other ) const noexcept -> std::weak_ordering {
            auto cmp = data->id.compare( other.data->id );
            if( cmp == 0 )
                return std::weak_ordering::equivalent;
            else if( cmp < 0 )
                return std::weak_ordering::less;
            else
                return std::weak_ordering::greater;
        }

        auto resume() const -> FObjectPtr;
    };

} // namespace nq

#endif //NANOQUANT_BUILDTASK_H

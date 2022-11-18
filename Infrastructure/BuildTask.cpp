#include "BuildTask.h"
#include "Dependencies.h"

namespace nq {

    auto BuildTask::Data::await_transform( Dependencies&& dependencies ) -> DependencyAwaiter {
        this->dependencies = std::move( dependencies.unmetDependencies );
        return DependencyAwaiter{ !this->dependencies.empty() };
    }

    auto BuildTask::resume() const -> FObjectPtr {
        LOG( "potentially resuming task for " << getId() );
        if( !handle.done() ) {
            LOG( " - resuming" );
            handle.resume();
            if( !handle.done())
                return {};
        }
        assert( data().obj );
        return data().obj;
    }

} // namespace nq

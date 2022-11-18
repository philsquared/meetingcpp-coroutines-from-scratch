#include "BuildTask.h"
#include "Dependencies.h"

namespace nq {

    auto BuildTask::promise_type::await_transform( Dependencies& dependencies ) -> DependencyAwaiter {
        this->dependencies = dependencies.unmetDependencies;
        return DependencyAwaiter{ !this->dependencies.empty() };
    }

    auto BuildTask::resume() const -> FObjectPtr {
        LOG( "potentially resuming task for " << getId() );
        if( !handle.done() ) {
            LOG( " - resuming" );
            handle.resume();
            if( !handle.done() )
                return {};
        }
        assert( promise().obj );
        return promise().obj;
    }

} // namespace nq

#include "BuildTask.h"

namespace nq {
    auto BuildTask::resume() const -> FObjectPtr {
        LOG( "potentially resuming task for " << getId() );
        if( data->continuation ) {
            LOG( " - resuming" );
            auto obj = data->continuation();
            data->continuation = Continuation();
            return obj;
        }
        assert( data->obj );
        return data->obj;
    }

} // namespace nq

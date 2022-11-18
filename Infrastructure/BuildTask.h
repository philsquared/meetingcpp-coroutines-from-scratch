#ifndef NANOQUANT_BUILDTASK_H
#define NANOQUANT_BUILDTASK_H

#include "../FObjects/FObject.h"

#include "Deserialiser.h"
#include "Dependency.h"
#include "Log.h"
#include "Continuations.h"

#include <span>
#include <string>

namespace nq {

    struct DependencyAwaiter {
        bool shouldSuspend;

        bool ready() const noexcept {
            return !shouldSuspend;
        }
        bool suspend( FrameHandle<> handle ) noexcept {
            LOG("suspending");
            return true;
        }
        void resume() noexcept {
            LOG("resuming" );
        }
    };

    class Dependencies;

    class BuildTask {
    public:

        struct Data {
            std::string id;
            std::vector<Dependency> dependencies;
            FObjectPtr obj = nullptr;

            auto get_return_object()                { return BuildTask( Handle::from_data( *this ) ); }
            auto initial_suspend() const -> bool    { return false; }
            void return_value( FObjectPtr&& new_obj ) {
                LOG( "Setting return object for " << id << " (" << new_obj.get() << ")" );
                obj = std::move(new_obj);
            }
            auto await_transform( Dependencies&& dependencies ) -> DependencyAwaiter;
        };

    private:
        using Handle = FrameHandle<Data>;
        Handle handle;
        auto data() const -> Data& {
            assert( handle );
            return handle.data();
        }
    public:
        explicit BuildTask( Handle handle ) noexcept
        : handle( handle ) {
            assert( handle );
        }
        ~BuildTask() {
            destroyFrame();
        }
        void destroyFrame() {
            if( handle ) {
                LOG( "(" << this << ") destroying frame Handle: " << handle.address() );
                handle.destroy();
            }
            else
                LOG( "(" << this << ") empty/ moved-from" );
        }

        // move-only
        BuildTask( BuildTask&& other ) noexcept {
            assert( other.handle );
            std::swap( handle, other.handle );
        }
        auto operator=( BuildTask&& other ) noexcept -> BuildTask& {
            destroyFrame();
            handle = other.handle;
            other.handle = Handle();
            return *this;
        }

        void setId( std::string const& id ) {
            data().id = id;
        }
        auto getId() const {
            return data().id;
        }

        auto getDependencies() const -> std::span<Dependency> {
            return data().dependencies;
        }

        auto operator <=> ( BuildTask const& other ) const noexcept {
            return handle <=> other.handle;
        }

        auto resume() const -> FObjectPtr;
    };

} // namespace nq

#endif //NANOQUANT_BUILDTASK_H

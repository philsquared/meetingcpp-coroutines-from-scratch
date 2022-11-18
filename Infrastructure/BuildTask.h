#ifndef NANOQUANT_BUILDTASK_H
#define NANOQUANT_BUILDTASK_H

#include "../FObjects/FObject.h"

#include "Deserialiser.h"
#include "Dependency.h"
#include "Log.h"
#include "TaskTracker.h"

#include <coroutine>
#include <span>
#include <string>

namespace nq {

    struct DependencyAwaiter {
        bool shouldSuspend;

        bool await_ready() const noexcept {
            return !shouldSuspend;
        }
        bool await_suspend( std::coroutine_handle<> ) noexcept {
            LOG("suspending");
            return true;
        }
        void await_resume() noexcept {
            LOG("resuming" );
        }
    };

    class Dependencies;

    class BuildTask : TaskTracker {
    public:
        struct promise_type {
            std::string id;
            std::span<Dependency> dependencies; // from awaiter
            FObjectPtr obj = nullptr;

            // Coroutine invoked methods
            auto get_return_object()            { return BuildTask( Handle::from_promise( *this ) ); }
            auto initial_suspend() const        { return std::suspend_never{}; }
            auto final_suspend() const noexcept { return std::suspend_always{}; }
            void return_value( FObjectPtr&& new_obj ) {
                LOG( "Setting return object for " << id << " (" << new_obj.get() << ")" );
                obj = std::move(new_obj);
            }
            auto await_transform( Dependencies& dependencies ) -> DependencyAwaiter;
            void unhandled_exception() const    { std::terminate(); }
        };
    private:
        using Handle = std::coroutine_handle<promise_type>;
        Handle handle;
        auto promise() const -> promise_type& {
            assert( handle );
            return handle.promise();
        }
    public:
        explicit BuildTask( Handle handle ) noexcept
        : handle( handle ) {
            assert( handle );
        }
        ~BuildTask() {
            destroyCoroutine();
        }
        void destroyCoroutine() const {
            if( handle ) {
                LOG( "(" << this << ") destroying coroutine Handle: " << handle.address() );
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
            destroyCoroutine();
            handle = other.handle;
            other.handle = Handle();
            return *this;
        }

        void setId(std::string const& id ) {
            promise().id = id;
        }
        auto getId() const {
            return promise().id;
        }

        auto getDependencies() const -> std::span<Dependency> {
            return promise().dependencies;
        }

        auto operator <=> ( BuildTask const& other ) const noexcept {
            return handle <=> other.handle;
        }

        auto resume() const -> FObjectPtr;
    };

} // namespace nq

#endif //NANOQUANT_BUILDTASK_H

//
// Created by Phil Nash on 01/09/2022.
//

#ifndef NANOQUANT_CONTINUATIONS_H
#define NANOQUANT_CONTINUATIONS_H

#include <memory>
#include <functional>
#include <cassert>

namespace nq {

    template<typename DataT=void>
    struct FrameHandle;

    struct AwaiterAdapterBase {
        virtual ~AwaiterAdapterBase() = default;
        virtual auto ready() const noexcept -> bool = 0;
        virtual auto suspend( FrameHandle<> const& handle ) noexcept -> bool = 0;
        virtual void resume() noexcept = 0;
    };

    template<typename AwaiterT>
    struct AwaiterAdapter : AwaiterAdapterBase {
        AwaiterT awaiter;

        explicit AwaiterAdapter( AwaiterT&& awaiter ) : awaiter( std::move( awaiter ) ) {}

        auto ready() const noexcept -> bool override {
            return awaiter.ready();
        }
        auto suspend( FrameHandle<> const& handle ) noexcept -> bool override {
            return awaiter.suspend( handle );
        }
        void resume() noexcept override {
            awaiter.resume();
        }
    };

    struct FrameBase {
        virtual ~FrameBase() noexcept = default;
        virtual auto done() const -> bool = 0;
        virtual void resume() = 0;
    };

    template<typename DataT>
    struct Frame : FrameBase {
        DataT data;
        //...

        std::unique_ptr<AwaiterAdapterBase> awaiter;

        struct AwaitMgr {
            Frame& frame;

            template<typename T>
            void then( T&& continuation ) {
                frame.setContinuation( std::forward<T>( continuation ) );
            }
        };

        struct ContinuationMgr {
            Frame& frame;
            explicit ContinuationMgr( Frame& frame ) : frame( frame ){}

            template<typename T>
            void return_( T&& v ) {
                frame.data.return_value( std::forward<T>(v) );
            }
            template<typename T>
            auto await( T&& awaitable ) -> AwaitMgr {
                frame.awaiter.reset( new AwaiterAdapter( frame.data.await_transform( std::forward<T>( awaitable) ) ) );
                // !TBD: don't suspend if awaiter is "ready"
                frame.awaiter->suspend( FrameHandle<DataT>{ &frame } );
                return { frame };
            }
        };
        using Continuation = std::function<void(ContinuationMgr)>;
        Continuation continuation;

        ~Frame() noexcept override = default;

        auto done() const -> bool final {
            return !continuation;
        }
        template<typename T>
        void setContinuation( T&& continuation ) {
            this->continuation = std::forward<T>(continuation);
        }
        void resume() final {
            if( continuation ) {
                awaiter->resume();
                auto next_continuation = std::move( continuation );
                continuation = Continuation();
                next_continuation( ContinuationMgr(*this) );
            }
        }

    };

    template<typename DataT, typename ClosureT>
    struct FrameWithInitialClosure : Frame<DataT> {
        ClosureT initialClosure;

        explicit FrameWithInitialClosure( ClosureT&& initialClosure )
                : initialClosure( std::move( initialClosure ) )
        {
            using Base = Frame<DataT>;
            if( Base::data.initial_suspend() ) // !TBD at compile time, if possible
                Base::continuation = this->initialClosure;
            else
                this->initialClosure( typename Base::ContinuationMgr( *this ) );
        }
    };

    template<typename DataT, typename ClosureT>
    auto makeFrame( ClosureT&& closure ) {
        return new FrameWithInitialClosure<DataT, ClosureT>( std::forward<ClosureT>( closure ) );
    }

    template<typename DataT>
    struct FrameHandle {
        Frame<DataT>* frame = nullptr;

        auto data() const -> DataT& {
            assert( frame );
            return frame->data;
        }
        static auto from_data( DataT& data ) -> FrameHandle<DataT> {
            auto dataOffset = offsetof(Frame<DataT>, data);
            auto dataAddr = ptrdiff_t( std::addressof(data) );
            auto frameAddr = dataAddr-dataOffset;
            auto frame = reinterpret_cast<Frame<DataT>*>( frameAddr );
            assert( &data == &frame->data );
            return FrameHandle{ frame };
        }

        explicit operator bool() const { return frame; }

        auto operator <=> ( FrameHandle const& other ) const noexcept -> std::weak_ordering {
            auto cmp = data().id.compare( other.data().id );
            if( cmp == 0 )
                return std::weak_ordering::equivalent;
            else if( cmp < 0 )
                return std::weak_ordering::less;
            else
                return std::weak_ordering::greater;
        }

        auto done() const -> bool {
            return frame->done();
        }
        void resume() const {
            frame->resume();
        }
        void destroy() {
            delete frame;
            frame = nullptr;
        }
        auto address() const -> void* {
            return frame;
        }
    };

    template<>
    struct FrameHandle<void> {

        FrameBase* frame = nullptr;

        template<typename T>
        explicit(false) FrameHandle( FrameHandle<T> const& typedHandle ) : frame( typedHandle.frame ) {}

        explicit operator bool() const { return frame; }
        auto done() const -> bool {
            return frame->done();
        }
        void resume() const {
            frame->resume();
        }
        void destroy() {
            delete frame;
        }
    };

} // namespace nq

#endif //NANOQUANT_CONTINUATIONS_H

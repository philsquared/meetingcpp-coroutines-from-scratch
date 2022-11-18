#ifndef NANOQUANT_DEPENDENCY_H
#define NANOQUANT_DEPENDENCY_H

#include "../FObjects/FObject.h"

#include <functional>

namespace nq {

    class Dependency {
        using SetterFunction = std::function<void(FObjectPtr const &)>;
        SetterFunction setter;
    public:
        std::string id;
        Dependency( std::string&& id, SetterFunction&& setter )
        :   setter( std::move( setter ) ),
            id( std::move(id ) )
        {}

        auto isMet() const -> bool {
            return !setter;
        }

        void supplyDependency( FObjectPtr const& obj ) {
            assert( !isMet() );
            setter( obj );
            setter = SetterFunction();
        }
    };

} // namespace nq

#endif //NANOQUANT_DEPENDENCY_H

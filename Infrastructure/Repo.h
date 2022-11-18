#ifndef NANOQUANT_REPO_H
#define NANOQUANT_REPO_H

#include <unordered_map>
#include <string>
#include <vector>

#include "../FObjects/FObject.h"

namespace nq {

    class Repo {
        std::unordered_map<std::string, FObjectPtr> cache;

        auto load( std::string const& id ) -> FObjectPtr;

    public:
        auto loadAndBuildObjects( std::initializer_list<std::string> ids) -> std::vector<FObjectPtr>;
    };

} // namespace nq

#endif //NANOQUANT_REPO_H

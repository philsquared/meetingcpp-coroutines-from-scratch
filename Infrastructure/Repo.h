#ifndef NANOQUANT_REPO_H
#define NANOQUANT_REPO_H

#include <unordered_map>
#include <string>
#include <vector>

#include "../FObjects/FObject.h"
#include "BuildTask.h"

namespace nq {

    class Repo {
        std::unordered_map<std::string, FObjectPtr> cache;
        std::vector<std::string> toLoad;

        auto requireObject( std::string const& id ) -> FObjectPtr;
        auto resolveDependencies( BuildTask const& task ) -> bool;
    public:
        auto loadAndBuildObjects( std::initializer_list<std::string> ids ) -> std::vector<FObjectPtr>;
    };

} // namespace nq

#endif //NANOQUANT_REPO_H

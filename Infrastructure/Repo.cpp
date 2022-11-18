#include "Repo.h"
#include "Serialisation.h"

#include <algorithm>
#include <ranges>

namespace nq {

    auto Repo::requireObject( std::string const& id ) -> FObjectPtr {

        // If it's in the cache, return it
        if( auto it = cache.find( id ); it != cache.end() )
            return it->second;

        // Otherwise mark as required for loading
        if( std::ranges::find( toLoad, id ) == toLoad.end() )
            toLoad.push_back( id);
        return {};
    }

    auto Repo::resolveDependencies( BuildTask const& task ) -> bool {
        // resolve any dependencies we already have (and note any we don't)
        bool hasUnmetDependencies = false;
        for( auto &&dep: task.getDependencies() ) {
            if( !dep.isMet() ) {
                if( auto obj = requireObject( dep.id ) ) {
                    dep.supplyDependency( obj );
                } else {
                    hasUnmetDependencies = true;
                }
            }
        }

        if( !hasUnmetDependencies ) {
            if( auto obj = task.resume() ) {
                cache.try_emplace( task.getId(), obj );
                return true;
            }
        }
        return false;
    }

    auto Repo::loadAndBuildObjects( std::initializer_list<std::string> ids ) -> std::vector<FObjectPtr> {

        std::ranges::for_each( ids, [this]( auto const& id ){ requireObject(id); } );

        std::vector<BuildTask> incompleteTasks;
        while( !toLoad.empty()) {
            auto buildTasks = deserialiseAll( toLoad );
            toLoad.clear();

            for( auto&& task :  buildTasks ) {
                if( !resolveDependencies( task ) )
                    incompleteTasks.emplace_back( std::move( task ) );
            }

        } // end while

        std::ranges::for_each( incompleteTasks | std::ranges::views::reverse,
                               [this]( auto const& task ){ resolveDependencies( task ); });

        // At this point all objects should be loaded and built
        std::vector<FObjectPtr> objects;
        objects.reserve( ids.size() );
        std::ranges::transform(ids, std::back_inserter(objects), [this](auto const& id){ return cache[id]; });
        return objects;
    }

} // namespace nq

#include "../Builders/AllBuilders.h"

#include "Serialisation.h"
#include "Deserialiser.h"

#include "json.hpp"

#include <fstream>
#include <algorithm>
#include <chrono>
#include <thread>

namespace nq {

    namespace {
        auto loadAsJson( std::string const& id ) {

            // Open file
            auto path = "repo/" + id + ".json";
            std::ifstream f( path );
            if( !f )
                throw std::domain_error( "Missing data for " + std::string(id) );

            // read in as JSON
            nlohmann::json json;
            f >> json;

            return json;
        }
    }

    auto deserialiseAll( std::vector<std::string> const& ids ) -> std::vector<BuildTask> {

        // Sleep for a bit to simulate datastore access latency
        using namespace std::chrono_literals;
        std::this_thread::sleep_for( 10ms );

        // This part simulates retrieving the data from a remote datastore
        std::vector<BuildTask> tasks;
        tasks.reserve( ids.size() );

        for( auto const& id : ids ) {
            LOG( "loading: " << id );
            Deserialiser in( loadAsJson( id ) );

            LOG( "Building: " << id );
            auto task = buildObject( std::move( in ));
            LOG( "got build task for " << id );

            task.setId( id );

            tasks.emplace_back( std::move( task ) );
        }

        return tasks;
    }
}

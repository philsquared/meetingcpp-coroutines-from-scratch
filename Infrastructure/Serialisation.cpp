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

            return Deserialiser(json);
        }
    }

    // Q: Why does this need to take ids by value?
    auto deserialiseAll( std::vector<std::string> ids ) -> LoadedObjectGenerator {

        // Sleep for a bit to simulate datastore access latency
        using namespace std::chrono_literals;
        std::this_thread::sleep_for( 10ms );

        // This part simulates retrieving the data from a remote datastore
        std::vector<Deserialiser> deserialisers;
        deserialisers.reserve( ids.size() );

        std::ranges::transform(ids, std::back_inserter(deserialisers),
                               [](auto const& id){ return loadAsJson(id); });


        // Now iterate the raw data and start building the objects,
        // yielding partially "built" objects as we go
        for( size_t i = 0; i < ids.size(); ++i ) {
            auto const& id = ids[i];
            auto task = buildObject( std::move( deserialisers[i] ));
            task.setId( id );

            co_yield std::move( task );
        }
    }
}

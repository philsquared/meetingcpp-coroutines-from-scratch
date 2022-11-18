#ifndef QATEST_DESERIALISER_H
#define QATEST_DESERIALISER_H

#include "json.hpp"
#include <string>

namespace nq {

    // A simpler wrapper round a json object
    class Deserialiser {
        nlohmann::json json;

        struct FieldDeserialiser {
            std::string const& name;
            Deserialiser const& in;

            template<typename T>
            friend auto operator << ( T& field, FieldDeserialiser&& fd ) -> Deserialiser const& {
                field = fd.in.get<T>( fd.name );
                return fd.in;
            }
        };

    public:
        explicit Deserialiser( nlohmann::json json ) : json( std::move( json ) ) {}
        Deserialiser( Deserialiser&& other ) : json( std::move( other.json ) ) {}
        Deserialiser( Deserialiser const& other ) = default; // !TBD Only here so we can hold lambda in std::function

        template<typename T>
        auto get( std::string const& name ) const -> T {
            return json[name].get<T>();
        }

        auto operator [] ( std::string const& name ) const -> FieldDeserialiser {
            return { name, *this };
        }
    };

} // namespace nq

#endif //QATEST_DESERIALISER_H

#ifndef UUID_H
#define UUID_H

#include "Core/Sprocket.pch"

namespace Sprocket {
    
    class UUID {

    private:

        inline static std::unordered_set<uint64_t> m_ExistingUUIDs;

    public:

        /// @brief Generates a unique 64-bit unsigned integer UUID.
        /// @return 64-bit unsigned integer UUID.
        static uint64_t Generate();

    };

}

#endif
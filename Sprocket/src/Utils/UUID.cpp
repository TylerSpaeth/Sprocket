#include "UUID.h"

namespace Sprocket {

    uint64_t UUID::Generate() {
        std::random_device rd;

        std::mt19937_64 gen(rd());

        uint64_t uuid;
        do {
            uuid = gen();
        }
        while(m_ExistingUUIDs.count(uuid) != 0);

        m_ExistingUUIDs.insert(uuid);
        return uuid;
    }

}
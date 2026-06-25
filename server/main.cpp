#include <iostream>
#include <vector>

#include "database/database.hpp"
#include "mrp/include/mrp.hpp"

int main() {

    Database database{};

    database.createMRPTable();
    database.createMRPHeader(MRPHeader("client1", getCurrentUTCDate(), "wsp bro"));
    std::vector<MRPHeader> mrpHeaders = database.getMRPHeaders(5);

    for (MRPHeader mrpHeader : mrpHeaders)
        std::cout << mrpHeader.clientName << " " << mrpHeader.utcDate << " " << mrpHeader.messageData;

    return 0;
}

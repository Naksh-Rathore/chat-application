#pragma once

#include <sqlite3.h>

#include <vector>

#include "mrp/include/mrp.hpp"

class Database {

public:

    Database();
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    void createMRPTable();
    std::vector<MRPHeader> getMRPHeaders(int numHeaders);
    void createMRPHeader(const MRPHeader& mrpHeader);

private:

    sqlite3 *database = nullptr;

    void errorCheck(int returnCode);
};

#pragma once

#include <string>

struct MRPHeader {
    std::string clientName;
    std::string utcDate;
    std::string messageData;

    MRPHeader(const std::string& client_name, const std::string& utc_date, const std::string& messageData);

    std::string getFormattedHeader() const;
};

std::string getCurrentUTCDate();


#include "mrp.hpp"

#include <string>
#include <chrono>
#include <format>

MRPHeader::MRPHeader(const std::string& client_name, const std::string& utc_date, const std::string& message_data)
    : clientName(client_name)
    , utcDate(utc_date)
    , messageData(message_data + "\n")
{}

std::string MRPHeader::getFormattedHeader() const {
    std::string formattedHeader = "";

    formattedHeader += clientName + "|";
    formattedHeader += utcDate + "|";
    formattedHeader += messageData + "\n";

    return formattedHeader;
}

std::string getCurrentUTCDate() {
    std::chrono::utc_time<std::chrono::system_clock::duration> utcNow = std::chrono::utc_clock::now();
    std::string utcString = std::format("{:%FT%T}Z", utcNow);

    return utcString;
}


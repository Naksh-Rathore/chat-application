#include "mrp.hpp"

#include <string>
#include <chrono>
#include <format>

MRPHeader::MRPHeader(const std::string& client_name, const std::string& utc_date, const std::string& message_data)
    : clientName(client_name)
    , utcDate(utc_date)
    , messageData(message_data)
{}

MRPHeader::MRPHeader(const std::string& formattedHeader) 
    : clientName(formattedHeader.substr(0, formattedHeader.find('|')))
{
    std::string formattedHeaderCopy = formattedHeader;
    formattedHeaderCopy.erase(0, formattedHeaderCopy.find('|') + 1);

    utcDate = formattedHeaderCopy.substr(0, formattedHeaderCopy.find('|'));
    formattedHeaderCopy.erase(0, formattedHeaderCopy.find('|') + 1);
    
    messageData = formattedHeaderCopy.substr(0, formattedHeaderCopy.find('\n'));
}


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


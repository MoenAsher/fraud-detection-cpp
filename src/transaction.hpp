#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <cctype>

// Structure to represent a single financial transaction
struct Transaction {
    std::string transaction_id;
    std::string timestamp;
    std::string sender_account;
    std::string receiver_account;
    double amount;
    std::string transaction_type;
    std::string merchant_category;
    std::string location;
    std::string device_used;
    std::string is_fraud;
    std::string fraud_type;
    std::string time_since_last_transaction;
    std::string spending_deviation;
    std::string velocity_score;
    std::string geo_anomaly;
    std::string payment_channel;
    std::string ip_address;
    std::string device_hash;
};

// Utility: convert string to lowercase for case-insensitive comparison
inline std::string toLower(const std::string& str) {
    std::string result = "";
    for (char c : str) {
        result += std::tolower(c);
    }
    return result;
}

// Utility: trim whitespace, quotes, and newline characters
inline std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n\"");
    size_t end = s.find_last_not_of(" \t\r\n\"");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

#endif // TRANSACTION_HPP 
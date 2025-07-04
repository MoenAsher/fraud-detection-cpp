#include "array_store.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

// Helper function to parse a CSV line into a Transaction
Transaction parseTransaction(const std::string& line) {
    Transaction t;
    std::stringstream ss(line);
    std::string item;
    // Parse each field (assumes no commas in fields)
    getline(ss, t.transaction_id, ',');
    getline(ss, t.timestamp, ',');
    getline(ss, t.sender_account, ',');
    getline(ss, t.receiver_account, ',');
    getline(ss, item, ','); t.amount = std::stod(item);
    getline(ss, t.transaction_type, ',');
    getline(ss, t.merchant_category, ',');
    getline(ss, t.location, ',');
    getline(ss, t.device_used, ',');
    getline(ss, t.is_fraud, ',');
    getline(ss, t.fraud_type, ',');
    getline(ss, t.time_since_last_transaction, ',');
    getline(ss, t.spending_deviation, ',');
    getline(ss, t.velocity_score, ',');
    getline(ss, t.geo_anomaly, ',');
    getline(ss, t.payment_channel, ',');
    getline(ss, t.ip_address, ',');
    getline(ss, t.device_hash, ',');
    return t;
}

int main() {
    ArrayStore store(10000);
    std::ifstream file("data/financial_fraud_detection_dataset.csv");
    if (!file) {
        std::cout << "Could not open CSV file!" << std::endl;
        return 1;
    }
    std::string line;
    // Skip header
    getline(file, line);
    int max_to_load = 10000; // Limit for testing
    int count = 0;
    // Read each transaction
    while (getline(file, line)) {
        if (line.empty()) continue;
        Transaction t = parseTransaction(line);
        store.addTransaction(t);
        count++;
        if (count >= max_to_load) break; // Stop after 10,000
    }
    file.close();
    std::cout << "Loaded " << store.getSize() << " transactions.\n";

    // Demonstrate grouping by payment channel
    std::string channel = "card";
    ArrayStore cardStore = store.groupByPaymentChannel(channel);
    std::cout << "\nTransactions with payment channel '" << channel << "':\n";
    cardStore.display();

    // Demonstrate sorting by location
    cardStore.sortByLocation();
    std::cout << "\nTransactions with payment channel '" << channel << "' sorted by location:\n";
    cardStore.display();

    // Demonstrate searching by transaction type
    std::string type = "withdrawal";
    ArrayStore withdrawalStore = store.searchByTransactionType(type);
    std::cout << "\nTransactions with type '" << type << "':\n";
    withdrawalStore.display();

    // Demonstrate exporting to JSON
    nlohmann::json jsonData = withdrawalStore.toJSON();
    std::ofstream jsonFile("output/withdrawal_transactions.json");
    jsonFile << jsonData.dump(4); // Pretty print with 4 spaces
    jsonFile.close();
    std::cout << "\nExported withdrawal transactions to withdrawal_transactions.json\n";

    return 0;
} 
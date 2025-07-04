#include "linked_list_store.hpp"
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
    std::cout << "=== Linked List Fraud Detection System ===\n\n";
    
    LinkedListStore store; // No size limit needed for linked list
    std::ifstream file("data/financial_fraud_detection_dataset.csv");
    
    if (!file) {
        std::cout << "Could not open CSV file!" << std::endl;
        return 1;
    }
    
    std::string line;
    // Skip header
    getline(file, line);
    
    // Load limited number of transactions for testing
    int max_to_load = 1000; // Adjust this number as needed
    int count = 0;
    
    std::cout << "Loading transactions...\n";
    while (getline(file, line) && count < max_to_load) {
        if (line.empty()) continue;
        try {
            Transaction t = parseTransaction(line);
            store.addTransaction(t);
            count++;
        } catch (const std::exception& e) {
            std::cout << "Error parsing line: " << e.what() << std::endl;
            continue;
        }
    }
    file.close();
    
    std::cout << "Loaded " << store.getSize() << " transactions.\n\n";

    // 1. Demonstrate grouping by payment channel
    std::cout << "1. Grouping by payment channel 'card':\n";
    std::string channel = "card";
    LinkedListStore cardStore = store.groupByPaymentChannel(channel);
    std::cout << "Found " << cardStore.getSize() << " card transactions.\n";
    cardStore.display();

    // 2. Demonstrate sorting by location
    std::cout << "\n2. Sorting card transactions by location:\n";
    cardStore.sortByLocation();
    cardStore.display();

    // 3. Demonstrate searching by transaction type
    std::cout << "\n3. Searching for 'withdrawal' transactions:\n";
    std::string type = "withdrawal";
    LinkedListStore withdrawalStore = store.searchByTransactionType(type);
    std::cout << "Found " << withdrawalStore.getSize() << " withdrawal transactions.\n";
    withdrawalStore.display();

    // 4. Demonstrate exporting to JSON
    std::cout << "\n4. Exporting withdrawal transactions to JSON:\n";
    nlohmann::json jsonData = withdrawalStore.toJSON();
    std::ofstream jsonFile("output/withdrawal_transactions_linkedlist.json");
    jsonFile << jsonData.dump(4); // Pretty print with 4 spaces
    jsonFile.close();
    std::cout << "Exported " << withdrawalStore.getSize() << " withdrawal transactions to withdrawal_transactions_linkedlist.json\n";

    // 5. Additional demonstration: Group by different payment channels
    std::cout << "\n5. Payment channel distribution:\n";
    std::string channels[] = {"card", "wire_transfer", "mobile_payment"};
    for (const std::string& ch : channels) {
        LinkedListStore channelStore = store.groupByPaymentChannel(ch);
        std::cout << "Channel '" << ch << "': " << channelStore.getSize() << " transactions\n";
    }

    std::cout << "\n=== Linked List Implementation Complete ===\n";
    return 0;
} 
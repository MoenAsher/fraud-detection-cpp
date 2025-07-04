#include "array_store.hpp"
#include "linked_list_store.hpp" // Include for LinkedListStore
#include "transaction.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string> // For std::string
#include <cctype> // For tolower

// Helper function to parse a CSV line into a Transaction
// This function needs to be robust to handle various CSV structures
// and map them correctly to the Transaction struct.
Transaction parseTransaction(const std::string& line) {
    std::stringstream ss(line);
    std::string field;
    Transaction t;
    int col = 0;

    // Initialize string fields to empty to prevent uninitialized data
    t.transaction_id = "";
    t.timestamp = "";
    t.sender_account = "";
    t.receiver_account = "";
    t.transaction_type = "";
    t.merchant_category = "";
    t.location = "";
    t.device_used = "";
    t.is_fraud = "";
    t.fraud_type = "";
    t.time_since_last_transaction = "";
    t.spending_deviation = "";
    t.velocity_score = "";
    t.geo_anomaly = "";
    t.payment_channel = "";
    t.ip_address = "";
    t.device_hash = "";

    while (getline(ss, field, ',')) {
        field = trim(field);
        switch (col) {
            case 0: t.transaction_id = field; break;
            case 1: t.timestamp = field; break; // Assuming timestamp is at index 1
            case 2: t.sender_account = field; break; // Assuming sender_account is at index 2
            case 3: t.receiver_account = field; break; // Assuming receiver_account is at index 3
            case 4: t.amount = std::stod(field); break;
            case 5: t.transaction_type = field; break;
            case 6: t.merchant_category = field; break; // Assuming merchant_category is at index 6
            case 7: t.location = field; break;
            case 8: t.device_used = field; break; // Assuming device_used is at index 8
            case 9: t.is_fraud = field; break;
            case 10: t.fraud_type = field; break;
            case 11: t.time_since_last_transaction = field; break; // Assuming time_since_last_transaction is at index 11
            case 12: t.spending_deviation = field; break; // Assuming spending_deviation is at index 12
            case 13: t.velocity_score = field; break; // Assuming velocity_score is at index 13
            case 14: t.geo_anomaly = field; break; // Assuming geo_anomaly is at index 14
            case 15: t.payment_channel = field; break;
            case 16: t.ip_address = field; break; // Assuming ip_address is at index 16
            case 17: t.device_hash = field; break; // Assuming device_hash is at index 17
        }
        col++;
    }
    return t;
}

// Reusable function to display fraudulent transactions for ArrayStore
void showFrauds(const ArrayStore& store) {
    std::cout << "\n--- FRAUDULENT TRANSACTIONS (ArrayStore) ---\n";
    ArrayStore fraudTransactions = store.getFraudulentTransactions();
    if (fraudTransactions.getSize() == 0) {
        std::cout << "No fraudulent transactions found in ArrayStore.\n";
    } else {
        fraudTransactions.display();
    }
}

// Reusable function to display fraudulent transactions for LinkedListStore
void showFrauds(const LinkedListStore& store) {
    std::cout << "\n--- FRAUDULENT TRANSACTIONS (LinkedListStore) ---\n";
    LinkedListStore fraudTransactions = store.getFraudulentTransactions();
    if (fraudTransactions.getSize() == 0) {
        std::cout << "No fraudulent transactions found in LinkedListStore.\n";
    } else {
        fraudTransactions.display();
    }
}

int main() {
    // ArrayStore demonstration
    ArrayStore arrayStore(10000);
    std::ifstream arrayFile("data/financial_fraud_detection_dataset.csv");
    if (!arrayFile) {
        std::cerr << "Could not open CSV file for ArrayStore!" << std::endl;
        return 1;
    }
    std::string line;
    getline(arrayFile, line); // Skip header
    int count = 0;
    int max_to_load = 10000;
    while (getline(arrayFile, line) && count < max_to_load) {
        if (line.empty()) continue;
        Transaction t = parseTransaction(line);
        arrayStore.addTransaction(t);
        count++;
    }
    arrayFile.close();
    std::cout << "Loaded " << arrayStore.getSize() << " transactions into ArrayStore.\n";

    std::cout << "\n--- ArrayStore Demonstrations ---\n";

    // Demonstrate grouping by payment channel for ArrayStore
    std::string arrayChannel = "card";
    ArrayStore arrayCardStore = arrayStore.groupByPaymentChannel(arrayChannel);
    std::cout << "\nTransactions with payment channel '" << arrayChannel << "' (ArrayStore):\n";
    arrayCardStore.display();

    // Demonstrate sorting by location for ArrayStore
    arrayCardStore.sortByLocation();
    std::cout << "\nTransactions with payment channel '" << arrayChannel << "' sorted by location (ArrayStore):\n";
    arrayCardStore.display();

    // Demonstrate searching by transaction type for ArrayStore
    std::string arrayType = "withdrawal";
    ArrayStore arrayWithdrawalStore = arrayStore.searchByTransactionType(arrayType);
    std::cout << "\nTransactions with type '" << arrayType << "' (ArrayStore):\n";
    arrayWithdrawalStore.display();

    // Demonstrate exporting to JSON for ArrayStore
    nlohmann::json arrayJsonData = arrayWithdrawalStore.toJSON();
    std::ofstream arrayJsonFile("output/withdrawal_transactions_array.json");
    arrayJsonFile << arrayJsonData.dump(4);
    arrayJsonFile.close();
    std::cout << "\nExported withdrawal transactions (ArrayStore) to output/withdrawal_transactions_array.json\n";

    // --- Linked List Store demonstration ---
    LinkedListStore linkedListStore;
    std::ifstream linkedListFile("data/financial_fraud_detection_dataset.csv");
    if (!linkedListFile) {
        std::cerr << "Could not open CSV file for LinkedListStore!" << std::endl;
        return 1;
    }
    getline(linkedListFile, line); // Skip header
    count = 0;
    while (getline(linkedListFile, line) && count < max_to_load) {
        if (line.empty()) continue;
        Transaction t = parseTransaction(line);
        linkedListStore.addTransaction(t);
        count++;
    }
    linkedListFile.close();
    std::cout << "\nLoaded " << linkedListStore.getSize() << " transactions into LinkedListStore.\n";

    std::cout << "\n--- LinkedListStore Demonstrations ---\n";

    // Demonstrate grouping by payment channel for LinkedListStore
    std::string linkedListChannel = "card";
    LinkedListStore linkedListCardStore = linkedListStore.groupByPaymentChannel(linkedListChannel);
    std::cout << "\nTransactions with payment channel '" << linkedListChannel << "' (LinkedListStore):\n";
    linkedListCardStore.display();

    // Demonstrate sorting by location for LinkedListStore
    linkedListCardStore.sortByLocation();
    std::cout << "\nTransactions with payment channel '" << linkedListChannel << "' sorted by location (LinkedListStore):\n";
    linkedListCardStore.display();

    // Demonstrate searching by transaction type for LinkedListStore
    std::string linkedListType = "transfer";
    LinkedListStore linkedListTransferStore = linkedListStore.searchByTransactionType(linkedListType);
    std::cout << "\nTransactions with type '" << linkedListType << "' (LinkedListStore):\n";
    linkedListTransferStore.display();

    // Demonstrate exporting to JSON for LinkedListStore
    nlohmann::json linkedListJsonData = linkedListTransferStore.toJSON();
    std::ofstream linkedListJsonFile("output/transfer_transactions_linkedlist.json");
    linkedListJsonFile << linkedListJsonData.dump(4);
    linkedListJsonFile.close();
    std::cout << "\nExported transfer transactions (LinkedListStore) to output/transfer_transactions_linkedlist.json\n";

    // Demonstrate showFrauds for both ArrayStore and LinkedListStore
    showFrauds(arrayStore);
    showFrauds(linkedListStore);

    return 0;
} 
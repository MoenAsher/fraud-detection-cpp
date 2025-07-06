#include "array_store.hpp"
#include "linked_list_store.hpp"
#include "transaction.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <map>
#include <vector>

// parse csv line into transaction object
Transaction parseTransaction(const std::string& line) {
    std::stringstream ss(line);
    std::string field;
    Transaction t;
    int col = 0;

    // initialize string fields
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
            case 1: t.timestamp = field; break;
            case 2: t.sender_account = field; break;
            case 3: t.receiver_account = field; break;
            case 4: t.amount = std::stod(field); break;
            case 5: t.transaction_type = field; break;
            case 6: t.merchant_category = field; break;
            case 7: t.location = field; break;
            case 8: t.device_used = field; break;
            case 9: t.is_fraud = field; break;
            case 10: t.fraud_type = field; break;
            case 11: t.time_since_last_transaction = field; break;
            case 12: t.spending_deviation = field; break;
            case 13: t.velocity_score = field; break;
            case 14: t.geo_anomaly = field; break;
            case 15: t.payment_channel = field; break;
            case 16: t.ip_address = field; break;
            case 17: t.device_hash = field; break;
        }
        col++;
    }
    return t;
}

// group transactions by payment channel
void demonstratePaymentChannelGrouping(const ArrayStore& arrayStore, const LinkedListStore& linkedListStore) {
    std::cout << "\n=== FUNCTION 1: GROUPING BY PAYMENT CHANNEL ===\n";
    
    std::vector<std::string> channels = {"card", "wire_transfer", "mobile_payment", "online_banking"};
    
    std::cout << "\n--- Array Implementation ---\n";
    for (const std::string& channel : channels) {
        ArrayStore channelStore = arrayStore.groupByPaymentChannel(channel);
        std::cout << "Channel '" << channel << "': " << channelStore.getSize() << " transactions\n";
        if (channelStore.getSize() > 0) {
            std::cout << "Sample transactions:\n";
            channelStore.display();
        }
    }
    
    std::cout << "\n--- Linked List Implementation ---\n";
    for (const std::string& channel : channels) {
        LinkedListStore channelStore = linkedListStore.groupByPaymentChannel(channel);
        std::cout << "Channel '" << channel << "': " << channelStore.getSize() << " transactions\n";
        if (channelStore.getSize() > 0) {
            std::cout << "Sample transactions:\n";
            channelStore.display();
        }
    }
}

// sort transactions by location
void demonstrateLocationSorting(ArrayStore& arrayStore, LinkedListStore& linkedListStore) {
    std::cout << "\n=== FUNCTION 2: SORTING BY LOCATION ===\n";
    
    // create copies for sorting demonstration
    ArrayStore arrayCopy = arrayStore;
    LinkedListStore linkedListCopy = linkedListStore;
    
    std::cout << "\n--- Array Implementation (Merge Sort) ---\n";
    std::cout << "Before sorting:\n";
    arrayCopy.display();
    
    arrayCopy.sortByLocation();
    std::cout << "After sorting by location:\n";
    arrayCopy.display();
    
    std::cout << "\n--- Linked List Implementation (Merge Sort) ---\n";
    std::cout << "Before sorting:\n";
    linkedListCopy.display();
    
    linkedListCopy.sortByLocation();
    std::cout << "After sorting by location:\n";
    linkedListCopy.display();
}

// search transactions by type
void demonstrateTransactionTypeSearch(const ArrayStore& arrayStore, const LinkedListStore& linkedListStore) {
    std::cout << "\n=== FUNCTION 3: SEARCHING BY TRANSACTION TYPE ===\n";
    
    std::vector<std::string> transactionTypes = {"withdrawal", "transfer", "payment", "deposit"};
    
    std::cout << "\n--- Array Implementation (Linear Search) ---\n";
    for (const std::string& type : transactionTypes) {
        ArrayStore typeStore = arrayStore.searchByTransactionType(type);
        std::cout << "Transaction type '" << type << "': " << typeStore.getSize() << " transactions\n";
        if (typeStore.getSize() > 0) {
            std::cout << "Sample transactions:\n";
            typeStore.display();
        }
    }
    
    std::cout << "\n--- Linked List Implementation (Linear Search) ---\n";
    for (const std::string& type : transactionTypes) {
        LinkedListStore typeStore = linkedListStore.searchByTransactionType(type);
        std::cout << "Transaction type '" << type << "': " << typeStore.getSize() << " transactions\n";
        if (typeStore.getSize() > 0) {
            std::cout << "Sample transactions:\n";
            typeStore.display();
        }
    }
}

// generate json exports
void demonstrateJSONGeneration(const ArrayStore& arrayStore, const LinkedListStore& linkedListStore) {
    std::cout << "\n=== FUNCTION 4: JSON GENERATION ===\n";
    
    // generate json for withdrawal transactions
    std::cout << "\n--- Array Implementation JSON Export ---\n";
    ArrayStore withdrawalArray = arrayStore.searchByTransactionType("withdrawal");
    nlohmann::json arrayJson = withdrawalArray.toJSON();
    std::ofstream arrayJsonFile("output/withdrawal_transactions_array.json");
    arrayJsonFile << arrayJson.dump(4);
    arrayJsonFile.close();
    std::cout << "Exported " << withdrawalArray.getSize() << " withdrawal transactions to output/withdrawal_transactions_array.json\n";
    
    // generate json for card transactions
    ArrayStore cardArray = arrayStore.groupByPaymentChannel("card");
    nlohmann::json cardArrayJson = cardArray.toJSON();
    std::ofstream cardArrayJsonFile("output/card_transactions_array.json");
    cardArrayJsonFile << cardArrayJson.dump(4);
    cardArrayJsonFile.close();
    std::cout << "Exported " << cardArray.getSize() << " card transactions to output/card_transactions_array.json\n";
    
    // generate json for all transactions
    nlohmann::json allArrayJson = arrayStore.toJSON();
    std::ofstream allArrayJsonFile("output/all_transactions_array.json");
    allArrayJsonFile << allArrayJson.dump(4);
    allArrayJsonFile.close();
    std::cout << "Exported " << arrayStore.getSize() << " all transactions to output/all_transactions_array.json\n";
    
    std::cout << "\n--- Linked List Implementation JSON Export ---\n";
    LinkedListStore withdrawalLinkedList = linkedListStore.searchByTransactionType("withdrawal");
    nlohmann::json linkedListJson = withdrawalLinkedList.toJSON();
    std::ofstream linkedListJsonFile("output/withdrawal_transactions_linkedlist.json");
    linkedListJsonFile << linkedListJson.dump(4);
    linkedListJsonFile.close();
    std::cout << "Exported " << withdrawalLinkedList.getSize() << " withdrawal transactions to output/withdrawal_transactions_linkedlist.json\n";
    
    // generate json for card transactions
    LinkedListStore cardLinkedList = linkedListStore.groupByPaymentChannel("card");
    nlohmann::json cardLinkedListJson = cardLinkedList.toJSON();
    std::ofstream cardLinkedListJsonFile("output/card_transactions_linkedlist.json");
    cardLinkedListJsonFile << cardLinkedListJson.dump(4);
    cardLinkedListJsonFile.close();
    std::cout << "Exported " << cardLinkedList.getSize() << " card transactions to output/card_transactions_linkedlist.json\n";
    
    // generate json for all transactions
    nlohmann::json allLinkedListJson = linkedListStore.toJSON();
    std::ofstream allLinkedListJsonFile("output/all_transactions_linkedlist.json");
    allLinkedListJsonFile << allLinkedListJson.dump(4);
    allLinkedListJsonFile.close();
    std::cout << "Exported " << linkedListStore.getSize() << " all transactions to output/all_transactions_linkedlist.json\n";
}

// detect fraudulent transactions
void demonstrateFraudDetection(const ArrayStore& arrayStore, const LinkedListStore& linkedListStore) {
    std::cout << "\n=== FUNCTION 5: FRAUD DETECTION ===\n";
    
    // debug fraud values
    arrayStore.debugFraudValues();
    
    ArrayStore fraudArray = arrayStore.getFraudulentTransactions();
    LinkedListStore fraudLinkedList = linkedListStore.getFraudulentTransactions();
    
    std::cout << "\n--- Array Implementation ---\n";
    std::cout << "Found " << fraudArray.getSize() << " fraudulent transactions\n";
    if (fraudArray.getSize() > 0) {
        fraudArray.display();
    } else {
        std::cout << "No fraudulent transactions found in Array implementation.\n";
        std::cout << "This could mean:\n";
        std::cout << "1. No fraud in the dataset\n";
        std::cout << "2. Fraud flag uses different values (e.g., '1'/'0' instead of 'true'/'false')\n";
        std::cout << "3. Fraud flag is in a different column\n";
        std::cout << "4. CSV column mapping is incorrect\n";
    }
    
    std::cout << "\n--- Linked List Implementation ---\n";
    std::cout << "Found " << fraudLinkedList.getSize() << " fraudulent transactions\n";
    if (fraudLinkedList.getSize() > 0) {
        fraudLinkedList.display();
    } else {
        std::cout << "No fraudulent transactions found in Linked List implementation.\n";
    }
}

// run all functions
void demonstrateAllFunctions(ArrayStore& arrayStore, LinkedListStore& linkedListStore) {
    std::cout << "\n=== RUNNING ALL FUNCTIONS ===\n";
    demonstratePaymentChannelGrouping(arrayStore, linkedListStore);
    demonstrateLocationSorting(arrayStore, linkedListStore);
    demonstrateTransactionTypeSearch(arrayStore, linkedListStore);
    demonstrateJSONGeneration(arrayStore, linkedListStore);
    demonstrateFraudDetection(arrayStore, linkedListStore);
}

// check csv header and column mapping
void checkCSVHeader() {
    std::cout << "\n=== FUNCTION 7: CSV HEADER CHECK ===\n";
    
    std::ifstream file("data/financial_fraud_detection_dataset.csv");
    if (!file) {
        std::cerr << "Could not open CSV file!" << std::endl;
        return;
    }
    
    std::string header;
    getline(file, header);
    file.close();
    
    std::cout << "CSV Header: " << header << "\n\n";
    
    std::stringstream ss(header);
    std::string column;
    int colIndex = 0;
    
    std::cout << "Column mapping:\n";
    while (getline(ss, column, ',')) {
        column = trim(column);
        std::cout << "Column " << colIndex << " (" << (char)('A' + colIndex) << "): " << column << "\n";
        colIndex++;
    }
    
    std::cout << "\nCurrent mapping in code:\n";
    std::cout << "Column 9 (J): is_fraud\n";
    std::cout << "Column 10 (K): fraud_type\n";
}

// show fraud statistics
void showFraudStatistics(const ArrayStore& arrayStore, const LinkedListStore& linkedListStore) {
    std::cout << "\n=== FUNCTION 8: FRAUD STATISTICS ===\n";
    
    int totalTransactions = arrayStore.getSize();
    ArrayStore fraudArray = arrayStore.getFraudulentTransactions();
    int fraudCount = fraudArray.getSize();
    
    std::cout << "Total transactions loaded: " << totalTransactions << "\n";
    std::cout << "Fraudulent transactions: " << fraudCount << "\n";
    std::cout << "Legitimate transactions: " << (totalTransactions - fraudCount) << "\n";
    
    if (totalTransactions > 0) {
        double fraudPercentage = (double)fraudCount / totalTransactions * 100.0;
        std::cout << "Fraud rate: " << fraudPercentage << "%\n";
    }
    
    std::cout << "\n--- Sample of fraudulent transactions ---\n";
    if (fraudCount > 0) {
        fraudArray.display();
    } else {
        std::cout << "No fraudulent transactions found in this dataset.\n";
    }
}

// display main menu
void displayMenu() {
    std::cout << "\n=== FRAUD DETECTION SYSTEM MENU ===\n";
    std::cout << "1. Group transactions by payment channel\n";
    std::cout << "2. Sort transactions by location\n";
    std::cout << "3. Search transactions by type\n";
    std::cout << "4. Generate JSON exports\n";
    std::cout << "5. Detect fraudulent transactions\n";
    std::cout << "6. Run all functions\n";
    std::cout << "7. Check CSV header and column mapping\n";
    std::cout << "8. Show fraud statistics\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice (0-8): ";
}

// load data from csv file with chunk selection
bool loadData(ArrayStore& arrayStore, LinkedListStore& linkedListStore) {
    std::cout << "Loading transaction data...\n";
    
    // chunk selection menu
    std::cout << "\n=== DATA LOADING OPTIONS ===\n";
    std::cout << "1. Quick test (1,000 rows)\n";
    std::cout << "2. Small sample (10,000 rows)\n";
    std::cout << "3. Medium sample (100,000 rows)\n";
    std::cout << "4. Large sample (500,000 rows)\n";
    std::cout << "5. Full dataset (~5,000,000 rows) - SLOW\n";
    std::cout << "6. Custom number of rows\n";
    std::cout << "Enter your choice (1-6): ";
    
    int choice;
    std::cin >> choice;
    
    int max_to_load = -1; // -1 means load all
    
    switch (choice) {
        case 1: max_to_load = 1000; break;
        case 2: max_to_load = 10000; break;
        case 3: max_to_load = 100000; break;
        case 4: max_to_load = 500000; break;
        case 5: max_to_load = -1; break; // load all
        case 6: 
            std::cout << "Enter number of rows to load: ";
            std::cin >> max_to_load;
            break;
        default:
            std::cout << "Invalid choice, using quick test (1,000 rows)\n";
            max_to_load = 1000;
            break;
    }
    
    std::cout << "\nLoading " << (max_to_load == -1 ? "ALL" : std::to_string(max_to_load)) << " rows...\n";
    
    // load data from csv file
    std::ifstream file("data/financial_fraud_detection_dataset.csv");
    if (!file) {
        std::cerr << "Could not open CSV file! Please ensure 'data/financial_fraud_detection_dataset.csv' exists.\n";
        return false;
    }
    
    std::string line;
    getline(file, line); // skip header
    int count = 0;
    
    while (getline(file, line) && (max_to_load == -1 || count < max_to_load)) {
        if (line.empty()) continue;
        try {
            Transaction t = parseTransaction(line);
            arrayStore.addTransaction(t);
            linkedListStore.addTransaction(t);
            count++;
            
            // progress indicator for large loads
            if (count % 10000 == 0) {
                std::cout << "Loaded " << count << " rows...\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error parsing line: " << e.what() << std::endl;
            continue;
        }
    }
    file.close();
    
    std::cout << "Loaded " << arrayStore.getSize() << " transactions into both data structures.\n";
    // print number of frauds found right after loading
    ArrayStore fraudArray = arrayStore.getFraudulentTransactions();
    std::cout << "Immediately after loading: Found " << fraudArray.getSize() << " fraudulent transactions in ArrayStore.\n";
    return true;
}

// main program with interactive menu
void runMainProgram() {
    std::cout << "=== FRAUD DETECTION SYSTEM - MAIN PROGRAM ===\n";
    
    // initialize data structures
    ArrayStore arrayStore(10000);
    LinkedListStore linkedListStore;
    
    // load data
    if (!loadData(arrayStore, linkedListStore)) {
        std::cout << "Failed to load data. Exiting...\n";
        return;
    }
    
    int choice;
    do {
        displayMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                demonstratePaymentChannelGrouping(arrayStore, linkedListStore);
                break;
            case 2:
                demonstrateLocationSorting(arrayStore, linkedListStore);
                break;
            case 3:
                demonstrateTransactionTypeSearch(arrayStore, linkedListStore);
                break;
            case 4:
                demonstrateJSONGeneration(arrayStore, linkedListStore);
                break;
            case 5:
                demonstrateFraudDetection(arrayStore, linkedListStore);
                break;
            case 6:
                demonstrateAllFunctions(arrayStore, linkedListStore);
                break;
            case 7:
                checkCSVHeader();
                break;
            case 8:
                showFraudStatistics(arrayStore, linkedListStore);
                break;
            case 0:
                std::cout << "Exiting program...\n";
                break;
            default:
                std::cout << "Invalid choice! Please enter a number between 0 and 8.\n";
                break;
        }
        
        if (choice != 0) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
        
    } while (choice != 0);
    
    std::cout << "=== PROGRAM ENDED ===\n";
}

int main() {
    runMainProgram();
    return 0;
} 
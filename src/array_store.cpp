// Initial commit message: I added this after finishing the array data structure
#include "array_store.hpp"
#include "transaction.hpp"
#include <iostream> // For display

// Constructor: initializes the array with a given maximum size
ArrayStore::ArrayStore(int max_size) {
    capacity = max_size;
    size = 0;
    transactions = new Transaction[capacity]; // Dynamically allocate array
}

// Destructor: releases the memory used by the array
ArrayStore::~ArrayStore() {
    delete[] transactions;
}

// Adds a transaction to the array
void ArrayStore::addTransaction(const Transaction& t) {
    if (size >= capacity) {
        // Double the capacity
        int new_capacity = capacity * 2;
        if (new_capacity == 0) new_capacity = 1; // Handle initial case
        Transaction* new_transactions = new Transaction[new_capacity];
        // Copy old data
        for (int i = 0; i < size; ++i) {
            new_transactions[i] = transactions[i];
        }
        // Delete old array and update pointer/capacity
        delete[] transactions;
        transactions = new_transactions;
        capacity = new_capacity;
    }
    transactions[size] = t;
    size++;
}

// Returns the current number of transactions in the array
int ArrayStore::getSize() const {
    return size;
}

// Displays all transactions in the array to the console
void ArrayStore::display() const {
    std::cout << "\n--- Transactions ---\n";
    for (int i = 0; i < size; ++i) {
        const Transaction& t = transactions[i];
        std::cout << "ID: " << t.transaction_id
                  << ", Date: " << t.timestamp
                  << ", Amount: " << t.amount
                  << ", Type: " << t.transaction_type
                  << ", Location: " << t.location
                  << ", Channel: " << t.payment_channel
                  << std::endl;
    }
    std::cout << "-------------------\n";
}

// Groups transactions by payment channel (returns a new ArrayStore)
ArrayStore ArrayStore::groupByPaymentChannel(const std::string& channel) const {
    ArrayStore grouped(capacity); // Create a new ArrayStore with the same capacity
    for (int i = 0; i < size; ++i) {
        if (transactions[i].payment_channel == channel) {
            grouped.addTransaction(transactions[i]);
        }
    }
    return grouped;
}

// Helper function to merge two sorted halves
void merge(Transaction* arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Transaction* L = new Transaction[n1];
    Transaction* R = new Transaction[n2];
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].location <= R[j].location) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++; k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++; k++;
    }
    delete[] L;
    delete[] R;
}

// Recursive merge sort function
void mergeSort(Transaction* arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Sorts transactions by location in ascending order
void ArrayStore::sortByLocation() {
    // Use merge sort to sort the transactions array by location
    if (size > 1) {
        mergeSort(transactions, 0, size - 1);
    }
}

// Searches for transactions by type (returns a new ArrayStore)
ArrayStore ArrayStore::searchByTransactionType(const std::string& type) const {
    ArrayStore found(capacity); // Create a new ArrayStore with the same capacity
    for (int i = 0; i < size; ++i) {
        if (transactions[i].transaction_type == type) {
            found.addTransaction(transactions[i]);
        }
    }
    return found;
}

// Exports transactions to JSON format
nlohmann::json ArrayStore::toJSON() const {
    nlohmann::json j_array = nlohmann::json::array(); // Create a JSON array
    for (int i = 0; i < size; ++i) {
        const Transaction& t = transactions[i];
        nlohmann::json j_trans = {
            {"transaction_id", t.transaction_id},
            {"timestamp", t.timestamp},
            {"sender_account", t.sender_account},
            {"receiver_account", t.receiver_account},
            {"amount", t.amount},
            {"transaction_type", t.transaction_type},
            {"merchant_category", t.merchant_category},
            {"location", t.location},
            {"device_used", t.device_used},
            {"is_fraud", t.is_fraud},
            {"fraud_type", t.fraud_type},
            {"time_since_last_transaction", t.time_since_last_transaction},
            {"spending_deviation", t.spending_deviation},
            {"velocity_score", t.velocity_score},
            {"geo_anomaly", t.geo_anomaly},
            {"payment_channel", t.payment_channel},
            {"ip_address", t.ip_address},
            {"device_hash", t.device_hash}
        };
        j_array.push_back(j_trans); // Add transaction to JSON array
    }
    return j_array; // Return the JSON array
}

// Gets all fraudulent transactions
ArrayStore ArrayStore::getFraudulentTransactions() const {
    ArrayStore fraudulent(capacity); // Create a new ArrayStore with the same capacity
    for (int i = 0; i < size; ++i) {
        // Assuming 'is_fraud' is stored as a string like "TRUE" or "FALSE"
        // and we need case-insensitive comparison.
        // Using toLower for robustness.
        if (toLower(transactions[i].is_fraud) == "true") {
            fraudulent.addTransaction(transactions[i]);
        }
    }
    return fraudulent;
} 
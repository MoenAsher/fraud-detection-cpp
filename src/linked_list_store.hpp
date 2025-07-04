#ifndef LINKED_LIST_STORE_HPP
#define LINKED_LIST_STORE_HPP

#include <string>
#include "../lib/json.hpp" // For JSON export

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

// Node structure for the linked list
struct Node {
    Transaction data;
    Node* next;
    
    Node(const Transaction& t) : data(t), next(nullptr) {}
};

// Linked list-based class to store and manage transactions
class LinkedListStore {
private:
    Node* head;  // Pointer to the first node
    int size;    // Number of transactions

public:
    // Constructor and destructor
    LinkedListStore();
    ~LinkedListStore();
    
    // Copy constructor and assignment operator
    LinkedListStore(const LinkedListStore& other);
    LinkedListStore& operator=(const LinkedListStore& other);

    // Add a transaction to the linked list
    void addTransaction(const Transaction& t);

    // Group transactions by payment channel (returns a new LinkedListStore)
    LinkedListStore groupByPaymentChannel(const std::string& channel) const;

    // Sort transactions by location (ascending) using merge sort
    void sortByLocation();

    // Search for transactions by type (returns a new LinkedListStore)
    LinkedListStore searchByTransactionType(const std::string& type) const;

    // Export transactions to JSON
    nlohmann::json toJSON() const;

    // Display transactions to console
    void display() const;

    // Get the number of transactions
    int getSize() const;

private:
    // Helper methods for merge sort
    Node* mergeSort(Node* head);
    Node* merge(Node* left, Node* right);
    Node* getMiddle(Node* head);
    Node* copyList(Node* head) const;
    void deleteList(Node* head);
};

#endif // LINKED_LIST_STORE_HPP 
#ifndef LINKED_LIST_STORE_HPP
#define LINKED_LIST_STORE_HPP

#include <string>
#include "../lib/json.hpp" // For JSON export
#include "transaction.hpp" // Use shared Transaction struct and utilities

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

    // Get fraudulent transactions
    LinkedListStore getFraudulentTransactions() const;

private:
    // Helper methods for merge sort
    Node* mergeSort(Node* head);
    Node* merge(Node* left, Node* right);
    Node* getMiddle(Node* head);
    Node* copyList(Node* head) const;
    void deleteList(Node* head);
};

#endif // LINKED_LIST_STORE_HPP 
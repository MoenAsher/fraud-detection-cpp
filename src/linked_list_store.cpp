#include "linked_list_store.hpp"
#include "transaction.hpp"
#include <iostream>

// constructor: initialize empty linked list
LinkedListStore::LinkedListStore() {
    head = nullptr;
    size = 0;
}

// destructor: release all memory
LinkedListStore::~LinkedListStore() {
    deleteList(head);
}

// copy constructor: create deep copy
LinkedListStore::LinkedListStore(const LinkedListStore& other) {
    head = copyList(other.head);
    size = other.size;
}

// assignment operator: create deep copy
LinkedListStore& LinkedListStore::operator=(const LinkedListStore& other) {
    if (this != &other) {
        deleteList(head);
        head = copyList(other.head);
        size = other.size;
    }
    return *this;
}

// helper method to delete all nodes
void LinkedListStore::deleteList(Node* head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// helper method to create deep copy
Node* LinkedListStore::copyList(Node* head) const {
    if (head == nullptr) return nullptr;
    
    Node* newHead = new Node(head->data);
    Node* current = newHead;
    head = head->next;
    
    while (head != nullptr) {
        current->next = new Node(head->data);
        current = current->next;
        head = head->next;
    }
    
    return newHead;
}

// add transaction to end of linked list
void LinkedListStore::addTransaction(const Transaction& t) {
    Node* newNode = new Node(t);
    
    if (head == nullptr) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    size++;
}

// return current number of transactions
int LinkedListStore::getSize() const {
    return size;
}

// display all transactions to console
void LinkedListStore::display() const {
    std::cout << "\n--- Transactions (Linked List) ---\n";
    Node* current = head;
    int count = 0;
    
    // show first 10 transactions
    while (current != nullptr && count < 10) {
        const Transaction& t = current->data;
        std::cout << "ID: " << t.transaction_id
                  << ", Date: " << t.timestamp
                  << ", Amount: " << t.amount
                  << ", Type: " << t.transaction_type
                  << ", Location: " << t.location
                  << ", Channel: " << t.payment_channel
                  << std::endl;
        current = current->next;
        count++;
    }
    
    // if there are more transactions, ask user if they want to see all
    if (size > 10) {
        std::cout << "... and " << (size - 10) << " more transactions\n";
        std::cout << "Total: " << size << " transactions\n";
        std::cout << "Show all transactions? (y/n): ";
        
        char choice;
        std::cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            std::cout << "\n--- ALL TRANSACTIONS (Linked List) ---\n";
            current = head;
            count = 0;
            while (current != nullptr) {
                const Transaction& t = current->data;
                std::cout << "ID: " << t.transaction_id
                          << ", Date: " << t.timestamp
                          << ", Amount: " << t.amount
                          << ", Type: " << t.transaction_type
                          << ", Location: " << t.location
                          << ", Channel: " << t.payment_channel
                          << std::endl;
                current = current->next;
                count++;
            }
            std::cout << "Total: " << size << " transactions\n";
        }
    } else {
        std::cout << "Total: " << size << " transactions\n";
    }
    
    std::cout << "--------------------------------\n";
}

// group transactions by payment channel
LinkedListStore LinkedListStore::groupByPaymentChannel(const std::string& channel) const {
    LinkedListStore grouped;
    Node* current = head;
    
    while (current != nullptr) {
        if (current->data.payment_channel == channel) {
            grouped.addTransaction(current->data);
        }
        current = current->next;
    }
    
    return grouped;
}

// search for transactions by type
LinkedListStore LinkedListStore::searchByTransactionType(const std::string& type) const {
    LinkedListStore found;
    Node* current = head;
    
    while (current != nullptr) {
        if (current->data.transaction_type == type) {
            found.addTransaction(current->data);
        }
        current = current->next;
    }
    
    return found;
}

// helper method to find middle of linked list
Node* LinkedListStore::getMiddle(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    Node* slow = head;
    Node* fast = head->next;
    
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

// helper method to merge two sorted linked lists
Node* LinkedListStore::merge(Node* left, Node* right) {
    if (left == nullptr) return right;
    if (right == nullptr) return left;
    
    Node* result = nullptr;
    
    if (left->data.location <= right->data.location) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }
    
    return result;
}

// recursive merge sort for linked list
Node* LinkedListStore::mergeSort(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    
    Node* middle = getMiddle(head);
    Node* nextOfMiddle = middle->next;
    middle->next = nullptr;
    
    Node* left = mergeSort(head);
    Node* right = mergeSort(nextOfMiddle);
    
    return merge(left, right);
}

// sort transactions by location using merge sort
void LinkedListStore::sortByLocation() {
    if (size > 1) {
        head = mergeSort(head);
    }
}

// export transactions to json format
nlohmann::json LinkedListStore::toJSON() const {
    nlohmann::json j_array = nlohmann::json::array();
    Node* current = head;
    
    while (current != nullptr) {
        const Transaction& t = current->data;
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
        j_array.push_back(j_trans);
        current = current->next;
    }
    
    return j_array;
}

// get all fraudulent transactions
LinkedListStore LinkedListStore::getFraudulentTransactions() const {
    LinkedListStore fraudulent;
    Node* current = head;
    while (current != nullptr) {
        std::string fraudValue = current->data.is_fraud;
        // check for true/false values (case-insensitive)
        if (toLower(fraudValue) == "true") {
            fraudulent.addTransaction(current->data);
        }
        current = current->next;
    }
    return fraudulent;
} 
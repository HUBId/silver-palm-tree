#ifndef TRANSACTION_POOL_H
#define TRANSACTION_POOL_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include <string>
#include <ctime>

// Forward declarations for Transaction and SNARK proof validation
class Transaction;
class SnarkProofValidator;

// Transaction structure to hold the data
struct Transaction {
    std::string sender;
    std::string receiver;
    uint64_t amount;
    uint64_t timestamp;      // Timestamp when the transaction was created
    std::string signature;   // Signature for the transaction
    std::vector<uint8_t> snark_proof; // SNARK proof

    // Constructor to initialize the transaction
    Transaction(std::string sender, std::string receiver, uint64_t amount, uint64_t timestamp, std::string signature, std::vector<uint8_t> snark_proof)
        : sender(sender), receiver(receiver), amount(amount), timestamp(timestamp), signature(signature), snark_proof(snark_proof) {}
};

class TransactionPool {
private:
    std::vector<Transaction> transactions;

public:
    // Function to insert a transaction into the pool
    void insertTransaction(const Transaction& tx);

    // Function to select transactions from the pool based on timestamp and validity
    std::vector<Transaction> selectTransactionsForBlock(size_t max_block_size);

    // Function to validate a transaction's SNARK proof
    bool validateTransactionProof(const Transaction& tx);

    // Function to remove a transaction from the pool after it's included in a block
    void removeTransaction(const Transaction& tx);

    // Getter for transactions
    const std::vector<Transaction>& getTransactions() const { return transactions; }
};

#endif // TRANSACTION_POOL_H

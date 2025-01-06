#include "transaction_pool.h"
#include "snark_proof_validator.h"  // Assume this is the header file for your SNARK proof validation logic

// Function to insert a transaction into the pool
void TransactionPool::insertTransaction(const Transaction& tx) {
    // Validate SNARK proof before inserting it
    if (validateTransactionProof(tx)) {
        transactions.push_back(tx);
        std::cout << "Transaction inserted: " << tx.sender << " -> " << tx.receiver << std::endl;
    } else {
        std::cout << "Invalid SNARK proof for transaction: " << tx.sender << " -> " << tx.receiver << std::endl;
    }
}

// Function to select transactions from the pool for block inclusion
std::vector<Transaction> TransactionPool::selectTransactionsForBlock(size_t max_block_size) {
    std::vector<Transaction> selected_transactions;
    size_t total_size = 0;

    // Sort transactions by timestamp (oldest first) to prioritize them
    std::sort(transactions.begin(), transactions.end(), [](const Transaction& a, const Transaction& b) {
        return a.timestamp < b.timestamp;
    });

    // Select transactions until the block size is filled
    for (const Transaction& tx : transactions) {
        if (total_size + sizeof(tx) > max_block_size) {
            break;
        }
        selected_transactions.push_back(tx);
        total_size += sizeof(tx);
    }

    return selected_transactions;
}

// Function to validate a transaction's SNARK proof (using the SnarkProofValidator)
bool TransactionPool::validateTransactionProof(const Transaction& tx) {
    // Here you would use the actual SNARK proof validator, which is a separate class.
    SnarkProofValidator proofValidator;
    return proofValidator.validateProof(tx.snark_proof);
}

// Function to remove a transaction from the pool after it has been included in a block
void TransactionPool::removeTransaction(const Transaction& tx) {
    transactions.erase(std::remove(transactions.begin(), transactions.end(), tx), transactions.end());
    std::cout << "Transaction removed from pool: " << tx.sender << " -> " << tx.receiver << std::endl;
}

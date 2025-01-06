#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <memory>
#include <ctime>

// Forward declaration of Transaction class for the blockchain
class Transaction;

// Structure for Block to hold data and metadata
class Block {
public:
    uint64_t index;                  // Block index in the chain
    std::string previous_hash;       // Hash of the previous block
    std::string block_hash;          // Hash of the current block
    uint64_t timestamp;              // Time the block was created
    std::vector<std::shared_ptr<Transaction>> transactions;  // List of transactions in the block
    std::string block_proposer;      // The node that proposed this block
    bool is_valid;                   // Validation status of the block (part of BFT)
    
    // Constructor for Block
    Block(uint64_t idx, std::string prev_hash, std::vector<std::shared_ptr<Transaction>> txns, std::string proposer) :
        index(idx), previous_hash(prev_hash), timestamp(std::time(nullptr)), transactions(txns), block_proposer(proposer), is_valid(true) {
            this->block_hash = compute_hash();
        }

    // Function to calculate the block's hash based on its data
    std::string compute_hash() {
        std::string data = std::to_string(index) + previous_hash + std::to_string(timestamp) + block_proposer;
        for (const auto& txn : transactions) {
            data += txn->get_transaction_data();
        }
        return std::to_string(std::hash<std::string>{}(data));
    }

    // Get the block hash
    std::string get_block_hash() const {
        return block_hash;
    }
};

// Transaction class for representing a transaction in the blockchain
class Transaction {
public:
    std::string sender;
    std::string receiver;
    double amount;
    uint64_t timestamp;  // Timestamp for when the transaction was created
    std::string snark_proof;  // Placeholder for SNARK proof

    Transaction(std::string s, std::string r, double amt, std::string proof)
        : sender(s), receiver(r), amount(amt), timestamp(std::time(nullptr)), snark_proof(proof) {}

    // Function to get the transaction data (used for block hash computation)
    std::string get_transaction_data() const {
        return sender + receiver + std::to_string(amount) + std::to_string(timestamp);
    }
};

// Blockchain class to represent the chain and its operations
class Blockchain {
public:
    uint64_t difficulty;  // Difficulty level for block hash (e.g., PoW)
    uint64_t max_block_size;  // Max size for a block (in bytes)
    uint64_t max_chain_size;  // Max number of blocks the blockchain can hold

    std::vector<std::shared_ptr<Block>> chain;   // The blockchain (list of blocks)
    std::unordered_map<std::string, std::shared_ptr<Block>> block_map;  // Map for quick lookup of blocks by hash

    // Constructor for the Blockchain, sets up the genesis block
    Blockchain(uint64_t max_size = 1024 * 1024 * 1, uint64_t max_chain = 1000)
        : max_block_size(max_size), max_chain_size(max_chain), difficulty(1) {
        // Initialize the blockchain with a genesis block
        std::shared_ptr<Block> genesis = create_genesis_block();
        chain.push_back(genesis);
        block_map[genesis->get_block_hash()] = genesis;
    }

    // Create and return the Genesis block
    std::shared_ptr<Block> create_genesis_block() {
        std::vector<std::shared_ptr<Transaction>> empty_transactions;
        return std::make_shared<Block>(0, "0", empty_transactions, "genesis_proposer");
    }

    // Add a new block to the chain
    bool add_block(std::shared_ptr<Block> block) {
        if (block->index != chain.size()) {
            std::cerr << "Invalid block index!" << std::endl;
            return false;
        }

        // Check if the block size exceeds the maximum limit
        uint64_t block_size = sizeof(*block);
        for (const auto& txn : block->transactions) {
            block_size += sizeof(*txn);
        }

        if (block_size > max_block_size) {
            std::cerr << "Block size exceeds maximum allowed size!" << std::endl;
            return false;
        }

        // Check if the chain has exceeded its size limit
        if (chain.size() > max_chain_size) {
            std::cerr << "Blockchain exceeded maximum allowed size!" << std::endl;
            prune_blockchain();
        }

        // Add the block to the blockchain
        chain.push_back(block);
        block_map[block->get_block_hash()] = block;
        return true;
    }

    // Function to get the latest block
    std::shared_ptr<Block> get_latest_block() const {
        return chain.back();
    }

    // Function to prune the blockchain (remove the oldest block)
    void prune_blockchain() {
        if (chain.size() > max_chain_size) {
            // Remove the oldest block from the blockchain
            std::shared_ptr<Block> block_to_remove = chain.front();
            chain.erase(chain.begin());
            block_map.erase(block_to_remove->get_block_hash());
        }
    }

    // Function to validate the blockchain
    bool validate_blockchain() {
        for (size_t i = 1; i < chain.size(); i++) {
            std::shared_ptr<Block> current_block = chain[i];
            std::shared_ptr<Block> previous_block = chain[i - 1];

            // Check the hash of the current block to ensure it matches
            if (current_block->previous_hash != previous_block->get_block_hash()) {
                std::cerr << "Blockchain validation failed: invalid previous hash!" << std::endl;
                return false;
            }

            // Validate the block hash matches
            if (current_block->get_block_hash() != current_block->compute_hash()) {
                std::cerr << "Blockchain validation failed: invalid block hash!" << std::endl;
                return false;
            }
        }
        return true;
    }
};

#endif // BLOCKCHAIN_H

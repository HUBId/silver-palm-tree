#include "blockchain.h"
#include "compression.h"
#include "transaction.h"
#include "snark_proof.h"
#include <iostream>
#include <vector>
#include <ctime>

// Blockchain class constructor
Blockchain::Blockchain() {
    // Create the genesis block and initialize the blockchain
    Block genesis = createGenesisBlock();
    chain.push_back(genesis);
    std::cout << "Genesis block created with hash: " << genesis.getHash() << std::endl;
}

// Create the genesis block with predefined values
Block Blockchain::createGenesisBlock() {
    Transaction genesisTransaction("0", "0", 0, "genesis block");
    Block genesisBlock(0, genesisTransaction);
    return genesisBlock;
}

// Add a new block to the blockchain
void Blockchain::addBlock(const Block& newBlock) {
    // Validate the block before adding
    if (isValidNewBlock(newBlock, chain.back())) {
        chain.push_back(newBlock);
        std::cout << "Block added with hash: " << newBlock.getHash() << std::endl;
    } else {
        std::cout << "Block validation failed!" << std::endl;
    }
}

// Validate the block using its previous block's hash and other properties
bool Blockchain::isValidNewBlock(const Block& newBlock, const Block& previousBlock) {
    if (newBlock.getPreviousHash() != previousBlock.getHash()) {
        std::cout << "Invalid block: Previous hash mismatch." << std::endl;
        return false;
    }

    if (!isValidProofOfWork(newBlock)) {
        std::cout << "Invalid block: Proof of work invalid." << std::endl;
        return false;
    }

    if (!isValidTransaction(newBlock.getTransactions())) {
        std::cout << "Invalid block: Transactions are invalid." << std::endl;
        return false;
    }

    return true;
}

// Check the validity of transactions in the block
bool Blockchain::isValidTransaction(const std::vector<Transaction>& transactions) {
    for (const auto& tx : transactions) {
        if (!tx.isValid()) {
            return false;
        }
    }
    return true;
}

// Placeholder for proof of work validation (simplified)
bool Blockchain::isValidProofOfWork(const Block& block) {
    // Here you can implement a proof of work mechanism (for example, mining difficulty).
    // This is a placeholder; it could be a hash matching a pattern or any other logic.
    return block.getHash().substr(0, 4) == "0000";  // Simple example (difficulty of 4 leading zeros)
}

// Compress the blockchain using recursive compression
void Blockchain::compressBlockchain() {
    // Iterate through each block and apply compression
    for (size_t i = 0; i < chain.size(); ++i) {
        compressBlock(chain[i]);
    }
}

// Recursive compression for each block using Compcrypt
void Blockchain::compressBlock(Block& block) {
    // Applying the compression algorithm (pseudo-code)
    std::string blockData = block.serialize();
    std::string compressedData = Compcrypt::compress(blockData);
    block.setCompressedData(compressedData);
    std::cout << "Block " << block.getIndex() << " compressed." << std::endl;
}

// Get the latest block in the blockchain
Block Blockchain::getLatestBlock() {
    return chain.back();
}

// Retrieve the entire blockchain
std::vector<Block> Blockchain::getChain() {
    return chain;
}

// Print out the details of the blockchain
void Blockchain::printBlockchain() {
    for (const auto& block : chain) {
        block.printBlock();
    }
}

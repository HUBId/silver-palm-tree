#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

#include "blockchain.h"
#include "wallet.h"
#include "transaction.h"
#include "networking.h"
#include "consensus.h"
#include "compression.h"
#include "snark_proof.h"

namespace prunet {

void initializeBlockchain() {
    std::cout << "Initializing Blockchain...\n";
    // Initialize blockchain with genesis block and other settings
    Blockchain::getInstance().initialize();
}

void createWallet() {
    std::cout << "Creating Wallet...\n";
    // Generate a random wallet and display the address and private key
    Wallet wallet;
    std::cout << "Wallet Created:\n";
    std::cout << "Address: " << wallet.getAddress() << "\n";
    std::cout << "Private Key: " << wallet.getPrivateKey() << "\n";

    // Simulate a transaction by signing it
    Transaction transaction(wallet.getAddress(), "recipient_address", 50);
    std::string signature = wallet.signTransaction(transaction);
    std::cout << "Transaction signed with signature: " << signature << "\n";

    // Add transaction to the pool
    TransactionPool::getInstance().addTransaction(transaction);

    // Submit the transaction to the network
    Networking::getInstance().broadcastTransaction(transaction, signature);
}

void startConsensus() {
    std::cout << "Starting Consensus Mechanism...\n";
    Consensus::getInstance().start();
}

void startNetworking() {
    std::cout << "Starting Networking Layer...\n";
    // Start Libp2p and Gossip protocol for peer discovery and message propagation
    Networking::getInstance().start();
}

void runTest() {
    std::cout << "Running Test for Max TPS...\n";
    // Run a simple test for transactions per second (TPS)
    unsigned int numTransactions = 10000;
    unsigned int tps = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < numTransactions; ++i) {
        // Create a new wallet and sign transactions
        Wallet wallet;
        Transaction transaction(wallet.getAddress(), "recipient_address", 50);
        std::string signature = wallet.signTransaction(transaction);

        // Add transaction to pool
        TransactionPool::getInstance().addTransaction(transaction);

        // Simulate broadcasting the transaction
        Networking::getInstance().broadcastTransaction(transaction, signature);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    tps = numTransactions / duration.count();

    std::cout << "Max TPS achieved: " << tps << "\n";
}

int main() {
    try {
        // Initialize the blockchain
        initializeBlockchain();

        // Create and test the wallet
        createWallet();

        // Start the consensus mechanism
        startConsensus();

        // Start the networking layer (Libp2p + Gossip Protocol)
        startNetworking();

        // Run a test for the maximum transactions per second (TPS)
        runTest();

        // Simulate the operation of adding a few blocks
        std::cout << "Simulating Block Creation...\n";
        Blockchain::getInstance().addBlock("Sample Block Data");

        // Keep the program running
        std::cout << "Blockchain system running...\n";
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
}


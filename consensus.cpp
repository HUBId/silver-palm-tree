#include "consensus.h"
#include "blockchain.h"
#include "snark_proof.h"
#include "transaction.h"
#include "networking.h"
#include "reputation_system.h"
#include <iostream>
#include <vector>
#include <ctime>

// Consensus class constructor
Consensus::Consensus(Blockchain& blockchain, Networking& network)
    : blockchain(blockchain), network(network) {
    // Initialize consensus round with empty state
    currentRound = 0;
    currentLeader = nullptr;
    std::cout << "Consensus system initialized!" << std::endl;
}

// Start a new consensus round
void Consensus::startRound() {
    currentRound++;
    std::cout << "Starting consensus round: " << currentRound << std::endl;

    // Select the leader based on the reputation system
    currentLeader = selectLeader();
    if (currentLeader == nullptr) {
        std::cout << "No leader selected, skipping consensus round." << std::endl;
        return;
    }

    std::cout << "Leader for round " << currentRound << " is: " << currentLeader->getId() << std::endl;

    // Broadcast leader to all nodes in the network
    network.broadcastLeader(currentLeader->getId());

    // Nodes validate the leader's block and SNARK proof
    Block proposedBlock = currentLeader->proposeBlock();
    if (validateBlock(proposedBlock)) {
        // If block is valid, add it to the blockchain
        blockchain.addBlock(proposedBlock);
        network.broadcastBlock(proposedBlock);
        std::cout << "Block validated and added to the blockchain." << std::endl;
    } else {
        std::cout << "Block proposal failed validation." << std::endl;
    }
}

// Select the leader based on the highest reputation and VRF selection
Node* Consensus::selectLeader() {
    // First, select nodes using VRF (Verifiable Random Function)
    std::vector<Node*> candidates = network.getNodes();
    std::vector<Node*> eligibleNodes;

    for (auto& node : candidates) {
        if (node->getReputation() > MIN_REPUTATION_THRESHOLD) {
            eligibleNodes.push_back(node);
        }
    }

    // If no nodes meet the reputation threshold, return nullptr
    if (eligibleNodes.empty()) {
        return nullptr;
    }

    // From eligible nodes, select the one with the highest reputation (tie-breaker)
    Node* leader = nullptr;
    unsigned int highestReputation = 0;
    for (auto& node : eligibleNodes) {
        if (node->getReputation() > highestReputation) {
            highestReputation = node->getReputation();
            leader = node;
        }
    }

    return leader;
}

// Validate the block proposed by the leader
bool Consensus::validateBlock(const Block& block) {
    // Step 1: Verify SNARK proof for all transactions in the block
    for (const auto& tx : block.getTransactions()) {
        if (!SnarkProof::verify(tx.getSnarkProof())) {
            std::cout << "SNARK proof verification failed for transaction in block!" << std::endl;
            return false;
        }
    }

    // Step 2: Validate block structure (previous hash, proof of work, etc.)
    if (!blockchain.isValidNewBlock(block, blockchain.getLatestBlock())) {
        std::cout << "Block structure is invalid!" << std::endl;
        return false;
    }

    return true;
}

// Validator reward distribution after consensus
void Consensus::distributeRewards() {
    // Distribute block rewards among all validators (nodes that participated in consensus)
    unsigned int rewardPerValidator = BLOCK_REWARD / blockchain.getChain().size();

    for (auto& node : network.getNodes()) {
        node->addToBalance(rewardPerValidator);
        std::cout << "Reward distributed to node " << node->getId() << ": " << rewardPerValidator << " coins." << std::endl;
    }
}

// Handle transaction fees distribution
void Consensus::distributeFees() {
    // Distribute transaction fees to all participating nodes
    unsigned int totalFees = 0;

    // Calculate total transaction fees in the latest block
    const Block& latestBlock = blockchain.getLatestBlock();
    for (const auto& tx : latestBlock.getTransactions()) {
        totalFees += tx.getFee();
    }

    unsigned int feePerValidator = totalFees / network.getNodes().size();

    for (auto& node : network.getNodes()) {
        node->addToBalance(feePerValidator);
        std::cout << "Transaction fees distributed to node " << node->getId() << ": " << feePerValidator << " coins." << std::endl;
    }
}

// Perform end-of-round actions such as updating reputation
void Consensus::endRound() {
    std::cout << "Ending consensus round: " << currentRound << std::endl;
    
    // Update node reputations based on activity (participation, validating blocks, etc.)
    for (auto& node : network.getNodes()) {
        node->updateReputation();
        std::cout << "Node " << node->getId() << " reputation updated to: " << node->getReputation() << std::endl;
    }

    // Perform any additional end-of-round tasks (cleanup, reset state, etc.)
}

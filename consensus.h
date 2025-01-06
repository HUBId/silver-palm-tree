#ifndef CONSENSUS_H
#define CONSENSUS_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <memory>
#include <ctime>
#include <cassert>

// Forward declaration of Blockchain and Block classes
class Blockchain;
class Block;

// Node class representing a node in the consensus protocol
class Node {
public:
    std::string id;          // Node ID (can be public key or unique identifier)
    uint64_t reputation;     // Node's reputation based on activity and timetokens
    uint64_t timetokens;     // Node's accumulated timetokens for activity
    uint64_t selected_timetokens; // Timetokens at the time of consensus round selection
    bool is_leader;          // Whether the node is elected as the leader in the round

    // Constructor to initialize a node
    Node(std::string node_id) : id(node_id), reputation(0), timetokens(0), selected_timetokens(0), is_leader(false) {}

    // Increment the node's reputation based on some activity or contribution
    void increment_reputation(uint64_t amount) {
        reputation += amount;
        timetokens += amount;
    }

    // Reset timetokens after each round
    void reset_timetokens() {
        selected_timetokens = timetokens;  // Keep the reputation in tact but reset for the next selection
    }

    // Compare nodes based on reputation and timetokens
    bool operator<(const Node& other) const {
        if (reputation == other.reputation) {
            return selected_timetokens < other.selected_timetokens;
        }
        return reputation < other.reputation;
    }
};

// Consensus class which handles the leader election and voting mechanism
class Consensus {
public:
    Blockchain* blockchain; // Pointer to the blockchain for validating blocks
    std::vector<std::shared_ptr<Node>> nodes;  // List of all nodes in the network
    uint64_t round_number;  // Current round number

    // Constructor for Consensus class
    Consensus(Blockchain* chain) : blockchain(chain), round_number(0) {}

    // Function to add a node to the consensus protocol
    void add_node(std::shared_ptr<Node> node) {
        nodes.push_back(node);
    }

    // Function to run the consensus round and select a leader
    void run_consensus_round() {
        std::cout << "Running consensus round " << round_number << std::endl;

        // Select a leader based on reputation and timetokens
        std::shared_ptr<Node> leader = select_leader();

        // Once a leader is selected, the leader creates a block and broadcasts it
        create_and_broadcast_block(leader);

        // Increment round number
        round_number++;
    }

    // Function to select the leader based on reputation and timetokens
    std::shared_ptr<Node> select_leader() {
        std::vector<std::shared_ptr<Node>> eligible_nodes = get_eligible_nodes_for_leadership();

        // Randomly shuffle eligible nodes to simulate VRF-like selection, but weighted by reputation
        std::shuffle(eligible_nodes.begin(), eligible_nodes.end(), std::default_random_engine(std::time(0)));

        // The node with the highest reputation and timetokens is selected
        std::shared_ptr<Node> leader = eligible_nodes.back();
        leader->is_leader = true;

        std::cout << "Leader elected: " << leader->id << " with reputation: " << leader->reputation << std::endl;
        return leader;
    }

    // Function to get eligible nodes for leadership selection based on reputation
    std::vector<std::shared_ptr<Node>> get_eligible_nodes_for_leadership() {
        std::vector<std::shared_ptr<Node>> eligible_nodes;
        uint64_t max_reputation = 0;

        // Filter nodes based on reputation and timetokens
        for (const auto& node : nodes) {
            if (node->reputation >= max_reputation) {
                eligible_nodes.push_back(node);
                max_reputation = node->reputation;
            }
        }
        return eligible_nodes;
    }

    // Function for the leader to create a new block
    void create_and_broadcast_block(std::shared_ptr<Node> leader) {
        // Create a block for the leader to propose
        std::vector<std::shared_ptr<Block>> transactions_to_include = gather_transactions_for_block();

        // Simulate the block creation and propose it
        std::shared_ptr<Block> new_block = blockchain->create_block(transactions_to_include, leader);

        // Broadcast the block to other nodes (using gossip or direct communication)

        // Add the new block to the blockchain
        blockchain->add_block(new_block);

        std::cout << "Block proposed by leader: " << leader->id << std::endl;
    }

    // Function to gather valid transactions for a block
    std::vector<std::shared_ptr<Block>> gather_transactions_for_block() {
        std::vector<std::shared_ptr<Block>> transactions;
        // Here we would collect transactions from the pool or mempool to include in the block
        // For simplicity, we'll use a placeholder.
        return transactions;
    }
};

// Helper function to simulate VRF-like leader selection
uint64_t vrf_simulation(const Node& node) {
    // Simulate a VRF-like process where a node's chance to be selected as leader is weighted by reputation and timetokens
    return node.reputation + node.selected_timetokens + std::rand();
}

#endif // CONSENSUS_H

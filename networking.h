#ifndef NETWORKING_H
#define NETWORKING_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <memory>
#include <thread>
#include <atomic>
#include <functional>

// Forward declarations
class Transaction;
class Block;
class Node;
class Message;

// Networking layer namespace
namespace Networking {

    // Struct to hold the information about a network peer
    struct Peer {
        std::string address;  // IP or network address of the peer
        uint16_t port;        // Port on which the peer is listening
        std::string id;       // Unique identifier for the peer

        Peer(std::string addr, uint16_t p, std::string identifier)
            : address(addr), port(p), id(identifier) {}
    };

    // Message types for communication between peers
    enum class MessageType {
        TRANSACTION,
        BLOCK,
        STATUS,
        REQUEST_BLOCK,
        REQUEST_TRANSACTION,
        PEER_LIST
    };

    // The message structure used to send data between peers
    struct Message {
        MessageType type;        // Type of message (e.g., transaction, block, etc.)
        std::string content;     // The actual content of the message (e.g., transaction data)
        std::string sender_id;   // ID of the node sending the message
        std::string recipient_id; // ID of the intended recipient node

        Message(MessageType t, const std::string& c, const std::string& sender, const std::string& recipient)
            : type(t), content(c), sender_id(sender), recipient_id(recipient) {}
    };

    // The networking interface for the node
    class INetworkingLayer {
    public:
        virtual ~INetworkingLayer() {}

        // Starts the network communication for the node
        virtual void start() = 0;

        // Stops the network communication
        virtual void stop() = 0;

        // Sends a message to a specified peer
        virtual void send_message(const Peer& peer, const Message& message) = 0;

        // Broadcasts a message to all connected peers
        virtual void broadcast(const Message& message) = 0;

        // Retrieves the list of connected peers
        virtual std::vector<Peer> get_connected_peers() const = 0;

        // Adds a peer to the network
        virtual void add_peer(const Peer& peer) = 0;

        // Removes a peer from the network
        virtual void remove_peer(const std::string& peer_id) = 0;

        // Checks the connection status with a peer
        virtual bool is_connected(const Peer& peer) const = 0;
    };

    // The implementation of the networking layer using libp2p
    class Libp2pNetwork : public INetworkingLayer {
    public:
        Libp2pNetwork(const std::string& node_id);
        ~Libp2pNetwork();

        // Starts the libp2p network node
        void start() override;

        // Stops the libp2p network node
        void stop() override;

        // Sends a message to a specified peer
        void send_message(const Peer& peer, const Message& message) override;

        // Broadcasts a message to all connected peers
        void broadcast(const Message& message) override;

        // Retrieves the list of connected peers
        std::vector<Peer> get_connected_peers() const override;

        // Adds a peer to the network
        void add_peer(const Peer& peer) override;

        // Removes a peer from the network
        void remove_peer(const std::string& peer_id) override;

        // Checks the connection status with a peer
        bool is_connected(const Peer& peer) const override;

    private:
        std::string node_id_;  // Unique identifier for the node in the network
        std::unordered_set<std::string> connected_peers_; // Set of connected peer IDs
        std::atomic<bool> running_;  // Flag to indicate if the network is running
        std::thread network_thread_;  // Network thread for handling communication

        // Private helper function to run the libp2p network communication
        void run_network();

        // Private helper function to simulate message reception
        void handle_incoming_message(const Message& msg);

        // Private helper function to simulate message sending (mocked for the example)
        void simulate_send_message(const Message& msg);
    };

    // The GossipProtocol is responsible for propagating blocks and transactions
    class GossipProtocol {
    public:
        GossipProtocol(std::shared_ptr<INetworkingLayer> network_layer);
        ~GossipProtocol();

        // Starts the gossip protocol for the node
        void start();

        // Stops the gossip protocol
        void stop();

        // Propagates a transaction to other nodes in the network
        void propagate_transaction(const Transaction& tx);

        // Propagates a block to other nodes in the network
        void propagate_block(const Block& block);

        // Receives a transaction from the network and validates it
        void receive_transaction(const Message& msg);

        // Receives a block from the network and validates it
        void receive_block(const Message& msg);

    private:
        std::shared_ptr<INetworkingLayer> network_layer_;  // Reference to the networking layer
        std::atomic<bool> running_;  // Flag to indicate if the gossip protocol is running
        std::thread gossip_thread_;  // Gossip protocol thread

        // Private helper function to handle gossip protocol
        void run_gossip();
    };

}  // End of Networking namespace

#endif  // NETWORKING_H

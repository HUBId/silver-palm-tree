#ifndef WALLET_H
#define WALLET_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <stdexcept>

// Forward declarations
class Transaction;
class Block;
class Peer;
class SNARKProof;

// Wallet namespace
namespace Wallet {

    // Class representing a public/private key pair for the wallet
    class KeyPair {
    public:
        std::string public_key;   // The public key of the wallet
        std::string private_key;  // The private key for signing transactions

        KeyPair() {}
        KeyPair(const std::string& pub, const std::string& priv)
            : public_key(pub), private_key(priv) {}

        // Generate a new keypair (in a real scenario this would use a cryptographic library)
        static KeyPair generate_keypair();
    };

    // Transaction class: Handles the creation of transactions
    class Transaction {
    public:
        std::string sender;       // Public key of the sender
        std::string receiver;     // Public key of the receiver
        double amount;            // Amount to send in the transaction
        uint64_t timestamp;       // Timestamp of when the transaction was created
        std::string signature;    // The signature to validate the transaction

        Transaction(const std::string& send, const std::string& recv, double amt, uint64_t ts)
            : sender(send), receiver(recv), amount(amt), timestamp(ts) {}

        // Signs the transaction using the sender's private key
        void sign(const std::string& private_key);

        // Validates the transaction by verifying the signature
        bool validate() const;

        // Serializes the transaction to a string
        std::string serialize() const;
    };

    // Class for managing wallet operations
    class Wallet {
    public:
        KeyPair keypair;               // The keypair for this wallet
        std::vector<Transaction> tx_pool;  // Pool of transactions that need to be sent

        Wallet(const KeyPair& kp);
        
        // Creates a new transaction
        Transaction create_transaction(const std::string& receiver, double amount);

        // Signs a transaction using the wallet's private key
        void sign_transaction(Transaction& tx);

        // Validates a transaction
        bool validate_transaction(const Transaction& tx);

        // Sends a transaction to a peer (mock function for testing purposes)
        void send_transaction(const Peer& peer, const Transaction& tx);

        // Generates the SNARK proof for a transaction (if applicable)
        SNARKProof generate_snark_proof(const Transaction& tx);

        // Serializes the wallet into a string (for persistence)
        std::string serialize() const;

        // Deserializes the wallet from a string
        static Wallet deserialize(const std::string& data);
    };

    // Class to represent a SNARK Proof
    class SNARKProof {
    public:
        std::string proof_data;   // The actual proof data (in a real system, this would be complex)

        SNARKProof(const std::string& proof)
            : proof_data(proof) {}

        // Serializes the SNARK proof into a string
        std::string serialize() const;
    };

    // Utility function to simulate transaction signing (this would be based on a cryptographic library in reality)
    std::string sign_data(const std::string& data, const std::string& private_key);

    // Utility function to simulate transaction verification (again, would use cryptographic verification in practice)
    bool verify_signature(const std::string& data, const std::string& signature, const std::string& public_key);
}

#endif // WALLET_H

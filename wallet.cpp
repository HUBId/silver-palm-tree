#include "wallet.h"
#include "transaction.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <secp256k1.h>  // Secp256k1 library for cryptographic operations
#include <openssl/sha.h>  // For hash functions (SHA256, etc.)
#include <iomanip>  // For formatting the output

namespace prunet {

// Wallet class that contains keys, address generation, and transaction signing
class Wallet {
public:
    Wallet();  // Constructor to generate a new wallet with key pair
    Wallet(const std::string& privateKey);  // Constructor with an existing private key

    std::string getAddress() const;  // Get the public address of the wallet
    std::string getPrivateKey() const;  // Get the private key of the wallet
    std::string signTransaction(const Transaction& transaction);  // Sign a transaction
    static std::string generateRandomPrivateKey();  // Generate a random private key

private:
    secp256k1_context* ctx;  // Context for secp256k1 operations
    secp256k1_pubkey pubKey;  // Public key
    secp256k1_privkey privKey;  // Private key

    std::string privateKeyToHex(const secp256k1_privkey& privKey) const;  // Convert private key to hex string
    std::string publicKeyToAddress(const secp256k1_pubkey& pubKey) const;  // Convert public key to address
    void generateKeyPair();  // Generate a key pair (private + public)
};

// Constructor to generate a new wallet with a key pair
Wallet::Wallet() {
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    generateKeyPair();
}

// Constructor with an existing private key
Wallet::Wallet(const std::string& privateKeyHex) {
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    // Convert hex private key string to secp256k1_privkey type
    std::vector<unsigned char> privKeyVec = hexToBytes(privateKeyHex);
    if (!secp256k1_privkey_parse(ctx, &privKey, privKeyVec.data())) {
        throw std::runtime_error("Invalid private key");
    }
    // Generate the corresponding public key
    secp256k1_pubkey pubKey;
    if (!secp256k1_ec_pubkey_create(ctx, &pubKey, &privKey)) {
        throw std::runtime_error("Failed to generate public key from private key");
    }
    this->pubKey = pubKey;
}

// Get the wallet address (public key)
std::string Wallet::getAddress() const {
    return publicKeyToAddress(pubKey);
}

// Get the wallet private key as a hex string
std::string Wallet::getPrivateKey() const {
    return privateKeyToHex(privKey);
}

// Sign a transaction using the wallet's private key
std::string Wallet::signTransaction(const Transaction& transaction) {
    // Serialize the transaction (simplified for this example)
    std::string transactionData = transaction.serialize();

    // Hash the transaction data
    unsigned char transactionHash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, transactionData.c_str(), transactionData.size());
    SHA256_Final(transactionHash, &sha256_ctx);

    // Sign the transaction hash with the private key
    unsigned char signature[72];
    size_t sigLen = 0;
    if (!secp256k1_ecdsa_sign(ctx, signature, &sigLen, transactionHash, &privKey, nullptr)) {
        throw std::runtime_error("Failed to sign transaction");
    }

    // Convert signature to a string and return
    std::ostringstream oss;
    for (size_t i = 0; i < sigLen; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)signature[i];
    }
    return oss.str();
}

// Generate a random private key for the wallet
std::string Wallet::generateRandomPrivateKey() {
    // Here we simply use a basic random number generator for this example
    // In production, you should use a cryptographically secure random number generator
    std::vector<unsigned char> randomBytes(32);
    for (size_t i = 0; i < 32; ++i) {
        randomBytes[i] = rand() % 256;
    }
    std::string privateKeyHex = bytesToHex(randomBytes);
    return privateKeyHex;
}

// Convert private key to a hex string
std::string Wallet::privateKeyToHex(const secp256k1_privkey& privKey) const {
    unsigned char privKeyBytes[32];
    secp256k1_privkey_serialize(ctx, privKeyBytes, &privKey);
    return bytesToHex(privKeyBytes);
}

// Convert public key to an address (simplified)
std::string Wallet::publicKeyToAddress(const secp256k1_pubkey& pubKey) const {
    unsigned char pubKeyBytes[33];
    size_t pubKeyLen = 33;
    secp256k1_ec_pubkey_serialize(ctx, pubKeyBytes, &pubKeyLen, &pubKey, SECP256K1_EC_COMPRESSED);

    // Simple hash (this is a simplified address generation, actual blockchain may use SHA256 or RIPEMD160)
    unsigned char addressHash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, pubKeyBytes, pubKeyLen);
    SHA256_Final(addressHash, &sha256_ctx);

    // Convert hash to hex string for address
    return bytesToHex(addressHash);
}

} // namespace prunet

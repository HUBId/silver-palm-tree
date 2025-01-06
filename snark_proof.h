#ifndef SNARK_PROOF_H
#define SNARK_PROOF_H

#include <iostream>
#include <string>
#include <vector>

// Forward declaration for blockchain and transaction classes
class Transaction;
class Block;

// Namespace for SNARK-related functions
namespace SnarkProof {

    // Struct to hold a generated SNARK proof
    struct Proof {
        std::string proof_data;  // The proof itself (this is a placeholder for actual proof data)
        std::string public_inputs;  // Public inputs used in generating the proof
    };

    // Abstract class for the SNARK proof generation logic
    class ProofGenerator {
    public:
        virtual Proof generate_proof(const Transaction& tx) = 0;  // Generate SNARK proof for a transaction
        virtual bool verify_proof(const Proof& proof) = 0;  // Verify SNARK proof for a transaction
    };

    // Halo2 implementation of SNARK Proof Generator (a subclass of ProofGenerator)
    class Halo2ProofGenerator : public ProofGenerator {
    public:
        Halo2ProofGenerator();  // Constructor for the Halo2-based proof generator
        ~Halo2ProofGenerator();  // Destructor for the Halo2-based proof generator

        // Generate a proof for a given transaction (using Halo2)
        Proof generate_proof(const Transaction& tx) override;

        // Verify the proof for a given transaction (using Halo2)
        bool verify_proof(const Proof& proof) override;

    private:
        // Private helper functions for Halo2-based SNARK proof generation
        std::string apply_halo2_circuit(const Transaction& tx);
        bool verify_halo2_proof(const std::string& proof_data);
    };

    // KZG10 implementation of SNARK Proof Generator (a subclass of ProofGenerator)
    class KZG10ProofGenerator : public ProofGenerator {
    public:
        KZG10ProofGenerator();  // Constructor for the KZG10-based proof generator
        ~KZG10ProofGenerator();  // Destructor for the KZG10-based proof generator

        // Generate a proof for a given transaction (using KZG10)
        Proof generate_proof(const Transaction& tx) override;

        // Verify the proof for a given transaction (using KZG10)
        bool verify_proof(const Proof& proof) override;

    private:
        // Private helper functions for KZG10-based SNARK proof generation
        std::string apply_kzg10_circuit(const Transaction& tx);
        bool verify_kzg10_proof(const std::string& proof_data);
    };

    // Utility function to serialize and deserialize proofs
    std::string serialize_proof(const Proof& proof);
    Proof deserialize_proof(const std::string& serialized_proof);

    // Utility function to validate the public inputs used in proof generation
    bool validate_public_inputs(const std::string& public_inputs);
    
} // End of namespace SnarkProof

#endif // SNARK_PROOF_H

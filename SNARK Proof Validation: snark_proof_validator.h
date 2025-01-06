#ifndef SNARK_PROOF_VALIDATOR_H
#define SNARK_PROOF_VALIDATOR_H

#include <vector>

class SnarkProofValidator {
public:
    bool validateProof(const std::vector<uint8_t>& proof_data);
};

#endif // SNARK_PROOF_VALIDATOR_H

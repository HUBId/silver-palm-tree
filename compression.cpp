#include "compression.h"
#include <iostream>
#include <vector>
#include <zlib.h>  // For compression and decompression
#include <stdexcept>
#include <assert.h>

// Compression using zlib (you can replace this with more advanced algorithms for real-world use)
namespace Compcrypt {

// Compress data using zlib (for simplicity, but can be replaced with more advanced techniques)
std::vector<uint8_t> compress(const std::vector<uint8_t>& inputData) {
    uLongf compressedSize = compressBound(inputData.size());
    std::vector<uint8_t> compressedData(compressedSize);

    int result = compress(compressedData.data(), &compressedSize, inputData.data(), inputData.size());
    if (result != Z_OK) {
        throw std::runtime_error("Compression failed!");
    }

    compressedData.resize(compressedSize);  // Resize to actual compressed data size
    return compressedData;
}

// Decompress data using zlib
std::vector<uint8_t> decompress(const std::vector<uint8_t>& compressedData) {
    uLongf decompressedSize = compressedData.size() * 10;  // Initial guess (can be optimized)
    std::vector<uint8_t> decompressedData(decompressedSize);

    int result = uncompress(decompressedData.data(), &decompressedSize, compressedData.data(), compressedData.size());
    while (result == Z_BUF_ERROR) {
        // Increase buffer size if it's not enough to decompress
        decompressedSize *= 2;
        decompressedData.resize(decompressedSize);
        result = uncompress(decompressedData.data(), &decompressedSize, compressedData.data(), compressedData.size());
    }

    if (result != Z_OK) {
        throw std::runtime_error("Decompression failed!");
    }

    decompressedData.resize(decompressedSize);  // Resize to actual decompressed data size
    return decompressedData;
}

// Recursive compression to optimize the blockchain's data footprint
std::vector<uint8_t> recursiveCompress(const std::vector<uint8_t>& inputData) {
    if (inputData.size() < MIN_COMPRESSION_THRESHOLD) {
        // Base case: data is small enough to not compress further
        return inputData;
    }

    // First round of compression using zlib
    std::vector<uint8_t> compressedData = compress(inputData);

    // Recursively compress the result if it still meets the threshold
    if (compressedData.size() < inputData.size()) {
        return recursiveCompress(compressedData);
    }

    // If compression doesn't help, return the original data
    return inputData;
}

// Function to test compression and decompression
void testCompression() {
    std::vector<uint8_t> sampleData = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<uint8_t> compressed = compress(sampleData);
    std::vector<uint8_t> decompressed = decompress(compressed);

    assert(sampleData == decompressed);  // Ensure compression-decompression cycle works
    std::cout << "Compression and decompression test passed!" << std::endl;
}

}  // namespace Compcrypt

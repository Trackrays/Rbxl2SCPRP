#include <fstream>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "lz4.h"

// Struct to read the header into
#pragma pack(push, 1)
struct Header {
    uint32_t classCount;
    uint32_t instanceCount;
    uint8_t  reserved[8];
};
#pragma pack(pop)

// Expected file signature for rbxl files; goes "<roblox! " to denote a binary .rbxl file
constexpr uint8_t kSig[14] = {
  0x3C, 0x72, 0x6F, 0x62, 0x6C, 0x6F, 0x78, 0x21,
  0x89, 0xFF, 0x0D, 0x0A, 0x1A, 0x0A
};

int main(int argc, char* argv[]) {
  std::string filepath;

  for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-path") == 0) {
            if (i + 1 >= argc) {
                std::fprintf(stderr, "Error: -path requires a value\n");
                return 1;
            }
            filepath = argv[++i]; // consume the next arg as the value
        }
    }

  if (filepath.empty()) {
    std::fprintf(stderr, "Usage: %s -path <file.rbxl>\n", argv[0]);
    return 1;
  }

  std::fprintf(stdout, "Reading file...\n");
  std::ifstream file(filepath, std::ios::binary);

  if (!file) {
    std::fprintf(stderr, "Could not open file\n");
    return 1;
  }

  uint8_t sig[14];
  file.read(reinterpret_cast<char*>(&sig), sizeof(sig));
  if (!file || std::memcmp(sig, kSig, sizeof(sig)) != 0) {
    std::fprintf(stderr, "invalid .rbxl file (bad signature)");
    return 1;
  }

  uint16_t version;
  file.read(reinterpret_cast<char*>(&version), sizeof(version));
  if (!file || version != 0) {
    std::fprintf(stderr, "unsupported version: %u\n", version);
    return 1;
  }

  Header header;
  file.read(reinterpret_cast<char*>(&header), sizeof(header));
  if (!file) {
    std::fprintf(stderr, "short reader on header\n");
    return 1;
  }

  std::printf("ClassCount:     %u\n", header.classCount);
  std::printf("InstanceCount:  %u\n", header.instanceCount);
}
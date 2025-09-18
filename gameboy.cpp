#include <iostream>
#include <cstdint>
#include <cstdio>

class MMU{
public:
  uint8_t getByteAt(uint16_t address) { return 0; }
  uint16_t getWordAt(uint16_t address) { return 0; }
  void setByteAt(uint16_t address, uint8_t value) {}
  void setWordAt(uint16_t address, uint16_t value) {}
};

class CPU{
public:
  uint16_t programCounter = 0;
  uint16_t stackPointer = 0;
  uint16_t registerBC = 0;
  uint16_t registerDE = 0;
  uint16_t registerHL = 0;
  uint16_t registerAF = 0;

  uint8_t* registerA = reinterpret_cast<uint8_t*>(&registerAF) + 1;
  uint8_t* registerF = reinterpret_cast<uint8_t*>(&registerAF);

  uint8_t* registerB = reinterpret_cast<uint8_t*>(&registerBC) + 1;
  uint8_t* registerC = reinterpret_cast<uint8_t*>(&registerBC);

  uint8_t* registerD = reinterpret_cast<uint8_t*>(&registerDE) + 1;
  uint8_t* registerE = reinterpret_cast<uint8_t*>(&registerDE);

  uint8_t* registerH = reinterpret_cast<uint8_t*>(&registerHL) + 1;
  uint8_t* registerL = reinterpret_cast<uint8_t*>(&registerHL);

  MMU* mmu;

  bool flagZ = false;
  bool flagN = false;
  bool flagH = false;
  bool flagC = false;
  bool interruptsEnabled = false;
  bool halt = false;
  bool stopped = false;

  int cycleCounter = 0;
  bool enableInterruptsNext = false;

  void executeInstruction(uint8_t opcode){
    switch (opcode) {
      case 0x00:
        break; // NOP

      case 0x10:
        std::printf("[INFO] CPU Stopped\n");
        stopped = true;
        break;

      case 0x76:
        if (interruptsEnabled) halt = true;
        break;

      case 0xF3:
        interruptsEnabled = false;
        break;

      case 0xFB:
        enableInterruptsNext = true;
        break;

      default:
        std::printf("[ERR] Unknown Opcode: 0x%02x At 0x%04x\n", opcode, programCounter - 1);
    }
  }
};

int main(int argc, char** argv){

  MMU mmu;
  CPU cpu;
  cpu.mmu = &mmu;
  
  uint8_t program[] = {0x00, 0x10, 0x76, 0xF3, 0xFB};
  for(uint8_t instruction : program){
    cpu.executeInstruction(instruction);
  }

  return 0;
}

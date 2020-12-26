#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

#include "instruction.h"
#include "printRoutines.h"

/* Reads one byte from memory, at the specified address. Stores the
   read value into *value. Returns 1 in case of success, or 0 in case
   of failure (e.g., if the address is beyond the limit of the memory
   size). */
int memReadByte(machine_state_t *state,	uint64_t address, uint8_t *value) {

  /* THIS PART TO BE COMPLETED BY THE STUDENT */
  if (address < state->programSize){
      *value = state->programMap[address];
      return 1;
  }
  return 0;
}

/* Reads one quad-word (64-bit number) from memory in little-endian
   format, at the specified starting address. Stores the read value
   into *value. Returns 1 in case of success, or 0 in case of failure
   (e.g., if the address is beyond the limit of the memory size). */
int memReadQuadLE(machine_state_t *state, uint64_t address, uint64_t *value) {

  /* THIS PART TO BE COMPLETED BY THE STUDENT */
  if ((address + 7) < state->programSize){
//      memcpy(value, state->programMap + address, 8);
      uint64_t currValue = state->programMap[address];

      for (int i = 1; i < 8; i++) {
          currValue = (uint64_t) state->programMap[address + i] << 8*i | currValue;
      }

      *value = currValue;
      return 1;
  }
  return 0;
}

/* Stores the specified one-byte value into memory, at the specified
   address. Returns 1 in case of success, or 0 in case of failure
   (e.g., if the address is beyond the limit of the memory size). */
int memWriteByte(machine_state_t *state,  uint64_t address, uint8_t value) {

  /* THIS PART TO BE COMPLETED BY THE STUDENT */
    if (address < state->programSize){
        state->programMap[address] = value;
        return 1;
    }
    return 0;
}

/* Stores the specified quad-word (64-bit) value into memory, at the
   specified start address, using little-endian format. Returns 1 in
   case of success, or 0 in case of failure (e.g., if the address is
   beyond the limit of the memory size). */
int memWriteQuadLE(machine_state_t *state, uint64_t address, uint64_t value) {

  /* THIS PART TO BE COMPLETED BY THE STUDENT */
    if ((address + 7) < state->programSize){
        state->programMap[address] = value & 0xff;
        for (int i = 1; i < 8; i++) {
            state->programMap[address + i] = (value >> 8*i) & 0xff;
        }
        return 1;
    }
    return 0;
}

/* Check register number. Return 1 if valid, return 0 otherwise. */
int checkRegisterNumber(uint8_t r_num){
    if(r_num < R_RAX || r_num > R_NONE){
        return 0;
    }
    return 1;
}

/* Check function code. Return 1 if valid, return 0 otherwise. */
int checkFunCode(uint8_t ifun_num){
    if(ifun_num < 0x0 || ifun_num > 0x6){
        return 0;
    }
    return 1;
}

/* Fetches one instruction from memory, at the address specified by
   the program counter. Does not modify the machine's state. The
   resulting instruction is stored in *instr. Returns 1 if the
   instruction is a valid non-halt instruction, or 0 (zero)
   otherwise. */
int fetchInstruction(machine_state_t *state, y86_instruction_t *instr) {

  /* THIS PART TO BE COMPLETED BY THE STUDENT */
    uint8_t byte_0;
    uint8_t byte_1;

    instr->location = state->programCounter;

    if (memReadByte(state, instr->location, &byte_0)){
        // Find icode and ifun in byte0.
        instr->icode = byte_0 >> 4;
        instr->ifun = byte_0 & 0x0F;

        //Check for 1-byte instruction: halt, nop, and ret.
        //TODO: should I send error message in Fetch?
        switch (instr->icode) {
            case I_HALT:
            case I_NOP:
            case I_RET:
                if (instr->ifun == 0){
                    instr->valP = instr->location + 1;

                    //Return 0 for HALT.
                    if (instr->icode == I_HALT){
                        return 0;
                    }
                    return 1;
                }
                else{
                    instr->icode = I_INVALID;
                    return 0;
                }

            //check jXX and call (No register involved).
            case I_JXX:
                if (!checkFunCode(instr->ifun)){
                    return 0;
                }
                else{
                    // The first one byte is valid, then check the rest.
                    if (!memReadQuadLE(state, instr->location+1, &instr->valC)) {
                        instr->icode = I_TOO_SHORT;
                        return 0;
                    }
                    instr->valP = instr->location + 9;
                    return 1;
                }
            case I_CALL:
                if (instr->ifun != 0){
                    instr->icode = I_INVALID;
                    return 0;
                }
                else{
                    // The first one byte is valid, then check the rest.
                    if (!memReadQuadLE(state, instr->location+1, &instr->valC)) {
                        instr->icode = I_TOO_SHORT;
                        return 0;
                    }
                    instr->valP = instr->location + 9;
                    return 1;
                }
            //Check for the rest instructions(Involving rA, rB).
            default:
                if (memReadByte(state, instr->location+1, &byte_1)){
                    //Read rA, rB.
                    if (!checkRegisterNumber(byte_1 >> 4) || !checkRegisterNumber(byte_1 & 0x0F)){
                        //Invalid register number.
                        instr->icode = I_INVALID;
                        return 0;
                    }

                    instr->rA = byte_1 >> 4;
                    instr->rB = byte_1 & 0x0F;

                    switch(instr->icode){
                        case I_RRMVXX:
                        case I_OPQ:
                            if (!checkFunCode(instr->ifun) || instr->rA == R_NONE || instr->rB == R_NONE){
                                instr->icode = I_INVALID;
                                return 0;
                            }
                            instr->valP = instr->location + 2;
                            return 1;


                        case I_IRMOVQ:
                            if (instr->ifun != 0 || instr->rA != R_NONE || instr->rB == R_NONE){
                                instr->icode = I_INVALID;
                                return 0;
                            }
                            else{
                                // The first two bytes are valid, then check the rest.
                                if (!memReadQuadLE(state, instr->location+2, &instr->valC)){
                                    instr->icode = I_TOO_SHORT;
                                    return 0;
                                }
                                instr->valP = instr->location + 10;
                                return 1;
                            }

                        case I_RMMOVQ:
                        case I_MRMOVQ:
                            if (instr->ifun != 0 || instr->rA == R_NONE || instr->rB == R_NONE){
                                instr->icode = I_INVALID;
                                return 0;
                            }
                            else{
                                // The first two bytes are valid, then check the rest.
                                if (!memReadQuadLE(state, instr->location+2, &instr->valC)){
                                    instr->icode = I_TOO_SHORT;
                                    return 0;
                                }
                                instr->valP = instr->location + 10;
                                return 1;
                            }

                        case I_PUSHQ:
                        case I_POPQ:
                            if (instr->ifun != 0 || instr->rA == R_NONE || instr->rB != R_NONE){
                                instr->icode = I_INVALID;
                                return 0;
                            }
                            instr->valP = instr->location + 2;
                            return 1;
                        default:
                            break;
                    }
                }else{
                    //Failed to read the second byte, so the instruction is too short.
                    instr->icode = I_TOO_SHORT;
                }
        }
    }else{
        //Failed to read one byte, so the instruction is too short.
        instr->icode = I_TOO_SHORT;
    }
  return 0;
}

/* Executes the instruction specified by *instr, modifying the
   machine's state (memory, registers, condition codes, program
   counter) in the process. Returns 1 if the instruction was executed
   successfully, or 0 if there was an error. Typical errors include an
   invalid instruction or a memory access to an invalid address. */
int executeInstruction(machine_state_t *state, y86_instruction_t *instr) {

  /* THIS PART TO BE COMPLETED BY THE STUDENT */
  switch (instr->icode){
      case I_HALT:
          return 1;

      case I_NOP:
          state->programCounter = instr->valP;
          return 1;

      case I_RRMVXX:

          // Move from rA to rB depending on the function code.
          switch (instr->ifun){
              case C_NC:
                  state->registerFile[instr->rB] = state->registerFile[instr->rA];
                  break;

              case C_LE:
                  if (((state->conditionCodes & CC_ZERO_MASK) == CC_ZERO_MASK) ||
                          (state->conditionCodes & CC_SIGN_MASK) == CC_SIGN_MASK){
                      state->registerFile[instr->rB] = state->registerFile[instr->rA];
                  }
                  break;

              case C_L:
                  if ((state->conditionCodes & CC_SIGN_MASK) == CC_SIGN_MASK){
                      state->registerFile[instr->rB] = state->registerFile[instr->rA];
                  }
                  break;

              case C_E:
                  if ((state->conditionCodes & CC_ZERO_MASK) == CC_ZERO_MASK){
                      state->registerFile[instr->rB] = state->registerFile[instr->rA];
                  }
                  break;

              case C_NE:
                  if ((state->conditionCodes & CC_ZERO_MASK) == 0){
                      state->registerFile[instr->rB] = state->registerFile[instr->rA];
                  }
                  break;

              case C_GE:
                  if ((state->conditionCodes & CC_SIGN_MASK) == 0){
                      state->registerFile[instr->rB] = state->registerFile[instr->rA];
                  }
                  break;

              case C_G:
                  if (((state->conditionCodes & CC_ZERO_MASK) == 0) && ((state->conditionCodes & CC_SIGN_MASK) == 0)){
                      state->registerFile[instr->rB] = state->registerFile[instr->rA];
                  }
                  break;
          }
          //Update program counter no matter what function code is.
          state->programCounter = instr->valP;
          return 1;

      case I_IRMOVQ:
          state->registerFile[instr->rB] = instr->valC;
          state->programCounter = instr->valP;
          return 1;

      case I_RMMOVQ:
          if (memWriteQuadLE(state, state->registerFile[instr->rB]+(instr->valC), state->registerFile[instr->rA]) == 1){
              state->programCounter = instr->valP;
              return 1;
          }
          printErrorInvalidMemoryLocation(stderr, instr, state->registerFile[instr->rB]+(instr->valC));
          break;

      case I_MRMOVQ:
          if (memReadQuadLE(state, state->registerFile[instr->rB]+(instr->valC), &state->registerFile[instr->rA]) == 1){
              state->programCounter = instr->valP;
              return 1;
          }
          printErrorInvalidMemoryLocation(stderr, instr, state->registerFile[instr->rB]+(instr->valC));
          break;

      case I_OPQ:
          switch (instr->ifun){
              case A_ADDQ:
                  state->registerFile[instr->rB] = state->registerFile[instr->rA] + state->registerFile[instr->rB];
                  break;

              case A_SUBQ:
                  state->registerFile[instr->rB] = state->registerFile[instr->rB] - state->registerFile[instr->rA];
                  break;

              case A_ANDQ:
                  state->registerFile[instr->rB] = state->registerFile[instr->rB] & state->registerFile[instr->rA];
                  break;

              case A_XORQ:
                  state->registerFile[instr->rB] = state->registerFile[instr->rA] ^ state->registerFile[instr->rB];
                  break;

              case A_MULQ:
                  state->registerFile[instr->rB] = state->registerFile[instr->rA] * state->registerFile[instr->rB];
                  break;

              case A_DIVQ:
                  state->registerFile[instr->rB] = state->registerFile[instr->rB] / state->registerFile[instr->rA];
                  break;

              case A_MODQ:
                  state->registerFile[instr->rB] = state->registerFile[instr->rB] % state->registerFile[instr->rA];
                  break;
          }

          //Update CC.
          uint64_t result = state->registerFile[instr->rB];

          if (!result){
              state->conditionCodes = CC_ZERO_MASK;
          }
          else if (result & 0x8000000000000000){
              state->conditionCodes = CC_SIGN_MASK;
          }
          else {
              state->conditionCodes = 0x0;
          }
          state->programCounter = instr->valP;
          return 1;

      case I_JXX:
          switch (instr->ifun){
              case C_NC:
                  state->programCounter = instr->valC;
                  return 1;

              case C_LE:
                  if (((state->conditionCodes & CC_ZERO_MASK) == CC_ZERO_MASK) ||
                          ((state->conditionCodes & CC_SIGN_MASK) == CC_SIGN_MASK)){
                      state->programCounter = instr->valC;
                      return 1;
                  }
                  break;

              case C_L:
                  if ((state->conditionCodes & CC_SIGN_MASK) == CC_SIGN_MASK){
                      state->programCounter = instr->valC;
                      return 1;
                  }
                  break;

              case C_E:
                  if ((state->conditionCodes & CC_ZERO_MASK) == CC_ZERO_MASK){
                      state->programCounter = instr->valC;
                      return 1;
                  }
                  break;

              case C_NE:
                  if ((state->conditionCodes & CC_ZERO_MASK) == 0){
                      state->programCounter = instr->valC;
                      return 1;
                  }
                  break;

              case C_GE:
                  if ((state->conditionCodes & CC_SIGN_MASK) == 0){
                      state->programCounter = instr->valC;
                      return 1;
                  }
                  break;

              case C_G:
                  if ((state->conditionCodes & CC_ZERO_MASK) == 0 && (state->conditionCodes & CC_SIGN_MASK) == 0){
                      state->programCounter = instr->valC;
                      return 1;
                  }
                  break;
          }
          state->programCounter = instr->valP;
          return 1;

      case I_CALL:
          if(memWriteQuadLE(state, state->registerFile[R_RSP]-8, instr->valP) == 1) {
              state->programCounter = instr->valC;
              state->registerFile[R_RSP] -= 8;
              return 1;
          }
          printErrorInvalidMemoryLocation(stderr, instr, state->registerFile[R_RSP]-8);
          break;

      case I_RET:
          if(memReadQuadLE(state, state->registerFile[R_RSP], &state->programCounter) == 1){
              state->registerFile[R_RSP] += 8;
              return 1;
          }
          printErrorInvalidMemoryLocation(stderr, instr, state->registerFile[R_RSP]);
          break;

      case I_PUSHQ:
          if(memWriteQuadLE(state, state->registerFile[R_RSP]-8, state->registerFile[instr->rA]) == 1){
              state->registerFile[R_RSP] -= 8;
              state->programCounter = instr->valP;
              return 1;
          }
          printErrorInvalidMemoryLocation(stderr, instr, state->registerFile[R_RSP]-8);
          break;

      case I_POPQ:
          if(memReadQuadLE(state, state->registerFile[R_RSP], &state->registerFile[instr->rA]) == 1){
              state->registerFile[R_RSP] += 8;
              state->programCounter = instr->valP;
              return 1;
          }
          printErrorInvalidMemoryLocation(stderr, instr, state->registerFile[R_RSP]);
          break;

      case I_INVALID:
      case I_TOO_SHORT:
          return 0;
  }
  return 0;
}

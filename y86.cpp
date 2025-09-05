#include <iostream>
#include <iomanip>
#include <climits>
#include <string>
#include <fstream>

using namespace std;
using Byte = unsigned char;

// System ram which contains 8192 bytes
struct MEM{
    long long mem[65536];
    long long operator[](long long pc){
        return mem[pc];
    }
};

void readObjectFile(string fname, MEM& instructionMemory){

}

struct CPU{
    long long rax;
    long long rcx;
    long long rdx;
    long long rbx;
    long long rsp;
    long long rbp;
    long long rsi;
    long long rdi;
    long long r8;
    long long r9;
    long long r10;
    long long r11;
    long long r12;
    long long r13;
    long long r14;

    bool ZF = 0;
    bool SF = 0;
    bool OF = 0;
    bool HLT = 0;

    unsigned long long PC;

    MEM RAM;

    static constexpr Byte halt = 0x00;
    static constexpr Byte nop = 0x10;
    static constexpr Byte rrmovq = 0x20;
    static constexpr Byte cmovle = 0x21;
    static constexpr Byte cmovl = 0x22;
    static constexpr Byte cmove = 0x23;
    static constexpr Byte cmovne = 0x24;
    static constexpr Byte cmovge = 0x25;
    static constexpr Byte cmovg = 0x26;
    static constexpr Byte irmovq = 0x30;
    static constexpr Byte rmmovq = 0x40;
    static constexpr Byte mrmovq = 0x50;
    static constexpr Byte addq = 0x60;
    static constexpr Byte subq = 0x61;
    static constexpr Byte andq = 0x62;
    static constexpr Byte xorq = 0x63;
    static constexpr Byte jmp = 0x70;
    static constexpr Byte jle = 0x71;
    static constexpr Byte jl = 0x72;
    static constexpr Byte je = 0x73;
    static constexpr Byte jne = 0x74;
    static constexpr Byte jge = 0x75;
    static constexpr Byte jg = 0x76;
    static constexpr Byte call = 0x80;
    static constexpr Byte ret = 0x90;
    static constexpr Byte pushq = 0xa0;
    static constexpr Byte popq = 0xb0;

    long long* getRegister(Byte regID){
        switch(regID){
            case 0x0:
            return &rax;
            case 0x1:
            return &rcx;
            case 0x2:
            return &rdx;
            case 0x3:
            return &rbx;
            case 0x4:
            return &rsp;
            case 0x5:
            return &rbp;
            case 0x6:
            return &rsi;
            case 0x7:
            return &rdi;
            case 0x8:
            return &r8;
            case 0x9:
            return &r9;
            case 0xa:
            return &r10;
            case 0xb:
            return &r11;
            case 0xc:
            return &r12;
            case 0xd:
            return &r13;
            case 0xe:
            return &r14;
            default:
            return nullptr;
        }
    }

    void resetFlags(){
        OF = 0;
        SF = 0;
        ZF = 0;
    }

    void setFlags(long long* reg){
        if(*reg == 0){
            ZF = 1;
        }
        else if(*reg < 0){
            SF = 1;
        }
    }

    long long fetch8Bytes(MEM mem){
        long long data = 0;
        for(int i = 0; i < 8; ++i){
            data += mem[PC++];
            if(i != 7){
                data = data<<8;
            }
        }
        return data;
    }

    void execute(MEM rom){
       Byte ins = rom[PC++];
       switch(ins){
        case halt:{
            HLT = 1;
            PC = 0;
            break;
        }
        case nop:{
            break;
        }
        case rrmovq:{
            Byte rArB = rom[PC++];
            long long* rA = getRegister(rArB&0xF0);
            long long* rB = getRegister(rArB&0x0F);
            swap(*rA, *rB);
            break;
        }
        // case cmovle:
        // break;
        // case cmovl:
        // break;
        // case cmove:
        // break;
        // case cmovne:
        // break;
        // case cmovge:
        // break;
        // case cmovg:
        // break;
        case irmovq:{
            Byte rArB = rom[PC++];
            long long* rB = getRegister(rArB&0x0F);
            long long i = fetch8Bytes(rom);
            cout << hex << i << endl;
            *rB = i;
            break;
        }
        case rmmovq:{
            Byte rArB = rom[PC++];
            long long* rA = getRegister(rArB&0xF0);
            long long* rB = getRegister(rArB&0x0F);
            long long d = fetch8Bytes(rom);
            RAM.mem[*rB + d] = *rA;
            break;
        }
        case mrmovq:{
            Byte rArB = rom[PC++];
            long long* rA = getRegister(rArB&0xF0);
            long long* rB = getRegister(rArB&0x0F);
            long long d = fetch8Bytes(rom);
            *rA = RAM[*rB];
            break;
        }
        case addq:{
            Byte rArB = rom[PC++];
            resetFlags();
            long long* rA = getRegister(rArB&0xF0);
            long long* rB = getRegister(rArB&0x0F);
            if(*rA > LLONG_MAX - *rB){
                OF = 1;
            }
            *rB += *rA;
            setFlags(rB);
            break;
        }
        case subq:{
            Byte rArB = rom[PC++];
            long long* rA = getRegister(rArB&0xF0);
            long long* rB = getRegister(rArB&0x0F);
            if(*rB > LLONG_MIN + *rA){
                OF = 1;
            }
            *rB -= *rA;
            setFlags(rB);
            break;
        }
        case andq:{
            Byte rArB = rom[PC++];
            long long* rA = getRegister(rArB&0xF0);
            long long* rB = getRegister(rArB&0x0F);
            *rB = *rB&*rA;
            setFlags(rB);
            break;
        }
        case xorq:{
            Byte rArB = rom[PC++];
            long long* rA = getRegister(rArB&0xF0);
            long long* rB = getRegister(rArB&0x0F);
            *rB = *rB^*rA;
            setFlags(rB);
            break;
        }
        case jmp:{
            long long valC = fetch8Bytes(rom);
            PC = valC;
            break;
        }
        // case jle:
        // break;
        // case jl:
        // break;
        // case je:
        // break;
        // case jne:
        // break;
        // case jge:
        // break;
        // case jg:
        // break;
        // case call:
        // break;
        // case ret:
        // break;
        // case pushq:
        // break;
        // case popq:
        // break;
        default:{
            cout << "Instruction unrecognized" << endl;
            break;
        }
        }
    }
};



int main(){
    CPU cpu;
    MEM rom;
    cpu.rax = 0x64;
    cpu.rcx = 0x2;
    cout << "%rax: " << hex << cpu.rax << endl;
    cout << "%rcx: " << hex << cpu.rcx << endl;
    cout << "Memory at 0x2: " << hex << cpu.RAM[0x2] << endl;
    rom.mem[0] = 0x40;
    rom.mem[1] = 0x01;
    // rom.mem[2] = 0x01;
    // rom.mem[3] = 0x23;
    // rom.mem[4] = 0x45;
    // rom.mem[5] = 0x67;
    // rom.mem[6] = 0x89;
    // rom.mem[7] = 0xab;
    // rom.mem[8] = 0xcd;
    // rom.mem[9] = 0xef;
    cpu.execute(rom);
    cout << "%rax: " << hex << cpu.rax << endl;
    cout << "%rcx: " << hex << cpu.rcx << endl;
    cout << "Memory at 0x2: " << hex << cpu.RAM[0x2] << endl;
}
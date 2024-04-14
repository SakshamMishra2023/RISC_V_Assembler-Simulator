#include <iostream>
#include <cmath>
using namespace std ;

int BinaryToInteger(string binary, int len){
    // given a string of bits and it's length, it returns it's integer value, assuming 2's complement system
    int power = 1; int ans = 0 ;
    int bit , value ;
    
    int number = stoi(binary) ;
    for(int i = 0; i < len; i++){
        bit = number % 10 ;
        value = bit * power ;
        ans = ans + value ;
        // preparing for next iteration
        power = 2 * power ;
        number = number / 10 ;
    }
    
    if(binary[0] == '1'){
        // negative number
        // power == pow(2, len) ;
        ans = power - ans ;
        ans = - ans ;
    }
    
    return ans ;
}


string Substring(string input, int start_idx, int len){
    // Returns a substring of input string starting from start_idx and has length len
    string ans = "" ;
    int end_idx = start_idx + len - 1 ;
    int curr_idx = start_idx ;
    while(curr_idx <= end_idx){
        ans = ans + input[curr_idx] ;
        curr_idx++;
    }
    return ans ;
}

string decimalToBinary32(int decimal) {
    std::bitset<32> binaryRepresentation;

    if (decimal < 0) {
        // Calculate the two's complement
        int positiveEquivalent = 4294967296 + decimal; // 2^12
        binaryRepresentation = bitset<32>(decimal);
    } else {
        // Positive number: directly convert to binary
        binaryRepresentation = std::bitset<32>(decimal);
    }

    return binaryRepresentation.to_string();
}

void I_Type_Executer(string instruction, string register_array[]){
    // Given an I type string, this executes it, i.e. does the needed changes in PC and File registers.

    string imm_binary = Substring(instruction, 0, 12) ; // bits of the immediate
    int imm_val = BinaryToInteger(imm_binary, 12) ; // integer value of immediate

    // string opcode = Substring(instruction, 25, 7) ;
    // string func3 = Substring(instruction, 17, 3) ;

    string rs_bin = Substring(instruction, 12, 5) ; // binary representation of rs1
    int rs = BinaryToInteger(rs_bin, 5) ; /* this is the corresponding number of the 
    register in the file registers. */
    string rd_bin = Substring(instruction, 20, 5) ; // same story now for rd
    int rd = BinaryToInteger(rd_bin, 5) ;

    string opcode = "" ; 
    if(opcode == "addi"){
        int rs_integer_value = BinaryToInteger(register_array[rs], 32) ; // integral value of rs

        int rd_int_answer = rs_integer_value + imm_val ; // Integral value of rd 
        string rd_bin_answer = decimalToBinary32(rd_int_answer) ; // This is what we'll store back into rd
        register_array[rd] = rd_bin_answer ;
    }
    else if(opcode == "sltiu"){
        string unsigned_rs_binary = '0' + register_array[rs] ; // we need unsigned int value of rs 
        string unsigned_rd_binary = '0' + register_array[rd] ; // we need unsigned int value of rd
        string unsigned_imm_binary = '0' + imm_binary ; // we need unsigned int value of rd

        int rs_unsigned_int = BinaryToInteger(unsigned_rs_binary , 33) ; // unsigned integral value of rs
        int rd_unsigned_int = BinaryToInteger(unsigned_rd_binary, 33) ; // unsigned integral value of rd
        int imm_unsigned_int = BinaryToInteger(unsigned_imm_binary, 13) ; // unsigned integral value of imm
        if(rs_unsigned_int < imm_unsigned_int){
            register_array[rd] = decimalToBinary32(1) ;
        }
    }
    else if(opcode == "jalr"){

    }

}

string is_I_type(string instruction){
    /* Given the input string, returns what kind of I-Type instruction it is or  false if not an I-Type */
    string opcode = Substring(instruction, 25, 7) ;
    string func3 = Substring(instruction, 17, 3) ;

    if( opcode == "0010011" && func3 == "000"){
        return "addi" ;
    }
    else if( opcode == "0010011" && func3 == "011"){
        return "sltiu" ;
    } 
    else if(opcode == "1100111" && func3 == "000"){
        return "jalr" ;
    }
    return "false" ; // the instruction is not I-Type at all.
} 




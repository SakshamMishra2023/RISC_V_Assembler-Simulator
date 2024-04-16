#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <bitset>

using namespace std;

string IntToHex32BitString(int value){
    // Ensure value fits within 32 bits
    value &= 0xFFFFFFFF;

    // Create a stringstream to format the hexadecimal string
    std::stringstream ss;
    ss << std::hex << std::setw(8) << std::setfill('0') << value;

    // Convert the hexadecimal string to uppercase
    std::string hex_str = ss.str();
    for (char& c : hex_str) {
        c = toupper(c);
    }

    // Return the formatted hexadecimal string
    return hex_str;
}

string decimalToBinary32(int decimal){
    bitset<32> binaryRepresentation;

    if(decimal < 0){
        // Calculate the two's complement
        int positiveEquivalent = 4294967296 + decimal; // 2^12
        binaryRepresentation = bitset<32>(decimal);
    } 
    else{
        // Positive number: directly convert to binary
        binaryRepresentation = bitset<32>(decimal);
    }

    return binaryRepresentation.to_string();
}

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

string bitwisexor(const string& a, const string& b) {
    // Convert binary strings to integers
    bitset<32> num1(a);
    bitset<32> num2(b);
    // Perform bitwise XOR
    bitset<32> result = (num1 ^ num2);

    // Convert result back to binary string
    return result.to_string();
}

string bitwiseOr(const string& bin1, const string& bin2) {
    // Ensure both strings have the same length
    int length = max(bin1.length(), bin2.length());
    string result(length, '0');

    // Perform bitwise OR
    for (int i = 0; i < length; ++i) {
        int bit1 = i < bin1.length() ? bin1[bin1.length() - 1 - i] - '0' : 0;
        int bit2 = i < bin2.length() ? bin2[bin2.length() - 1 - i] - '0' : 0;
        result[length - 1 - i] = (bit1 | bit2) + '0';
    }

    return result;
}

string bitwiseand(const string& binary1, const string& binary2) {
    // Convert binary strings to integers
    bitset<32> bits1(binary1);
    bitset<32> bits2(binary2);
    
    // Perform bitwise AND operation
    bitset<32> result = bits1 & bits2;
    
    // Convert result back to binary string
    return result.to_string();
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
void b_simu(string &bcode, map<string, long> register_map,map<int, string> register_add_map, int &pc){
    //beq
    if(bcode.substr(17,3) == "000"){
        string dummy1 = '0' + bcode.substr(7,5);
        int reg_no1 = BinaryToInteger(dummy1, 6);
        int reg2 = register_map[register_add_map[reg_no1]];
        string dummy2 = '0' + bcode.substr(12,5);
        int reg_no2 = BinaryToInteger(dummy2, 6);
        int reg1 = register_map[register_add_map[reg_no2]];

        if(reg1 == reg2){
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4);
        int imm = BinaryToInteger(binary_imm, 12);
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 1;
        }
        return;

    }
    //bne
    else if(bcode.substr(17,3) == "001"){

        string dummy1 = '0' + bcode.substr(7,5);
        int reg_no1 = BinaryToInteger(dummy1, 6);
        int reg2 = register_map[register_add_map[reg_no1]];

        string dummy2 = '0' + bcode.substr(12,5);
        int reg_no2 = BinaryToInteger(dummy2, 6);
        int reg1 = register_map[register_add_map[reg_no2]];

        if(reg2 != reg1){
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4);
        int imm = BinaryToInteger(binary_imm, 12);
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 1;
        }
        return;

    }
    //bge
    else if(bcode.substr(17,3) == "101"){

        string dummy1 = '0' + bcode.substr(7,5);
        int reg_no1 = BinaryToInteger(dummy1, 6);
        int reg2 = register_map[register_add_map[reg_no1]];

        string dummy2 = '0' + bcode.substr(12,5);
        int reg_no2 = BinaryToInteger(dummy2, 6);
        int reg1 = register_map[register_add_map[reg_no2]];

        if(reg2 <= reg1){
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4);
        int imm = BinaryToInteger(binary_imm, 12);
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 1;
        }
        return;

    }
    //bgeu
    else if(bcode.substr(17,3) == "111"){
        int imm;
        string dummy1 = '0' + bcode.substr(7,5);
        int reg_no1 = BinaryToInteger(dummy1, 6);
        int reg2 = register_map[register_add_map[reg_no1]];
        string dummy2 = '0' + bcode.substr(12,5);
        int reg_no2 = BinaryToInteger(dummy2, 6);
        int reg1 = register_map[register_add_map[reg_no2]];
        if(reg1 < 0){
            reg1 =  4294967296 + reg1;
        }
        if(reg2 < 0){
            reg2 = 4294967296 + reg2;
        }

        if(reg1 >= reg2){
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4);
        if(binary_imm[0] == '1'){
            binary_imm[0] = '0';
            imm = BinaryToInteger(binary_imm,12);
            imm = 0 - imm;

        }
        else{
            imm = BinaryToInteger(binary_imm, 12);

        }
        imm = BinaryToInteger(binary_imm, 12);
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 1;
        }
        return;

    }
    //blt
    else if(bcode.substr(17,3) == "100"){
        string dummy1 = '0' + bcode.substr(7,5);
        int reg_no1 = BinaryToInteger(dummy1, 6);
        int reg2 = register_map[register_add_map[reg_no1]];
        string dummy2 = '0' + bcode.substr(12,5);
        int reg_no2 = BinaryToInteger(dummy2, 6);
        int reg1 = register_map[register_add_map[reg_no2]];
        if(reg2 > reg1){
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4);
        int imm = BinaryToInteger(binary_imm, 12);
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 1;
        }
        return;

    }
    //bltu
    else if(bcode.substr(17,3) == "110"){
        int imm;
        string dummy1 = '0' + bcode.substr(7,5);
        int reg_no1 = BinaryToInteger(dummy1, 6);
        int reg2 = register_map[register_add_map[reg_no1]];
        string dummy2 = '0' + bcode.substr(12,5);
        int reg_no2 = BinaryToInteger(dummy2, 6);
        int reg1 = register_map[register_add_map[reg_no2]];

        if(reg1 < 0){
            reg1 =  4294967296 + reg1;
        }
        if(reg2 < 0){
            reg2 = 4294967296 + reg2;
        }

        if(reg1 < reg2){
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4);
        int imm = BinaryToInteger(binary_imm, 12);
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 1;
        }
        return;

    }
}

void j_simu(string &bcode, map<string, long> register_map,map<int, string> register_add_map, int &pc){
    string imm = bcode.substr(0,1)+ bcode.substr(12,8) + bcode.substr(11,1) + bcode.substr(1,10);
    int imm_val = BinaryToInteger(imm, 20);
    string dummy = '0' + bcode.substr(20,5);
    int reg_no = BinaryToInteger(dummy, 6);

    register_map[register_add_map[reg_no]] = pc + 4;
    pc = pc + imm_val;
    if(pc % 2 != 0){
        pc = pc - 1;

    }
    return;

}
void r_simu(string &bcode , map<string,long> register_map, map<int, string> register_add_map, int &pc){

    string dummy1 = '0' + bcode.substr(7,5);
    int reg_no1 = BinaryToInteger(dummy1, 6);
    long reg2 = register_map[register_add_map[reg_no1]];

    string dummy2 = '0' + bcode.substr(12,5);
    int reg_no2 = BinaryToInteger(dummy2, 6);
    long reg1 = register_map[register_add_map[reg_no2]];

    string dummy3 = '0' + bcode.substr(20,5);
    long reg_no3 = BinaryToInteger(dummy2, 6);
    //register_map[register_add_map[reg_no3]];

    if(bcode.substr(0,7)=="0000000" && bcode.substr(17,3) == "000"){
        register_map[register_add_map[reg_no3]] = reg1 + reg2;
        pc = pc + 1;
        
    }
    else if(bcode.substr(0,7)=="0100000" && bcode.substr(17,3) == "000"){
        register_map[register_add_map[reg_no3]] = reg1 - reg2;
        pc = pc + 1;
        
    }
    //sll
    else if(bcode.substr(17,3) == "001"){
         string binar_reg2 = decimalToBinary32(reg2);
         string dummy1 = '0' + binar_reg2.substr(27,5);
        int shift = BinaryToInteger(dummy1, 6);

        string  binar_reg1= decimalToBinary32(reg1);
        string new_reg1 = binar_reg1.substr(shift - 1, 32 - shift);
        for(int i=0;i<shift;i++){
            new_reg1= new_reg1 + '0';
        }
        long final = BinaryToInteger(new_reg1, 32);
        register_map[register_add_map[reg_no3]] = final;

        pc =pc+1;

    }
    //srl
    else if(bcode.substr(17,3) == "101"){
         string binar_reg2 = decimalToBinary32(reg2);
         string dummy1 = '0' + binar_reg2.substr(27,5);
        int shift = BinaryToInteger(dummy1, 6);

        string  binar_reg1= decimalToBinary32(reg1);
        string new_reg1 = binar_reg1.substr(0, 32 - shift);
        for(int i=0;i<shift;i++){
            new_reg1= '0'+ new_reg1;
        }
        long final = BinaryToInteger(new_reg1, 32);
        register_map[register_add_map[reg_no3]] = final;

        pc =pc+1;

    }
    //or
    else if(bcode.substr(17,3) == "110"){
        string reg1_bin = decimalToBinary32(reg1);
        string reg2_bin = decimalToBinary32(reg2);
        string new_reg_bin = bitwiseOr(reg1_bin,reg2_bin);
        long reg_3_int = BinaryToInteger(new_reg_bin,32);
        register_map[register_add_map[reg_no3]] = reg_3_int;

        pc= pc + 1;


    }
    //and
    else if(bcode.substr(17,3) == "110"){

        string reg1_bin = decimalToBinary32(reg1);
        string reg2_bin = decimalToBinary32(reg2);
        string new_reg_bin = bitwiseand(reg1_bin,reg2_bin);
        long reg_3_int = BinaryToInteger(new_reg_bin,32);
        register_map[register_add_map[reg_no3]] = reg_3_int;
        pc= pc +1 ;

    }
    //xor
    else if(bcode.substr(17,3) == "110"){

        string reg1_bin = decimalToBinary32(reg1);
        string reg2_bin = decimalToBinary32(reg2);
        string new_reg_bin = bitwisexor(reg1_bin,reg2_bin);
        long reg_3_int = BinaryToInteger(new_reg_bin,32);
        register_map[register_add_map[reg_no3]] = reg_3_int;
        pc= pc + 1 ;

    }
    //slt
    else if(bcode.substr(17,3) == "010"){
        if(reg2>reg1){
            register_map[register_add_map[reg_no3]] = 1;
        }
        pc = pc + 1;
    }

    //sltu
    else if(bcode.substr(17,3) == "010"){

        if(reg1 < 0){
            reg1 =  4294967296+ reg1;
        }
        if(reg2 < 0){
            reg2 = 4294967296+ reg2;
        }
        
        if(reg2>reg1){
            register_map[register_add_map[reg_no3]] = 1;
        }
        pc = pc + 1;
    }
    


}

void s_simu(string &bcode , map<string,long> register_map, map<int, string> register_add_map, int &pc,map<string, int> &program_mem){
    if(bcode.substr(17,3) == "010"){
        string imm_val = bcode.substr(0,7) + bcode.substr(20,5);
        int imm = BinaryToInteger(imm_val,12);

        string reg1 = '0' + bcode.substr(12,5);
        int reg_no1 = BinaryToInteger(reg1, 6);

        string reg2 = '0' + bcode.substr(7,5);
        int reg_no2 = BinaryToInteger(reg2, 6);

        long reg1_val = register_map[register_add_map[reg_no1]];
        long hex_int_add = imm + reg1_val;

        string hex_add = "0x" + IntToHex32BitString(hex_int_add);

        program_mem[hex_add] = reg_no2;
        pc= pc + 1;

    }

}

void u_simu(string &bcode , map<string,long> register_map, map<int, string> register_add_map, int &pc){
    if(bcode.substr(25,7) == "0110111"){
        string imm_val = bcode.substr(0,20) ;
        int imm = BinaryToInteger(imm_val,20);

        string reg = '0' + bcode.substr(20,5);
        int reg_add = BinaryToInteger(reg, 6);

        int reg_fin_val = pc + imm;
        register_map[register_add_map[reg_add]] = reg_fin_val;
        pc = pc + 1; 

    }
    else if(bcode.substr(25,7) == "0010111"){
        string imm_val = bcode.substr(0,20) ;
        int imm = BinaryToInteger(imm_val,20);

        string reg = '0' + bcode.substr(20,5);
        int reg_add = BinaryToInteger(reg, 6);

        int reg_fin_val = imm;
        register_map[register_add_map[reg_add]] = reg_fin_val;
        pc = pc + 1;

    }

}




void I_Type_Executer(string instruction, string register_array[]){
    // Given an I type string, this executes it, i.e. does the needed changes in PC and File registers.

    string imm_binary = Substring(instruction, 0, 12) ; // bits of the immediate
    int imm_val = BinaryToInteger(imm_binary, 12) ; // integer value of immediate

    string rs_bin = Substring(instruction, 12, 5) ; // binary representation of rs1
    int rs = BinaryToInteger(rs_bin, 5) ; /* this is the corresponding number of the 
    register in the file registers. */
    string rd_bin = Substring(instruction, 20, 5) ; // same story now for rd
    int rd = BinaryToInteger(rd_bin, 5) ;

    string opcode = "" ; 
    if(opcode == "addi"){
        int rs_integer_value = BinaryToInteger(register_array[rs], 32) ; // integer value of rs

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

string is_R_type(string instruction){
    //string opcode=substring(instruction,25,7);
    string func3=substring(instruction,17,3);
    string func7=substring(instruction,24,7);
    if(func3=="000" && func7=="0000000") return add;
    else if(func3=="000" && func7=="0100000") return sub;
    else if(func3=="001") return "add";
    else if(func3=="010") return "slt";
    else if(func3=="011") return "sltu";
    else if(func3=="100") return "xor";
    else if(func3=="101") return "srl";
    else if(func3=="110") return "or";
    else if(func3=="111") return "and";
    else{
        string g="this instruction is not R type "
        return g;
    }
    
    
}

void R_executer(string instruction,string register_arr[]){
    string rs1_binary = Substring(instruction, 0, 12) ; // bits of the rs1
    int rs1_value = BinaryToInteger(rs1_binary, 12) ; // integer value of rs1

    string rs2_binary = Substring(instruction, 12, 5) ; // binary representation of rs2
    int rs2_value = BinaryToInteger(rs2_binary, 5) ; /* this is the corresponding number of the 
    register in the file registers. */
    string rd_binary = Substring(instruction, 20, 5) ; // same story now for rd
    int rd_value= BinaryToInteger(rd_binary, 5) ;

    if(is_R_type=="add"){
          int rs1_integer_value = BinaryToInteger(register_arr[rs1_value], 32) ; // integer value of rs
          int rs2_integer_value = BinaryTOInteger(register_arr[rs2_value],32);
          int rd_int_answer = rs1_integer_value + rs2_integer_value ; // Integral value of rd 
          string rd_bin_answer = decimalToBinary32(rd_int_answer) ; // This is what we'll store back into rd
          register_array[rd] = rd_bin_answer ;
    }
   else if(is_R_type=="sub"){
        // rs is not specified in whwther 1 or 2
        // i consider with rs1
         int rs1_integer_value = BinaryToInteger(register_arr[rs1_value], 32);
         int rd_int_answer = 0-rs1_integer_value ;
         string rd_bin_answer = decimalToBinary32(rd_int_answer) ; // This is what we'll store back into rd
         register_array[rd] = rd_bin_answer ;
    }
    else if(is_R_type=="sub"){
        int rs1_integer_value = BinaryToInteger(register_arr[rs1_value], 32);
        int rs2_integer_value = BinaryTOInteger(register_arr[rs2_value],32);
        int rd_int_answer = rs1_integer_value-rs2_integer_value ;
        string rd_bin_answer = decimalToBinary32(rd_int_answer) ; // This is what we'll store back into rd
        register_array[rd] = rd_bin_answer ;
    }
     else if(is_R_type=="slt"){
        int rs1_integer_value = BinaryToInteger(register_arr[rs1_value], 32);
        int rs2_integer_value = BinaryTOInteger(register_arr[rs2_value],32);
        bool answer = rs1_integer_value<rs2_integer_value ;
        if(answer==true){
            int rd_int_answer=1;
        }
        string rd_bin_answer = decimalToBinary32(rd_int_answer) ; // This is what we'll store back into rd
        register_array[rd] = rd_bin_answer ;
    }
     else if(is_R_type=="sltu"){
        int rs1_integer_value = BinaryToInteger(register_arr[rs1_value], 32);
        int rs2_integer_value = BinaryTOInteger(register_arr[rs2_value],32);
        if(rs1_integer_value<0){
            rs1_integer_value=rs1_integer_value*-1;
        }
        if(rs2_integer_value<0){
            rs2_integer_value=rs2_integer_value*-1;
        }
        bool answer = rs1_integer_value<rs2_integer_value ;
        if(answer==true){
            int rd_int_answer=1;
        }
        string rd_bin_answer = decimalToBinary32(rd_int_answer) ; // This is what we'll store back into rd
        register_array[rd] = rd_bin_answer ;
    }
    else if(is_R_type=="xor"){
         string rd_bin_answer = bitwisexor(rs1_binary,rs2_binary) ; // Integral value of rd 
         //string rd_bin_answer = decimalToBinary32(rd_int_answer) ; // This is what we'll store back into rd
         register_array[rd] = rd_bin_answer ;
    }
    else if(is_R_type=="sll"){
        
    }
    else if(is_R_type=="srl"){
        
    }
    else if(is_R_type=="or"){
        string rd_bin_answer = bitwiseor(rs1_binary,rs2_binary);
        register_array[rd] = rd_bin_answer ;
    }
    else if(is_R_type=="and"){
        string rd_bin_answer = bitwiseand(rs1_binary,rs2_binary);
        register_array[rd] = rd_bin_answer ;
    }
}
void classifier(string & bcode,map<string, long> register_map, map<int, string> register_add_map, int & pc){
    string opcode;
    opcode = bcode.substr(26,6);
    if(opcode == "0110011"){
        r_simu(bcode,register_map,register_add_map,pc );
    }
    else if(opcode == "0000011" || opcode == "0010011" || opcode == "1100111"){
        i_simu(bcode,register_map,register_add_map,pc );

    }
    else if(opcode == "0100011"){
        s_simu(bcode, register_map, register_add_map, pc, program_mem );

    }
    else if(opcode == "1100011"){
        b_simu(bcode, register_map, register_add_map, pc );

    }
    else if(opcode == "0110111" || opcode == "0010111"){
        u_simu(bcode, register_map, register_add_map, pc );

    }

    else if(opcode == "1101111"){
        j_simu(bcode, register_map, register_add_map, pc );
    }

    else if(opcode == ""){

    }



}





int main(){

    map<string, long> register_map;

    register_map["zero"] = 0;
    register_map["ra"] = 0;
    register_map["sp"] = 0;
    register_map["gp"] = 0;
    register_map["tp"] = 0;
    register_map["t0"] = 0;
    register_map["t1"] = 0;
    register_map["t2"] = 0;
    register_map["s0"] = 0;
    register_map["fp"] = 0;
    register_map["s1"] = 0;
    register_map["a0"] = 0;
    register_map["a1"] = 0;
    register_map["a2"] = 0;
    register_map["a3"] = 0;
    register_map["a4"] = 0;
    register_map["a5"] = 0;
    register_map["a6"] = 0;
    register_map["a7"] = 0;
    register_map["s2"] = 0;
    register_map["s3"] = 0;
    register_map["s4"] = 0;
    register_map["s5"] = 0;
    register_map["s6"] = 0;
    register_map["s7"] = 0;
    register_map["s8"] = 0;
    register_map["s9"] = 0;
    register_map["s10"] =0;
    register_map["s11"] = 0;
    register_map["t3"] = 0;
    register_map["t4"] = 0;
    register_map["t5"] = 0;
    register_map["t6"] = 0;

    map<int, string> register_add_map;

    register_add_map[0] = "zero";
    register_add_map[1] = "ra";
    register_add_map[2] = "sp";
    register_add_map[3] = "gp";
    register_add_map[4] = "tp";
    register_add_map[5] = "t0";
    register_add_map[6] = "t1";
    register_add_map[7] = "t2";
    register_add_map[8] = "s0";
    register_add_map[8] = "fp";
    register_add_map[9] = "s1";
    register_add_map[10] ="a0";
    register_add_map[11] ="a1";
    register_add_map[12] ="a2";
    register_add_map[13] ="a3";
    register_add_map[14] ="a4";
    register_add_map[15] ="a5";
    register_add_map[16] ="a6";
    register_add_map[17] ="a7";
    register_add_map[18] ="s2";
    register_add_map[19] ="s3";
    register_add_map[20] ="s4";
    register_add_map[21] ="s5";
    register_add_map[22] ="s6";
    register_add_map[23] ="s7";
    register_add_map[24] ="s8";
    register_add_map[25] ="s9";
    register_add_map[26] ="s10";
    register_add_map[27] = "s11";
    register_add_map[28] = "t3";
    register_add_map[29] = "t4";
    register_add_map[30] = "t5";
    register_add_map[31] = "t6";

    map<string, int> program_mem;
    
    program_mem["0x00010000"] = 0;
    program_mem["0x00010004"] = 0;
    program_mem["0x00010008"] = 0;
    program_mem["0x0001000c"] = 0;
    program_mem["0x00010010"] = 0;
    program_mem["0x00010014"] = 0;
    program_mem["0x00010018"] = 0;
    program_mem["0x0001001c"] = 0;
    program_mem["0x00010020"] = 0;
    program_mem["0x00010024"] = 0;
    program_mem["0x00010028"] = 0;
    program_mem["0x0001002c"] = 0;
    program_mem["0x00010030"] = 0;
    program_mem["0x00010034"] = 0;
    program_mem["0x00010038"] = 0;
    program_mem["0x0001003c"] = 0;
    program_mem["0x00010040"] = 0;
    program_mem["0x00010044"] = 0;
    program_mem["0x00010048"] = 0;
    program_mem["0x0001004c"] = 0;
    program_mem["0x00010050"] = 0;
    program_mem["0x00010054"] = 0;
    program_mem["0x00010058"] = 0;
    program_mem["0x0001005c"] = 0;
    program_mem["0x00010060"] = 0;
    program_mem["0x00010064"] = 0;
    program_mem["0x00010068"] = 0;
    program_mem["0x0001006c"] = 0;
    program_mem["0x00010070"] = 0;
    program_mem["0x00010074"] = 0;
    program_mem["0x00010078"] = 0;
    program_mem["0x0001007c"] = 0;

    

int pc= 0;
map <int, string> instruction;
int total = -1;

ifstream input_file("inp.txt");
ofstream output_file("out.txt");

string line;

while(getline(input_file, line)){
    if(!line.empty() || !(line == "00000000000000000000000001100011")){
        total++;
        instruction[total] = line;
    
    }

}

while(pc <= total){

    classifier(instruction[pc], register_map, register_add_map, pc, program_mem);
    if(output_file.is_open()){
        string pc_bin = "0b" + decimalToBinary32(pc);
        cout << pc_bin << " ";
    
        for(int i = 0; i< 32;i++){
            string reg_bin = "0b" + decimalToBinary32(register_map[register_add_map[i]]);
            cout<< reg_bin <<" ";   
        }
        cout<<endl;
    }
    
}
cout <<"0x00010000:" << program_mem["0x00010000"] << endl;
cout <<"0x00010004:" << program_mem["0x00010004"] << endl;
cout <<"0x00010008:" << program_mem["0x00010008"] << endl;
cout <<"0x0001000c:" << program_mem["0x0001000c"] << endl;
cout <<"0x00010010:" << program_mem["0x00010010"] << endl;
cout <<"0x00010014:" << program_mem["0x00010014"] << endl;
cout <<"0x00010018:" << program_mem["0x00010018"] << endl;
cout <<"0x0001001c:" << program_mem["0x0001001c"] << endl;
cout <<"0x00010020:" << program_mem["0x00010020"] << endl;
cout <<"0x00010024:" << program_mem["0x00010024"] << endl;
cout <<"0x00010028:" << program_mem["0x00010028"] << endl;
cout <<"0x0001002c:" << program_mem["0x0001002c"] << endl;
cout <<"0x00010030:" << program_mem["0x00010030"] << endl;
cout <<"0x00010034:" << program_mem["0x00010034"] << endl;
cout <<"0x00010038:" << program_mem["0x00010038"] << endl;
cout <<"0x0001003c:" << program_mem["0x0001003c"] << endl;
cout <<"0x00010040:" << program_mem["0x00010040"] << endl;
cout <<"0x00010044:" << program_mem["0x00010044"] << endl;
cout <<"0x00010048:" << program_mem["0x00010048"] << endl;
cout <<"0x0001004c:" << program_mem["0x0001004c"] << endl;
cout <<"0x00010050:" << program_mem["0x00010050"] << endl;
cout <<"0x00010054:" << program_mem["0x00010054"] << endl;
cout <<"0x00010058:" << program_mem["0x00010058"] << endl;
cout <<"0x0001005c:" << program_mem["0x0001005c"] << endl;
cout <<"0x00010060:" << program_mem["0x00010060"] << endl;
cout <<"0x00010064:" << program_mem["0x00010064"] << endl;
cout <<"0x00010068:" << program_mem["0x00010068"] << endl;
cout <<"0x0001006c:" << program_mem["0x0001006c"] << endl;
cout <<"0x00010070:" << program_mem["0x00010070"] << endl;
cout <<"0x00010074:" << program_mem["0x00010074"] << endl;
cout <<"0x00010078:" << program_mem["0x00010078"] << endl;
cout <<"0x0001007c:" << program_mem["0x0001007c"] << endl;

output_file.close();
    return 0;
}

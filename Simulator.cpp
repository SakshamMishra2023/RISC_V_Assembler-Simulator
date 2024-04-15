#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>

using namespace std;

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


void b_simu(string &bcode, map<string, int> register_map,map<int, string> register_add_map, int &pc){
    //beq
    if(bcode.substr(17,3) == "000"){
        if(register_map [bcode.substr(8,5)] == register_map[bcode.substr(13,5)]){
        string binary_imm = bcode.substr(0,1) + bcode.substr(25,1)  + bcode.substr(1,6) + bcode.substr(22, 4);
        int imm = BinaryToInteger(binary_imm, 32);
        pc = pc + imm;
        return;
        }
        return;

    }
    //bne
    else if(bcode.substr(17,3) == "001"){
        if(register_map [bcode.substr(8,5)] != register_map[bcode.substr(13,5)]){
        string binary_imm = bcode.substr(0,1) + bcode.substr(25,1)  + bcode.substr(1,6) + bcode.substr(22, 4);
        int imm = BinaryToInteger(binary_imm, 32);
        pc = pc + imm;
        return;
        }
        return;

    }
    //blt
    else if(bcode.substr(17,3) == "100"){
        if(register_map [bcode.substr(8,5)] <= register_map[bcode.substr(13,5)]){
        string binary_imm = bcode.substr(0,1) + bcode.substr(25,1)  + bcode.substr(1,6) + bcode.substr(22, 4);
        int imm = BinaryToInteger(binary_imm, 32);
        pc = pc + imm;
        return;
        }
        return;

    }
    //bge
    else if(bcode.substr(17,3) == "101"){
        int imm;
        int reg2 = register_map[bcode.substr(8,5)];
        int reg1 = register_map[bcode.substr(13,5)];
        if(reg1 < 0){
            reg1 =  4294967296- reg1;
        }
        if(reg2 < 0){
            reg2 = 4294967296- reg2;
        }

        if(reg1 >= reg2){
        string binary_imm = bcode.substr(0,1) + bcode.substr(25,1)  + bcode.substr(1,6) + bcode.substr(22, 4);
        if(binary_imm[0] == '1'){
            binary_imm[0] = '0';
            imm = BinaryToInteger(binary_imm,32);
            imm = 0 - imm;

        }
        else{
            imm = BinaryToInteger(binary_imm, 32);

        }
        imm = BinaryToInteger(binary_imm, 32);
        pc = pc + imm;
        return;
        }
        return;

    }
    //bltu
    else if(bcode.substr(17,3) == "110"){
        if(register_map [bcode.substr(8,5)] > register_map[bcode.substr(13,5)]){
        string binary_imm = bcode.substr(0,1) + bcode.substr(25,1)  + bcode.substr(1,6) + bcode.substr(22, 4);
        int imm = BinaryToInteger(binary_imm, 32);
        pc = pc + imm;
        return;
        }
        return;

    }
    //bgeu
    else if(bcode.substr(17,3) == "111"){
        int imm;
        int reg2 = register_map[bcode.substr(8,5)];
        int reg1 = register_map[bcode.substr(13,5)];
        if(reg1 < 0){
            reg1 =  4294967296- reg1;
        }
        if(reg2 < 0){
            reg2 = 4294967296- reg2;
        }

        if(reg1 < reg2){
        string binary_imm = bcode.substr(0,1) + bcode.substr(25,1)  + bcode.substr(1,6) + bcode.substr(22, 4);
        int imm = BinaryToInteger(binary_imm, 32);
        pc = pc + imm;
        return;
        }
        return;

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


void classifier(string & bcode,map<string, int> register_map, map<int, string> register_add_map, int & pc){
    string opcode;
    opcode = bcode.substr(26,6);
    if(opcode == "0110011"){
        r_simu(bcode,register_map,register_add_map,pc );
    }
    else if(opcode == "0000011" || opcode == "0010011" || opcode == "1100111"){
        i_simu(bcode,register_map,register_add_map,pc );

    }
    else if(opcode == "0100011"){
        s_simu(bcode, register_map, register_add_map,pc );

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

    map<string, int> register_map;

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

    

int pc;
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
    classifier(instruction[pc], register_map, register_add_map, pc);
    pc = pc + 1;

}

    return 0;
}


/*
string is_S_type(string instruction){
    /* Given the input string, returns if it is S-Type instruction or not */
    string opcode= substring(instruction,25,7);
    string fucn3= substring(instruction,17,3);

    if(opcode == "0100011" && fucn3 == "010"){
        return "sw";
    }
    return "false";
}
*/




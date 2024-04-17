#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <bitset>
#include <iomanip>

using namespace std;

long long BinaryToInteger(string& binary, int len) {
    long long ans = 0;

    // Determine the sign of the number
    bool is_negative = (binary[0] == '1');

    // Start from the most significant bit
    long long power = 1LL << (len - 1);

    // Loop through each character of the binary string
    for (char bit : binary) {
        if (bit == '1') {
            ans += power;
        }
        power >>= 1; // Move to the next lower bit
    }

    // Handle negative numbers (two's complement)
    if (is_negative) {
        ans = -(1LL << len) + ans;
    }

    return ans;
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

string IntToHex32BitString(int value){
    // Ensure value fits within 32 bits
    value &= 0xFFFFFFFF;

    // Create a stringstream to format the hexadecimal string
    std::stringstream ss;
    ss << std::hex << std::setw(8) << std::setfill('0') << value;

    // Convert the hexadecimal string to uppercase
    std::string hex_str = ss.str();
    

    // Return the formatted hexadecimal string
    return hex_str;
}


std::string decimalToBinary32(int decimal) {
    std::bitset<32> binaryRepresentation;

    if (decimal < 0) {
        // Calculate the two's complement
        int positiveEquivalent = 4294967296 + decimal; // 2^32
        binaryRepresentation = std::bitset<32>(positiveEquivalent);
    } else {
        // Positive number: directly convert to binary
        binaryRepresentation = std::bitset<32>(decimal);
    }

    return binaryRepresentation.to_string();
}


long long BinaryToInteger1(string& binary, int len){
    long long ans = 0;

    // Start from the most significant bit
    long long power = 1 << (len - 1);

    // Loop through each character of the binary string
    for (char bit : binary) {
        if (bit == '1') {
            ans += power;
        }
        power >>= 1; // Move to the next lower bit
    }

    // Handle negative numbers (two's complement)
    if (binary[0] == '1') {
        ans -= (1 << len);
    }

    return ans;
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
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4) +'0';
        int imm = BinaryToInteger(binary_imm, 13);
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 4;
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
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4) +'0';
        int imm = BinaryToInteger(binary_imm, 13);
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 4;
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
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4) + '0';
        int imm = BinaryToInteger(binary_imm, 13);
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 4;
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
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4)+ '0';
        if(binary_imm[0] == '1'){
            string new_dum = '0' + binary_imm.substr(1,12);
            imm = BinaryToInteger(new_dum,13);
            imm = 0 - imm;

        }
        else{
            imm = BinaryToInteger(binary_imm, 13);

        }
        
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 4;
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
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4)+'0';
        int imm = BinaryToInteger(binary_imm, 13);
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 4;
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
        string binary_imm = bcode.substr(0,1) + bcode.substr(24,1)  + bcode.substr(1,6) + bcode.substr(20, 4)+'0';
        int imm = BinaryToInteger(binary_imm, 13);
        pc = pc + imm;
        return;
        }
        else{
            pc = pc + 4;
        }
        return;

    }
}

void j_simu(string &bcode, map<string, long> &register_map,map<int, string> &register_add_map, int &pc){
    string imm = bcode.substr(0,1)+ bcode.substr(12,8) + bcode.substr(11,1) + bcode.substr(1,10) + '0';
    int imm_val = BinaryToInteger(imm, 21);
    string dummy = '0' + bcode.substr(20,5);
    int reg_no = BinaryToInteger(dummy, 6);

    register_map[register_add_map[reg_no]] = pc + 4;
    pc = pc + imm_val;
    if(pc % 2 != 0){
        pc = pc - 1;

    }
    return;

}

void r_simu(string &bcode , map<string,long> &register_map, map<int, string> &register_add_map, int &pc){

    string dummy2 = '0' + bcode.substr(7,5);
    int reg_no2 = BinaryToInteger(dummy2, 6);
    long reg2 = register_map[register_add_map[reg_no2]];

    string dummy1 = '0' + bcode.substr(12,5);
    int reg_no1 = BinaryToInteger(dummy1, 6);
    long reg1 = register_map[register_add_map[reg_no1]];

    string dummy3 = '0' + bcode.substr(20,5);
    long reg_no3 = BinaryToInteger(dummy3, 6);
    //register_map[register_add_map[reg_no3]];

    if(bcode.substr(0,7)=="0000000" && bcode.substr(17,3) == "000"){
       //  register_map[register_add_map[reg_no3]] = reg1 + reg2;
        long ans = reg1 + reg2 ;
        long long max = pow(2, 31) ; // max positive value of 32 bits with 2's complement 
        if(ans >= max){
            // overflow has occurred
            ans = ans - max ;
        }
        register_map[register_add_map[reg_no3]] = ans ;
        pc = pc + 4 ;         
    }
    else if(bcode.substr(0,7)=="0100000" && bcode.substr(17,3) == "000"){
        register_map[register_add_map[reg_no3]] = reg1 - reg2;
        pc = pc + 4;
        
    }
    //sll
    else if(bcode.substr(17,3) == "001"){
         string binar_reg2 = decimalToBinary32(reg2);
         string dummy1 = '0' + binar_reg2.substr(27,5);
        long long shift = BinaryToInteger(dummy1, 6);

        string  binar_reg1= decimalToBinary32(reg1);
        //cout<<"here"<<reg1<<endl;
        string new_reg1 = binar_reg1.substr(shift , 32-shift);
        for(int i=0;i<shift;i++){
            new_reg1= new_reg1 + '0';
        }
        //cout<<new_reg1<<endl;
        long long final = BinaryToInteger(new_reg1, 32);
        //cout<<final<<endl;
        register_map[register_add_map[reg_no3]] = final;

        pc =pc+4;
        //cout<<pc;

    }
    //srl
    else if(bcode.substr(17,3) == "101"){
         string binar_reg2 = decimalToBinary32(reg2);
         string dummy1 = '0' + binar_reg2.substr(27,5);
        long long shift = BinaryToInteger(dummy1, 6);

        string  binar_reg1= decimalToBinary32(reg1);
        string new_reg1 = binar_reg1.substr(0, 32 - shift);
        for(int i=0;i<shift;i++){
            new_reg1= '0'+ new_reg1;
        }
        long final = BinaryToInteger(new_reg1, 32);
        register_map[register_add_map[reg_no3]] = final;

        pc =pc+4;

    }
    //or
    else if(bcode.substr(17,3) == "110"){
        string reg1_bin = decimalToBinary32(reg1);
        string reg2_bin = decimalToBinary32(reg2);
        string new_reg_bin = bitwiseOr(reg1_bin,reg2_bin);
        long reg_3_int = BinaryToInteger(new_reg_bin,32);
        register_map[register_add_map[reg_no3]] = reg_3_int;

        pc= pc + 4;


    }
    //and
    else if(bcode.substr(17,3) == "111"){

        string reg1_bin = decimalToBinary32(reg1);
        string reg2_bin = decimalToBinary32(reg2);
        string new_reg_bin = bitwiseand(reg1_bin,reg2_bin);
        long reg_3_int = BinaryToInteger(new_reg_bin,32);
        register_map[register_add_map[reg_no3]] = reg_3_int;
        pc= pc +4 ;

    }
    //xor
    else if(bcode.substr(17,3) == "100"){

        string reg1_bin = decimalToBinary32(reg1);
        string reg2_bin = decimalToBinary32(reg2);
        string new_reg_bin = bitwisexor(reg1_bin,reg2_bin);
        long reg_3_int = BinaryToInteger(new_reg_bin,32);
        register_map[register_add_map[reg_no3]] = reg_3_int;
        pc= pc + 4 ;

    }
    //slt
    else if(bcode.substr(17,3) == "010"){
        if(reg2>reg1){
            register_map[register_add_map[reg_no3]] = 1;
        }
        pc = pc + 4;
    }

    //sltu
    else if(bcode.substr(17,3) == "011"){

        if(reg1 < 0){
            reg1 =  4294967296+ reg1;
        }
        if(reg2 < 0){
            reg2 = 4294967296+ reg2;
        }
        
        if(reg2>reg1){
            register_map[register_add_map[reg_no3]] = 1;
        }
        pc = pc + 4;
    }
    return;
    
}

void s_simu(string &bcode , map<string,long> &register_map, map<int, string> &register_add_map, int &pc,map<string, int> &program_mem){
    if(bcode.substr(17,3) == "010"){
        string imm_val = bcode.substr(0,7) + bcode.substr(20,5);
        int imm = BinaryToInteger(imm_val,12);

        string reg1 = '0' + bcode.substr(12,5);
        int reg_no1 = BinaryToInteger(reg1, 6);

        string reg2 = '0' + bcode.substr(7,5);
        int reg_no2 = BinaryToInteger(reg2, 6);

        long reg1_val = register_map[register_add_map[reg_no1]];
        long hex_int_add = imm + reg1_val;
        cout<<hex_int_add<<endl;;
        string addfd= IntToHex32BitString(hex_int_add);
        cout<<addfd<<endl;
        cout<<register_map[register_add_map[reg_no2]]<<endl;
        

        string hex_add = "0x" + addfd;

        program_mem[hex_add] = register_map[register_add_map[reg_no2]];
        //cout<<pc<<endl;
        pc= pc + 4;
        //cout<<pc<<endl;

    }
    return;

}

void u_simu(string &bcode , map<string,long> &register_map, map<int, string> &register_add_map, int &pc){
    if(bcode.substr(25,7) == "0110111"){
        string imm_val = bcode.substr(0,20) + "000000000000";
        int imm = BinaryToInteger(imm_val,32);

        string reg = '0' + bcode.substr(20,5);
        int reg_add = BinaryToInteger(reg, 6);

        int reg_fin_val =imm;
        register_map[register_add_map[reg_add]] = reg_fin_val;
        //cout<<pc;
        pc = pc + 4; 

    }
    else if(bcode.substr(25,7) == "0010111"){
        string imm_val = bcode.substr(0,20) + "000000000000";
        cout<<imm_val<<endl;
        int imm = BinaryToInteger(imm_val,32);
        

        string reg = '0' + bcode.substr(20,5);
        int reg_add = BinaryToInteger(reg, 6);
        //cout<<pc<<endl;

        int reg_fin_val = (pc) +imm;
        cout<<pc<<endl;
        register_map[register_add_map[reg_add]] = reg_fin_val;
        pc = pc + 4;

    }

    return;
}

void i_simu(string &bcode , map<string,long> &register_map, map<int, string> &register_add_map, int &pc,map<string, int> &program_mem ){
    // Given an I type string, this executes it, i.e. does the needed changes in PC and File registers.

    string opcode = bcode.substr(25, 7) ;
    string func3 = bcode.substr(17, 3) ;
    string imm_binary = bcode.substr(0, 12) ; // bits of the immediate
    //cout<<imm_binary<<endl;
    int imm_val = BinaryToInteger(imm_binary, 12) ; // integer value of immediate, assuming 2's complement
    //cout<<imm_val;

    string rs_bin = bcode.substr(12, 5) ; // binary representation of rs1
    string rs_fin = '0' + rs_bin;
    int rs = BinaryToInteger(rs_fin, 6) ; /* this is the corresponding number of the 

    register in the file registers. */
    string rs_abi = register_add_map[rs] ; // abi of rs

    string rd_bin = bcode.substr(20, 5) ; // same story now for rd
    string rd_fin = '0' + rd_bin;
    int rd = BinaryToInteger(rd_fin, 6) ; 
    string rd_abi = register_add_map[rd] ; // abi of rd

    
    if(opcode == "0000011" && func3 == "010"){
        // lw
        string imm_val = bcode.substr(0,12);
        int imm = BinaryToInteger(imm_val,12);

        string reg1 = '0' + bcode.substr(12,5);
        int reg_no1 = BinaryToInteger(reg1, 6);

        string reg2 = '0' + bcode.substr(20,5);
        int reg_no2 = BinaryToInteger(reg2, 6);
        //cout<<imm;

        long reg1_val = register_map[register_add_map[reg_no1]];
        long hex_int_add = imm + reg1_val;
    

        string hex_add = "0x" + IntToHex32BitString(hex_int_add);
    

        register_map[register_add_map[reg_no2]] = program_mem[hex_add];
        pc= pc + 4;
    }
    else if(opcode == "0010011" && func3 == "000"){
        // addi
        int ans ;
        ans = register_map[rs_abi] + imm_val ;
        
        long long max = pow(2, 31) ; // max value of 32 bits with 2's complement 
        if(ans >= max){
            // overflow has occurred
            ans = ans - max ;
        }
        register_map[rd_abi] = ans ;
        //cout<<"here";
        
        pc=pc+4 ;
    }
    else if(opcode == "0010011" && func3 == "011"){
        // sltiu
        string binary_rs = decimalToBinary32(register_map[rs_abi]) ; // bin. repres. of the value of rs
        string fin_bin_rs= '0'+binary_rs;
        int unsigned_value_of_rs = BinaryToInteger(fin_bin_rs, 33) ; // unsigned value of binary_rs
        string imm_bin_fin = '0' + imm_binary;
        int unsigned_value_of_imm = BinaryToInteger(imm_bin_fin, 13) ; // unsigned value of imm

        if(unsigned_value_of_rs < unsigned_value_of_imm){
            register_map[rd_abi] = 1 ;
        }
        pc=pc+4 ;
    }
    else if(opcode == "1100111" && func3 == "000"){
        // jalr
        int return_addres = pc + 4 ; // the just next instruction
        int address_to_go = register_map[rs_abi] + imm_val ;

        register_map[rd_abi] = return_addres ; // save return address here.
        pc = address_to_go ; // now go here
    }
}


void bonus_simu(string &bcode , map<string,long> &register_map, map<int, string> &register_add_map, int &pc){
    string opcode = bcode.substr(25, 7) ;
    string func3 = bcode.substr(17, 3) ;

    string rs_bin = bcode.substr(12, 5) ; // binary representation of rs1
    string rs_fin = '0' + rs_bin;
    int rs = BinaryToInteger(rs_fin, 6) ; /* this is the corresponding number of the 
    register in the file registers. */
    string rs_abi = register_add_map[rs] ; // abi of rs

    string rd_bin = bcode.substr(20, 5) ; // same story now for rd
    string rd_fin = '0' + rd_bin;
    int rd = BinaryToInteger(rd_fin, 6) ; 
    string rd_abi = register_add_map[rd] ; // abi of rd

    if(opcode == "1111110" && func3 == "000"){
        // rst
        for(int i = 1; i < 32; i++){ // Don't reset x0, it's always 0.
            string reg_abi = register_add_map[i] ;
            register_map[reg_abi] = 0 ;
        }
        pc = pc + 4 ;
    }
    else if(opcode == "1111110" && func3 == "001"){
        // rvrs
        string bin_of_rs = decimalToBinary32(register_map[rs_abi]) ;
        string rev_ans = "" ;
        for(int i = 31; i >= 0; i--){
            rev_ans = rev_ans + bin_of_rs[i] ;
        }
        long long_int_of_rev_ans = BinaryToInteger(rev_ans, 32) ;
        register_map[rd_abi] = long_int_of_rev_ans ;
        pc = pc + 4 ;
    }
    else if(opcode == "1111110" && func3 == "010"){
        // mul
        string rs2_bin = bcode.substr(12, 5) ;
        string rs2_fin = '0' + rs2_bin ;
        int rs2 = BinaryToInteger(rs2_fin, 6) ; // corresp. no of rs2 in file registers
        string rs2_abi = register_add_map[rs2] ;

        long num1 = register_map[rs2_abi] ; 
        long num2 = register_map[rs_abi] ;
        // we'll add num1 , to itself, num2 times
        bool negative = false ;
        if( num1 < 0 && num2 >= 0){
            negative = true ;
        }
        else if( num1 >= 0 && num2 < 0){
            negative = true ;
        }
        else if(num1 == 0 || num2 == 0){
            // rd =  0
        }
        long ans = 0 ;
        if(negative == false){
            // mul two +ve numbers only
            num1 = abs(num1) ; num2 = abs(num2) ; 
            for(long i = 0; i < num2; i++){
                ans = ans + num1 ;
                long long max = pow(2, 31) ; // max value of 32 bits with 2's complement 
                if(ans >= max){
                    // overflow has occurred
                    ans = ans - max ;
                }
            }
        }
        else if(negative == true){
            // negative nos.
            long iterator ;
            long num_to_sub ;
            if(num1 < 0){
                iterator =  num1 ;
                num_to_sub = num2 ;
            }
            else{
                iterator = num2 ;
                num_to_sub = num1 ;
            }
            // repeated sub
            num_to_sub = abs(num_to_sub) ;
            for(long i = 0; i > iterator; i--){
                ans = ans - num_to_sub ;
                long max_neg = - pow(2, 31) ; // most -ve value we can accomodate in 32 bits
                if(ans < max_neg){
                    // overflow
                    ans = ans - (2 * max_neg) ;  
                }
            }
        }
        register_map[rd_abi] = ans ;
        pc = pc + 4 ;
    }
}



void classifier(string & bcode,map<string, long> &register_map, map<int, string> &register_add_map, int & pc,map<string, int> &program_mem){
    string opcode;
    opcode = bcode.substr(25,7);
    if(opcode == "0110011"){
        r_simu(bcode,register_map,register_add_map,pc );
    }
    else if(opcode == "0000011" || opcode == "0010011" || opcode == "1100111"){
        i_simu(bcode,register_map,register_add_map,pc,program_mem );

    }
    else if(opcode == "0100011"){
        s_simu(bcode, register_map, register_add_map,pc , program_mem);
        

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
    else if(opcode == "1111110"){
        bonus_simu(bcode, register_map, register_add_map, pc );
    }
    else{
        pc=pc+4;
    }

}

int main(){

    map<string, long> register_map;

    register_map["zero"] = 0;
    register_map["ra"] = 0;
    register_map["sp"] = 256;
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
    register_map["s10"] = 0;
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
    register_add_map[11] = "a1";
    register_add_map[12] = "a2";
    register_add_map[13] = "a3";
    register_add_map[10] = "a0";
    register_add_map[14] = "a4";
    register_add_map[15] = "a5";
    register_add_map[16] = "a6";
    register_add_map[17] = "a7";
    register_add_map[18] = "s2";
    register_add_map[19] = "s3";
    register_add_map[20] = "s4";
    register_add_map[21] = "s5";
    register_add_map[22] = "s6";
    register_add_map[23] = "s7";
    register_add_map[24] = "s8";
    register_add_map[25] = "s9";
    register_add_map[26] = "s10";
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


    

int pc = 0;
map <int, string> instruction;
int total = -1;


ifstream input_file("inp.txt");
ofstream output_file("out.txt");

string line;

while(getline(input_file, line)){
    if(!line.empty() ){
        total++;
        instruction[total] = line;
    }
}

while(pc/4 <= total){

    if(instruction[(pc/4)] == "00000000000000000000000001100011"){

        if(output_file.is_open()){
            string pc_bin = "0b" + decimalToBinary32(pc);
            output_file << pc_bin << " ";
        
            for(int i = 0; i< 32;i++){
                string reg_bin = "0b" + decimalToBinary32(register_map[register_add_map[i]]);
                output_file<< reg_bin <<" ";   
            }
            output_file<<endl;
        }
        break;
    }
    else{
        classifier(instruction[pc/4],register_map,register_add_map,pc,program_mem);
        
        if(output_file.is_open()){
        string pc_bin = "0b" + decimalToBinary32(pc);
        output_file << pc_bin << " ";
        string reg_bin;
        int b;
    
        for(int i = 0; i< 32;i++){
            b = register_map[register_add_map[i]];
            reg_bin = "0b" + decimalToBinary32(b);
            output_file<< reg_bin <<" ";   
        }
        output_file<<endl;
    }
        
    }
    
}
cout<<program_mem["0x00010000"]<<endl;
output_file <<"0x00010000:" << "0b" << decimalToBinary32(program_mem["0x00010000"]) << endl;
output_file <<"0x00010004:" << "0b" << decimalToBinary32(program_mem["0x00010004"]) << endl;
output_file <<"0x00010008:" << "0b" << decimalToBinary32(program_mem["0x00010008"]) << endl;
output_file <<"0x0001000c:" << "0b" << decimalToBinary32(program_mem["0x0001000c"]) << endl;
output_file <<"0x00010010:" << "0b" << decimalToBinary32(program_mem["0x00010010"]) << endl;
output_file <<"0x00010014:" << "0b" << decimalToBinary32(program_mem["0x00010014"]) << endl;
output_file <<"0x00010018:" << "0b" << decimalToBinary32(program_mem["0x00010018"]) << endl;
output_file <<"0x0001001c:" << "0b" << decimalToBinary32(program_mem["0x0001001c"]) << endl;
output_file <<"0x00010020:" << "0b" << decimalToBinary32(program_mem["0x00010020"]) << endl;
output_file <<"0x00010024:" << "0b" << decimalToBinary32(program_mem["0x00010024"]) << endl;
output_file <<"0x00010028:" << "0b" << decimalToBinary32(program_mem["0x00010028"]) << endl;
output_file <<"0x0001002c:" << "0b" << decimalToBinary32(program_mem["0x0001002c"]) << endl;
output_file <<"0x00010030:" << "0b" << decimalToBinary32(program_mem["0x00010030"]) << endl;
output_file <<"0x00010034:" << "0b" << decimalToBinary32(program_mem["0x00010034"]) << endl;
output_file <<"0x00010038:" << "0b" << decimalToBinary32(program_mem["0x00010038"]) << endl;
output_file <<"0x0001003c:" << "0b" << decimalToBinary32(program_mem["0x0001003c"]) << endl;
output_file <<"0x00010040:" << "0b" << decimalToBinary32(program_mem["0x00010040"]) << endl;
output_file <<"0x00010044:" << "0b" << decimalToBinary32(program_mem["0x00010044"]) << endl;
output_file <<"0x00010048:" << "0b" << decimalToBinary32(program_mem["0x00010048"]) << endl;
output_file <<"0x0001004c:" << "0b" << decimalToBinary32(program_mem["0x0001004c"]) << endl;
output_file <<"0x00010050:" << "0b" << decimalToBinary32(program_mem["0x00010050"]) << endl;
output_file <<"0x00010054:" << "0b" << decimalToBinary32(program_mem["0x00010054"]) << endl;
output_file <<"0x00010058:" << "0b" << decimalToBinary32(program_mem["0x00010058"]) << endl;
output_file <<"0x0001005c:" << "0b" << decimalToBinary32(program_mem["0x0001005c"]) << endl;
output_file <<"0x00010060:" << "0b" << decimalToBinary32(program_mem["0x00010060"]) << endl;
output_file <<"0x00010064:" << "0b" << decimalToBinary32(program_mem["0x00010064"]) << endl;
output_file <<"0x00010068:" << "0b" << decimalToBinary32(program_mem["0x00010068"]) << endl;
output_file <<"0x0001006c:" << "0b" << decimalToBinary32(program_mem["0x0001006c"]) << endl;
output_file <<"0x00010070:" << "0b" << decimalToBinary32(program_mem["0x00010070"]) << endl;
output_file <<"0x00010074:" << "0b" << decimalToBinary32(program_mem["0x00010074"]) << endl;
output_file <<"0x00010078:" << "0b" << decimalToBinary32(program_mem["0x00010078"]) << endl;
output_file <<"0x0001007c:" << "0b" << decimalToBinary32(program_mem["0x0001007c"]) << endl;
output_file.close();
cout<<pc;

    return 0;
}

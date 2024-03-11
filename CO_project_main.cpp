//If you're unsure about whether your code is correct or not, first add it in here as a comment and then later after discussion you can omit any changes you want
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <bitset>

using namespace std;

char flip(char c) {return (c == '0')? '1': '0';} 
int twoscomplement(string bin){ // shouldn't the return type of this instruction be string ?
    int n=bin.length(); 
    int i; 
    string ones, twos; 
    ones=twos=""; 
    //  for ones complement flip every bit 
    for(i=0;i<n;i++){ 
        ones+=flip(bin[i]); 
    //  for two's complement go from right to left in 
    //  ones complement and if we get 1 make, we make 
    //  them 0 and keep going left when we get first 
    //  0, make that 1 and go out of loop 
    twos=ones; 
    }

    for (i=n-1;i>=0;i--){ 
        if (ones[i]=='1'){ 
            twos[i]='0'; 
        }
        else{ 
            twos[i]='1'; 
            break; 
        } 
    } 
    // If No break : all are 1  as in 111  or  11111; 
    // in such case, add extra 1 at beginning 
    if (i==-1) 
        twos='1'+twos; 
    //cout << "1's complement: " << ones << endl; 
    //cout << "2's complement: " << twos << endl;
    return twos;
} 
int bin(string& imm){
    int n=stoi(imm);
    int binaryNum[32]; 
    int i=0;
    int m;
    if(n>0){
        m=n;
    } 
    else{
        m=-n;
    }
    while (m>0) { 
        binaryNum[i]=m%2; 
        m=m/2; 
        i++; 
    } 
    string c="";
    for (int j=i-1;j>=0;j--){
        c+=to_string(binaryNum[j]);
    } 
    int d=c.length();
    string v="0";
    for(int p=1;p<(20-d);p++){
        v+="0";
    }
    c=v+c;
    cout<<c<<endl;
    if(n>=0){
        return c;
    }
    else{
        return twoscomplement(c);
    }
    
}
bool checkLastLine(string& filename, regex& regexPattern){
    ifstream inputFile(filename);
    
    string line;
    string lastNonEmptyLine;

    while(getline(inputFile, line)){
        if(!line.empty()){
            lastNonEmptyLine=line;
        }
    }

    return regex_match(lastNonEmptyLine, regexPattern);
}


bool containsVirHalt(string& filename,regex& regexPattern) {
    // Input file stream for reading from the input file
    ifstream inputFile(filename);

    // Check if the input file is open
    if(!inputFile.is_open()){
        cout<< "Error opening input file.\n";
        return false;
    }

    string line;
    bool patternFound = false;

    while(getline(inputFile,line)){
        // Check if the line matches the specified regex pattern
        if(regex_search(line, regexPattern)){
            patternFound = true;
            break;  // Stop searching once the pattern is found
        }
    }

    // Close the input file
    inputFile.close();

    return patternFound;
}


string r_assembler(string & line, unordered_map<string, string> & reg_map){
    unordered_map <string, string> opcode_bi_rep;

    opcode_bi_rep["add"] = "0110011";
    opcode_bi_rep["sub"] = "0110011";
    opcode_bi_rep["sll"] = "0110011";
    opcode_bi_rep["slt"] = "0110011";
    opcode_bi_rep["sltu"] = "0110011";
    opcode_bi_rep["xor"] = "0110011";
    opcode_bi_rep["srl"] = "0110011";
    opcode_bi_rep["or"] = "0110011";
    opcode_bi_rep["and"] = "0110011";

    unordered_map <string, string> funct3_bi_rep;
    funct3_bi_rep["add"] = "000";
    funct3_bi_rep["sub"] = "000";
    funct3_bi_rep["sll"] = "001";
    funct3_bi_rep["slt"] = "010";
    funct3_bi_rep["sltu"] = "011";
    funct3_bi_rep["xor"] = "100";
    funct3_bi_rep["srl"] = "101";
    funct3_bi_rep["or"] = "110";
    funct3_bi_rep["and"] = "111";


    regex instruc("\\s*(add|sub|slt|sltu|xor|sll|srl|or|and)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*");

    smatch match;
    bool temp = regex_match(line, match, instruc);

    string regd = reg_map[match[2]];
    string regs1 = reg_map[match[4]];
    string regs2 = reg_map[match[6]];
    string fun3_bicode = funct3_bi_rep[match[1]] ; 
    string opcode_bi = opcode_bi_rep[match[1]] ;
    string output_bi_code;
    if(match[1] != "sub"){
        string output_bi_code = "0000000" + regs2 + regs1 + fun3_bicode + regd + opcode_bi;
    }
    else{
        string output_bi_code = "0100000" + regs2 + regs1 + fun3_bicode + regd + opcode_bi;
    }
    
    return output_bi_code;
}

string u_assembler(string& line,unordered_map<string,string>& reg_map){
    unordered_map<string,string> opcode_bi_rep;
    opcode_bi_rep["lui"]="0110111";
    opcode_bi_rep["auipc"]="0010111";
    regex instructions("\\s*(auipc|lui)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s+([(-1048576)-1048575])");

    smatch match;
    bool temp=regex_match(line,match,instructions);
    string opcode_bi=opcode_bi_rep[match[1]];
    string regd=reg_map[match[2]];
    string imm=immme_map[match[3]];
    string output_bicode=opcode_bi+regd+bin(imm);

    return output_bicode;
}

string s_assembler(string& line, unordered_map<string,string> reg_map){

    regex instRegex("\\s*sw\\s+(ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*\\d+\\(\\s*(ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*\\)\\s*");
    smatch match;
    bool tmp = regex_match(line, match,instRegex);
    string op_code = "0100011";
    string imm_string = match[4] ;
    int imm = stoi(imm_string);
    string imm_bin_code;
    if(imm >= 0 && imm<= 2047){
        imm_bin_code = bitset<12>(imm).to_string();
    }
    else if(imm < 0 && imm>= -2048){
    
        int positiveEquivalent = 4096 + imm; // 2^12
        imm_bin_code = bitset<12>(positiveEquivalent).to_string();
    }
    else{
        cerr<<"Immediate value out of bound";
        exit(1);
    }
    string fun3 = "010";
    string regs2 = match[2];
    string regs1 = match[6];
    string imm04 = imm_bin_code.substr(7,5);
    string imm511 = imm_bin_code.substr(0,7);

    string out = imm511 + regs2 + regs2 + fun3 + imm04 + op_code;
    return out;

}

string j_assembler(string &line, unordered_map<string,string> &reg_map, int program_counter,unordered_map <string , int> label_map){
    regex instruc("\\s*jal\\s+(ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(.*)\\s*$");
    smatch match;
    bool x = regex_match(line, match, instruc);

    int lab = label_map[match[4]];
    int pc = program_counter;
    
    int imm = 4*(pc - lab) ;
    string imm_bin_code;
    if(imm >= 0 && imm<= 524287){
        imm_bin_code = bitset<20>(imm).to_string();
    }
    else if(imm < 0 && imm>= -524288){
    
        int positiveEquivalent = 1048576 + imm; // 2^12
        imm_bin_code = bitset<20>(positiveEquivalent).to_string();
    }
    else{
        cerr<<"Immediate value out of bound";
        exit(1);
    }

    string regd = reg_map[match[2]];
    
    string opcod = "1101111";
    //string fu3 = funct3[match[1]];

    char temp11 = imm_bin_code[9];
    string imm11(1,temp11);
    string imm1912 = imm_bin_code.substr(1,8);
    char temp20 = imm_bin_code[0];
    string imm20(1,temp20);
    string imm101 = imm_bin_code.substr(10,10);

    string out = imm20 + imm101 + imm11  + imm1912 + regd + opcod;
    return out;
}


string binary_converter_for_int(string value){
    // takes a value b/w -2048 and +2047 and gives the binary representation , in 12 bits
    string ans ;
    int num = abs(stoi(value)) ;
    for(int j = 0; j < 12; j++){
        int rem = num % 2 ;
        string s_rem = to_string(rem) ;
        num = num / 2 ;
        ans = s_rem + ans ;
    }
    if (value[0] == '-'){
        ans = twoscomplement(ans) ;
    }
    return ans ;
}

int idx_of_first_non_s_char(string s){
    // returns the index of the first non-space char in string, and -1 if no such char is there
    int l = s.length() ;
    for(int i = 0; i < l; i++){
        if (s[i] != ' '){
            return i ;
        }
    }
    return -1 ;
}

string i_assembler(string instruction, unordered_map<string, string> &register_map){
    int l = instruction.length() ;

    for(int i = 0; i < l; i++){
        if (instruction[i] == '(' or instruction[i] == ')' or instruction[i] == ','){
            //cout << i << " " ;
            instruction[i] = ' ' ; // remove any parentheses or comma occuring, the instruction is now only space separated.
        } 
    }
    
    int idx_o_f_n_s_c_in_instruction = idx_of_first_non_s_char(instruction) ;
    string cpy_ins = instruction.substr(idx_o_f_n_s_c_in_instruction, l - idx_o_f_n_s_c_in_instruction) ;

    string assmbly[4] ;
    int counter = 0 ;

    while (counter < 4){
        int cpy_l = cpy_ins.length() ;
        string token ;
        int i = 0 ;
        while (cpy_ins[i] != ' ' and i < cpy_l){
            token = token + cpy_ins[i] ; 
            i++ ;
        }
        assmbly[counter] = token ;
        if (counter != 3){ // don't do any operation on the last token.
        cpy_ins = cpy_ins.substr(i, cpy_l - i) ; // now, length of cpy_ins is cpy_l - i

        int idx_o_f_n_s_c_in_cpy_ins = idx_of_first_non_s_char(cpy_ins) ;

        cpy_ins = cpy_ins.substr(idx_o_f_n_s_c_in_cpy_ins, (cpy_l - i) - idx_o_f_n_s_c_in_cpy_ins) ;
        }
        counter++ ;

    }

    // Now, we'll generate the output in binary. It's form: imm(31:20), rs1(19:15), funct3(14:12), rd(11:7), opcode(6:0)

    string op_code = assmbly[0] ;
    string rd = assmbly[1] ;
    string rs = assmbly[2] ;
    string imm = assmbly[3] ;

    if (op_code == "lw"){
        rd = assmbly[1] ; 
        imm = assmbly[2] ; 
        rs = assmbly[3] ; 
    }

    unordered_map<string, string> opcode_binary ;
    opcode_binary["lw"] = "0000011" ; opcode_binary["addi"] = "0010011" ; opcode_binary["sltiu"] = "0010011" ; 
    opcode_binary["jalr"] = "1100111" ;  

    unordered_map<string, string> opcode_func3 ;
    opcode_func3["lw"] = "010" ; opcode_func3["addi"] = "000" ; opcode_func3["sltiu"] = "011" ; opcode_func3["jalr"] = "000" ; 

    string binary_representation ;
    binary_representation = binary_converter_for_int(imm) + register_map[rs] + opcode_func3[op_code] + register_map[rd] + opcode_binary[op_code] ;
    
    return binary_representation ;


}



bool is_rinstruction(string& line){
    string low_line = line;
    transform(low_line.begin(), low_line.end(), low_line.begin(), ::tolower);
    
    // R-type instruction format //
    regex instRegex("\\s*(add|sub|slt|sltu|xor|sll|srl|or|and)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*");

    smatch match;

    if(regex_match(low_line,match,instRegex)){
        return true;
    } 
    else{
        return false;
    }
}

bool is_iinstruction(string &line){
    
    regex instruc1("\\s*(lw)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d+)\\s*\\(\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*\\)\\s*");
    regex instruc2("\\s*(addi|sltiu)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d+)\\s*");
    regex instruc3("\\s*(jalr)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d+)\\s*");

    smatch match1;
    smatch match2;
    smatch match3;

    if(regex_match(line, match1, instruc1)){
        //cout<<"lw";
        return true ;
    }    
    else if(regex_match(line, match2, instruc2)){
        //cout<<"addi";
        return true;
    }
    else if(regex_match(line, match3, instruc3)){
        //cout<<"jalr";
        return true;
    }
    return false ;
}

bool is_sinstruction(string& line){
    string low_line = line;
    transform(low_line.begin(), low_line.end(), low_line.begin(), ::tolower);

    //  S-type instruction format
    regex instRegex("\\s*sw\\s+(ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*\\d+\\(\\s*(ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*\\)\\s*");

    smatch match;

    regex swRegex("\\s*sw\\s+(ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d{1,4})\\(\\s*(ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*\\)\\s*");
    

    if(regex_match(low_line, match, instRegex)){
        if(regex_match(low_line, match,swRegex )){
            return true;
        }
        else{
            cout<< "Invalid Immediate";
            exit(0);
        }
    }
    else {
        return false;
    }
}

bool is_uinstructions(string& line){
    string low_line=line;
    transform(low_line.begin(),low_line.end(),low_line.begin(),::tolower);
    
    // U-type instruction format //
    regex instructions("\\s*(auipc|lui)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s+([(-1048576)-1048575])")
    smatch match;
    
    if(regex_match(low_line,match,instructions)){
        return true;
    }
    else return false;
}

bool is_jinstruction(string &line, unordered_map<string,int> label_map){
    regex instruc("\\s*jal\\s+(ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(.*)\\s*$");
    smatch match;
    if(regex_match(line, match, instruc)){
        auto it = label_map.find(match[6]);
        if(it!=label_map.end()){
            return true;
        }
    }
    return false;

}

string classifier(string &line, unordered_map<string,string> &reg_map){

    if(is_rinstruction(line)){
        return r_assembler(line , reg_map);
    }
    if(is_sinstruction(line)){
        return s_assembler(line , reg_map);
    }
    else{
        cout<< "invalid syntax";
        return "stop";
    }

}

int main(){

    unordered_map<string, string> register_map;

    register_map["zero"] = "00000";
    register_map["ra"] = "00001";
    register_map["sp"] = "00010";
    register_map["gp"] = "00011";
    register_map["tp"] = "00100";
    register_map["t0"] = "00101";
    register_map["t1"] = "00110";
    register_map["t2"] = "00111";
    register_map["s0"] = "01000";
    register_map["fp"] = "01000";
    register_map["s1"] = "01001";
    register_map["a0"] = "01010";
    register_map["a1"] = "01011";
    register_map["a2"] = "01100";
    register_map["a3"] = "01101";
    register_map["a4"] = "01110";
    register_map["a5"] = "01111";
    register_map["a6"] = "10000";
    register_map["a7"] = "10001";
    register_map["s2"] = "10010";
    register_map["s3"] = "10011";
    register_map["s4"] = "10100";
    register_map["s5"] = "10101";
    register_map["s6"] = "10110";
    register_map["s7"] = "10111";
    register_map["s8"] = "11000";
    register_map["s9"] = "11001";
    register_map["s10"] = "11010";
    register_map["s11"] = "11011";
    register_map["t3"] = "11100";
    register_map["t4"] = "11101";
    register_map["t5"] = "11110";
    register_map["t6"] = "11111";

    regex Vir_halt("\\s*beq\\s+(zero)\\s*,\\s*(zero)\\s*,\\s*(0x00000000)\\s*");
    string Ifilename= "inp.txt"
    //a map that store line string as key and hexadecimal adress as value

    if(inputFile.is_open()){
        while(getline(inputFile, line1)){
            if(!line1.empty()){
                    if(classifier(line1, register_map) != "stop"){
                    std::stringstream ss;
                    ss << std::setfill('0') << std::setw(8) << std::hex << address;
                    lineAddressMap[line1] = ss.str();
                    address += 0x00000001;
                }
            }
        }
        inputFile.close();
    } else {
        std::cout << "Unable to open file";
    }
    
    ifstream input_file(Ifilename);
    ofstream output_file("");
    string line;

    if(!containsVirHalt(Ifilename, Vir_halt)){
        cout << "missing halt instruction in input file"; 
    }
    else{
        if(!input_file.is_open()){
            if(checkLastLine(Ifilename, Vir_halt)){
                while(getline(input_file, line)){
                    if(!line.empty()){
                        if(classifier(line, register_map) != "stop"){

                        }
                        else{
                            exit(0);
                        }
                    }
                }
            }
            else{
                cout<< "No Virtual Halt at end of input instructions";
            }
        }
        else{
            cout<< "error in opening file";
        }
    }
    

    return 0;
}







 

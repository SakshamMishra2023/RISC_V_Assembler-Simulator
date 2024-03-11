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

bool keyExists(const std::unordered_map<string, int>& map, const string& key) {
    return map.find(key) != map.end();
}
bool is_number(string& s){
     return !s.empty() && std::find_if(s.begin() + (s[0] == '-' ? 1 : 0), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();;
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
string decimalToBinary16(int decimal) {
    std::bitset<16> binaryRepresentation;

    if (decimal < 0) {
        // Calculate the two's complement
        int positiveEquivalent = 65536 + decimal; // 2^12
        binaryRepresentation = bitset<16>(decimal);
    } else {
        // Positive number: directly convert to binary
        binaryRepresentation = std::bitset<16>(decimal);
    }

    return binaryRepresentation.to_string();
}


string decimalToBinary12(int decimal){
    bitset<12> binaryRepresentation;
    if(decimal <0){
        // Calculate the two's complement
        int positiveEquivalent = 4096 + decimal; // 2^12
        binaryRepresentation = bitset<12>(decimal);
    } 
    else{
        // Positive number: directly convert to binary
        binaryRepresentation = bitset<12>(decimal);
    }
    return binaryRepresentation.to_string();
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
            inputFile.close();
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

string j_assembler(string &line, unordered_map<string,string> &reg_map, int &program_counter,unordered_map <string , int> &label_map){
    regex instruc1("\\s*(jal)\\s+(zero|gp|ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\b\\w+\\b)\\s*");
    regex instruc2("\\s*(jal)\\s+(zero|gp|ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\b\\w+\\b)\\s*\\(\\s*(zero|gp|ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*\\)\\s*");
    smatch match1;
    smatch match2;
    if(regex_match(line, match1, instruc1)){
    string last_arg = match1[3];
    if(keyExists(label_map,last_arg)){
    //cout<<match[4];
    int lab = label_map[match1[3]];
    int pc = program_counter;
    //cout<<pc<<" "<<lab;
    int sub = lab - pc;
    //cout<<sub;
    int imm = 4*sub ;
    //cout<<imm<<endl;
    //out<<imm;
    string imm_bin_code;
    if(imm >= 0){
         imm_bin_code = decimalToBinary32(imm);
    }
    else{
    
        //int positiveEquivalent = 4096 + imm; // 2^12
        imm_bin_code = decimalToBinary32(imm);
    }
    //cout<< imm_bin_code<<endl;
    string regs = reg_map[match1[2]];
    string opcod = "1101111";
    //cout<<imm_bin_code;

        char temp20 = imm_bin_code[11];
        string imm20(1,temp20);
        string imm1912 = imm_bin_code.substr(12,8);
        char temp11 = imm_bin_code[20];
        string imm11(1,temp11);
        string imm110 = imm_bin_code.substr(21,10);
    
        string out = imm20 + imm110 + imm11 + imm1912 +  regs+opcod;
        return out;
    }
    else if(is_number(last_arg)){
        int imm =stoi(last_arg);
        if(imm >= -1048576 && imm<= 1048575){
        string imm_bin_code;
    
    
        //int positiveEquivalent = 4096 + imm; // 2^12
        imm_bin_code = decimalToBinary32(imm);
        //cout<<imm_bin_code<<endl;
        //cout<<imm_bin_code<<endl;
        string regs = reg_map[match1[2]];
        string opcod = "1101111";

        //cout<<imm_bin_code;
    
        char temp20 = imm_bin_code[11];
        string imm20(1,temp20);
        string imm1912 = imm_bin_code.substr(12,8);
        char temp11 = imm_bin_code[20];
        string imm11(1,temp11);
        string imm110 = imm_bin_code.substr(21,10);
    
        string out = imm20 + imm110 + imm11 + imm1912 +  regs+opcod;
        return out;
        }
        else{
            cerr<<"Immediate out of range";
            exit(1);
        }

    }
    }
    else if(regex_match(line, match2, instruc2)){
    string last_arg = match2[3];
    if(keyExists(label_map,last_arg)){
    //cout<<match[4];
    int lab = label_map[match2[3]];
    int pc = program_counter;
    //cout<<pc<<" "<<lab;
    int sub = lab - pc;
    //cout<<sub;
    int imm = 4*sub ;
    //cout<<imm<<endl;
    //out<<imm;
    string imm_bin_code;
    if(imm >= 0){
         imm_bin_code = decimalToBinary32(imm);
    }
    else{
    
        //int positiveEquivalent = 4096 + imm; // 2^12
        imm_bin_code = decimalToBinary32(imm);
    }
    //cout<< imm_bin_code<<endl;
    string regs = reg_map[match2[2]];
    string opcod = "1101111";
    //cout<<imm_bin_code;

        char temp20 = imm_bin_code[11];
        string imm20(1,temp20);
        string imm1912 = imm_bin_code.substr(12,8);
        char temp11 = imm_bin_code[20];
        string imm11(1,temp11);
        string imm110 = imm_bin_code.substr(21,10);
    
        string out = imm20 + imm110 + imm11 + imm1912 +  regs+opcod;
        return out;
    }
    else if(is_number(last_arg)){
        int imm =stoi(last_arg);
        if(imm >= 1048576 && imm <=1048575){
        string imm_bin_code;
    
    
        //int positiveEquivalent = 4096 + imm; // 2^12
        imm_bin_code = decimalToBinary32(imm);
        //cout<<imm_bin_code<<endl;
        //cout<<imm_bin_code<<endl;
        string regs = reg_map[match2[2]];
        string opcod = "1101111";

        //cout<<imm_bin_code;
    
        char temp20 = imm_bin_code[11];
        string imm20(1,temp20);
        string imm1912 = imm_bin_code.substr(12,8);
        char temp11 = imm_bin_code[20];
        string imm11(1,temp11);
        string imm110 = imm_bin_code.substr(21,10);
    
        string out = imm20 + imm110 + imm11 + imm1912 +  regs+opcod;
        return out;
        }
        else{
            cerr<<"Immediate out of range";
            exit(1);
        }


    }
    }
}

string i_assembler(std::string& line, std::unordered_map<std::string, std::string>& reg_map) {
    std::unordered_map<std::string, std::string> opcode_bi_rep;
    opcode_bi_rep["lw"] = "0000011";
    opcode_bi_rep["addi"] = "0010011";
    opcode_bi_rep["sltiu"] = "0010011";
    opcode_bi_rep["jalr"] = "1100111";

    unordered_map <string, string> funct3_bi_rep;
    funct3_bi_rep["lw"] = "010";
    funct3_bi_rep["addi"] = "000";
    funct3_bi_rep["sltiu"] = "011";
    funct3_bi_rep["jalr"] = "000";
    
    

    regex instruc1("\\s*(lw)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d+)\\s*\\(\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*\\)\\s*");
    regex instruc2("\\s*(addi|sltiu)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d+)\\s*");
    regex instruc3("\\s*(jalr)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d+)\\s*");

    smatch match1;
    smatch match2;
    smatch match3;
    if(regex_match(line, match1, instruc2)){

        int imm = stoi(match1[4]);
        //cout<<imm<<endl;
        
            string opcode_bi = opcode_bi_rep[match1[1]];
            string regd = reg_map[match1[2]];
            string brac_reg = reg_map[match1[3]];
            string fun3 = funct3_bi_rep[match1[1]];
            
            // Convert immediate value to 32-bit 2's complement
            string immBinary = decimalToBinary12(imm);

            // Construct the output binary code
            string output_bicode = immBinary + brac_reg+ fun3 + regd + opcode_bi;

            return output_bicode;        
    }
    else if(regex_match(line, match2, instruc1)){
        //int imm = stoi(match2[4]);
        string imm_str = match2[3];
        imm_str.erase(std::remove(imm_str.begin(), imm_str.end(), '('), imm_str.end());
        imm_str.erase(std::remove(imm_str.begin(), imm_str.end(), ')'), imm_str.end());

        int imm = stoi(imm_str);
        //cout<<imm<<endl;
        
            string opcode_bi = opcode_bi_rep[match2[1]];
            string regd = reg_map[match2[2]];
            string regs = reg_map[match2[4]];
            string fun3= funct3_bi_rep[match2[1]];
            
            // Convert immediate value to 32-bit 2's complement
            string immBinary = decimalToBinary12(imm);
            // Construct the output binary code
            string output_bicode = immBinary + regs + fun3 + regd + opcode_bi;

            return output_bicode;
    }
    else if(regex_match(line, match3, instruc3)){
        int imm = stoi(match3[4]);
        //cout<<imm<<endl;
        
            string opcode_bi = opcode_bi_rep[match3[1]];
            string regd = reg_map[match3[2]];
            string regs = reg_map[match3[3]];
            string fun3= funct3_bi_rep[match3[1]];
            
            // Convert immediate value to 32-bit 2's complement
            string immBinary = decimalToBinary12(imm);
    
            // Construct the output binary code
            string output_bicode = immBinary + regs + fun3 + regd + opcode_bi;

            return output_bicode;
    }
}

string b_assembler(string &line, unordered_map<string,string> &reg_map, int program_counter,unordered_map <string , int> label_map ){
    unordered_map <string, string> opcode_bi_rep;

    opcode_bi_rep["beq"] = "1100011";
    opcode_bi_rep["bne"] = "1100011";
    opcode_bi_rep["blt"] = "1100011";
    opcode_bi_rep["bge"] = "1100011";
    opcode_bi_rep["bltu"] = "1100011";

    unordered_map <string, string> funct3;

    funct3["beq"] = "000";
    funct3["bne"] = "001";
    funct3["blt"] = "100";
    funct3["bge"] = "101";
    funct3["bltu"] = "110";
    funct3["bgeu"] = "111";

    regex instruc("\\s*(beq|bne|blt|bge|bltu)\\s+(zero|gp|ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|gp|ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\b\\w+\\b)\\s*");

    smatch match;
    regex_match(line, match, instruc);
    string last_arg = match[4];
    if(keyExists(label_map,last_arg)){
    //cout<<match[4];
    int lab = label_map[match[4]];
    int pc = program_counter;
    //cout<<pc<<" "<<lab;
    int sub = lab -pc;
    //cout<<sub;
    int imm = 4*sub ;
    //out<<imm;
    string imm_bin_code;
    if(imm >= 0){
         imm_bin_code = decimalToBinary16(imm);
    }
    else{
    
        //int positiveEquivalent = 4096 + imm; // 2^12
        imm_bin_code = decimalToBinary16(imm);
    }
   
    string regs1 = reg_map[match[2]];
    string regs2 = reg_map[match[3]];
    string opcod = opcode_bi_rep[match[1]];
    string fu3 = funct3[match[1]];
    //cout<<imm_bin_code;

    char temp11 = imm_bin_code[4];
    string imm11(1,temp11);
    string imm41 = imm_bin_code.substr(11,4);
    char temp12 = imm_bin_code[3];
    string imm12(1,temp12);
    string imm510 = imm_bin_code.substr(5,6);

    string out = imm12 + imm510 + regs2 + regs1 + fu3 + imm41 + imm11 + opcod;
    return out;
    }
    else if(is_number(last_arg)){
        int imm =stoi(last_arg);
        string imm_bin_code;
        cout<<imm;
    
    
        //int positiveEquivalent = 4096 + imm; // 2^12
        imm_bin_code = decimalToBinary16(imm);
        //cout<<imm_bin_code<<endl;
        string regs1 = reg_map[match[2]];
        string regs2 = reg_map[match[3]];
        string opcod = opcode_bi_rep[match[1]];
        string fu3 = funct3[match[1]];
        cout<<imm_bin_code;
    
        char temp11 = imm_bin_code[4];
        string imm11(1,temp11);
        string imm41 = imm_bin_code.substr(11,4);
        char temp12 = imm_bin_code[3];
        string imm12(1,temp12);
        string imm510 = imm_bin_code.substr(5,6);
    
        string out = imm12 + imm510 + regs2 + regs1 + fu3 + imm41 + imm11 + opcod;
        return out;

    }
    //return out;
}

string u_assembler(std::string& line, std::unordered_map<std::string, std::string>& reg_map) {
    std::unordered_map<std::string, std::string> opcode_bi_rep;
    opcode_bi_rep["lui"] = "0110111";
    opcode_bi_rep["auipc"] = "0010111";

    regex instructions("\\s*(auipc|lui)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d+)\\s*");

    smatch match;
    if(regex_match(line, match, instructions)){
        int imm = stoi(match[3]);
        //cout<<imm<<endl;
        
            string opcode_bi = opcode_bi_rep[match[1]];
            string regd = reg_map[match[2]];
            
            // Convert immediate value to 32-bit 2's complement
            string immBinary = decimalToBinary32(imm);
            //cout<< immBinary<<endl;;
            string final_im = immBinary.substr(0,20);
            //cout<<final_im;

            // Construct the output binary code
            string output_bicode = final_im + regd + opcode_bi;

            return output_bicode;        
    }
    
}

string s_assembler(string& line, unordered_map<string,string> &reg_map){

    regex instRegex("\\s*(sw)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\b\\w+\\b)\\s*\\(\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*\\)\\s*");

    smatch match;
    if(regex_match(line, match,instRegex)){
    string op_code = "0100011";
    string imm_string = match[3] ;
    int imm = stoi(imm_string);
    //cout<<imm;

    string imm_bin_code;
    imm_bin_code = decimalToBinary12(imm);
    //cout<< imm_bin_code<<endl;
    string fun3 = "010";
    string regs2 = reg_map[match[2]];
    string regs1 = reg_map[match[4]];
    string imm04 = imm_bin_code.substr(7,5);
    string imm511 = imm_bin_code.substr(0,7);

    string out = imm511 + regs2 + regs2 + fun3 + imm04 + op_code;
    return out;
    }

}


bool is_binstruction(string& line, unordered_map<string,int> label_map){

    regex instruc("\\s*(beq|bne|blt|bge|bltu)\\s+(zero|gp|ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|gp|ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\b\\w+\\b)\\s*");
    smatch match;
    if(regex_match(line,match,instruc)){
        //if(keyExists(label_map,match[4] )){
        //    return true;
        //}
        string last_arg = match[4];
        if(keyExists(label_map,last_arg) || is_number(last_arg)){
            return true;
        }
    
    }
    return false;
   
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
        int first1 = stoi(match1[3]);
        //cout<<"lw";
        if(first1 <= 2047 && first1 >= -2048){
            return true;
        }
        else{
            cout<<"Immediate out of range";
            exit(1);
        }
    }    
    else if(regex_match(line, match2, instruc2)){
        int first2 = stoi(match2[4]);
        //cout<<"addi";
        if(first2 <= 2047 && first2 >= -2048){
            return true;
        }
        else{
            cout<<"Immediate out of range";
            exit(1);
        }
    }
    else if(regex_match(line, match3, instruc3)){
        int first3 = stoi(match3[4]);
        //cout<<"jalr";
        if(first3 <= 2047 && first3 >= -2048){
            return true;
        }
        else{
            cout<<"Immediate out of range";
            exit(1);
        }
    }

    cout<<"here";
    return false;
}
bool is_sinstruction(string& line){
    string low_line = line;
    //transform(low_line.begin(), low_line.end(), low_line.begin(), ::tolower);

    //  S-type instruction format
    regex instRegex("\\s*(sw)\\s+(zero|ra|sp|gp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\b\\w+\\b)\\s*\\(\\s*(zero|ra|sp|gp|t[0-6]|s[0-9]|a[0-7])\\s*\\)\\s*");
    smatch match;
    
    if(regex_match(low_line, match,instRegex)){
        int first1 = stoi(match[3]);
        if(first1>=-2048 && first1  <= 2047){
            return true;
        }
        cout<<"Immediate out of range";
        exit(1);

    }
    else{
        return false;
    }

}
bool is_uinstruction(string &line){
    string low_line = line;
    regex instructions("\\s*(auipc|lui)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d+)\\s*");
    smatch match;
    // /bool a = regex_search(low_line, instructions);
    // /regex_match(low_line,match,instructions);
    //cout<<match[3];
    // /cout<<a;

    if(regex_match(low_line,match,instructions)){     
        int imm = stoi(match[3]);
          // Convert matched string to integer
        if(imm >= -2147483648 && imm <= 2147483647){
            return true;
        } 
        else{
            cout<<"Immediate out of range";
        exit(1);
        }
    }
    return false;
}

bool is_jinstruction(string &line, unordered_map<string,int> label_map){
    regex instruc2("\\s*(jal)\\s+(zero|gp|ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\b\\w+\\b)\\s*\\(\\s*(zero|gp|ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*\\)\\s*");
    smatch match2;
    regex instruc1("\\s*(jal)\\s+(zero|gp|ra|sp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\b\\w+\\b)\\s*");
    smatch match1;
    cout<<"atl";
    if(regex_match(line,match1,instruc1)){
        //if(keyExists(label_map,match[4] )){
        //    return true;
        //}
        cout<<"here two";
        string last_arg = match1[3];
        if(keyExists(label_map,last_arg) || is_number(last_arg)){
            return true;
        }
        cout<<"ff:";
        return false;
    
    }
    else if(regex_match(line,match2,instruc2)){
        //if(keyExists(label_map,match[4] )){
        //    return true;
        //}
        cout<<"here two";
        string last_arg = match2[3];
        if(keyExists(label_map,last_arg) || is_number(last_arg)){
            return true;
        }
        cout<<"ff:";
        return false;

    }
    cout<<"what";
    return false;

}

bool is_label(string& line){
    regex pattern("\\s*(\\S+):\\s*(.*?)\\s*$");

    if(regex_search(line, pattern)){

        return true;
    }
    return false;
}
////change
string label_instruc_extrac(string &line , unordered_map <string , int> &label_map){
    regex pattern("\\s*(\\S+):\\s*(.*?)\\s*$");
    smatch match;
    string out;
    
    regex_match(line, match, pattern);

    
        out = match[2];
        //cout<<out;
        
        return out;
}
////change
string label_name_extrac(string &line , unordered_map <string , int> &label_map){
    regex pattern("\\s*(\\S+):\\s*(.*?)\\s*$");
    smatch match;
    string out;
    
    if( regex_match(line, match ,pattern)){
        out = match[1];
        //cout<<out;

        return out;
    }

}

string halt_assembler(string& line ){
    string bin_code = "00000000000000000000000001100011";
    return bin_code;

}
bool is_haltinstruc(string&line, unordered_map<string,string> &reg_map){
    regex hal("\\s*beq\\s+zero\\s*,\\s*zero\\s*,\\s*0\\s*");
    smatch match;
    if(regex_match(line,match, hal)){
        return true;
    }
}

/* Below, in comments, is the function to check if it's an i instruction, along with range checking. */

// bool is_iinstruction(string &line){
//     bool ans = false ;
//     regex instruc1("\\s*(lw)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d+)\\s*\\(\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*\\)\\s*");
//     regex instruc2("\\s*(addi|sltiu)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d+)\\s*");
//     regex instruc3("\\s*(jalr)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(-?\\d+)\\s*");

//     smatch match1;
//     smatch match2;
//     smatch match3;

//     if(regex_match(line, match1, instruc1)){
//         //cout<<"lw";
//         ans = true ;
//     }    
//     else if(regex_match(line, match2, instruc2)){
//         //cout<<"addi";
//         ans = true;
//     }
//     else if(regex_match(line, match3, instruc3)){
//         //cout<<"jalr";
//         ans = true;
//     }

//     int line_length = line.length() ;
//     int line_end_idx = line_length - 1 ;
//     int digit_start_idx ; // includes the '-' sign for negative nos.
//     int digit_end_idx ;
//     int i = line_end_idx ;
//     while (true) {
//         char line_char = line[i] ;
//         if ( isdigit(line_char) ){ // The first digit (starting from the end of the assembly instruction) is the last digit of the integer.  
//             digit_end_idx = i ;
//             break;
//         } 
//         i-- ;
//     }
//     while (true) {
//         char line_char = line[i] ;
//         if ( line_char == ',' ){
//             digit_start_idx = i + 1 ; // The digits (including '-' sign, if there) occurs after the last comma. Spaces between the number are dealt by the stoi function.
//             break;
//         } 
//         i-- ;
//     }
//     // Now, the string between digit_start_idx and digit_end_idx , both inclusive, represents an integer, with sign if it's -ve
//     int digit_len = digit_start_idx - digit_end_idx + 1 ;
//     string str_digit = line.substr(digit_start_idx, digit_len) ;
//     int int_digit = stoi(str_digit) ;
//     cout << int_digit << " " ;
//     if (int_digit > 2047 or int_digit < -2048){
//         ans = false ;
//     }
//     else{
//         ans = true ;
//     }

//     return ans ;
// }


string classifier(string &line, unordered_map<string,string> &reg_map,int &program_counter,unordered_map<string,int> &label_map){
    //cout<<is_label(line);
    if(is_label(line)){
        string extra = label_instruc_extrac(line, label_map);
        cout<<"here"<<endl;
        program_counter+=1;
        //cout<<"classifier label";
        cout<<extra;
        return classifier(extra, reg_map, program_counter, label_map);
        
    }
    else{

        if(is_rinstruction(line)){
            program_counter+=1;
            //cout<<"is r type instruc \n";
            string fina=r_assembler(line, reg_map);
            //cout<<fina<<endl;
            return fina;
        }
        else if(is_haltinstruc(line,reg_map)){
            program_counter+=1;
            return halt_assembler(line);
        }
    
        else if(is_binstruction(line, label_map)){
            program_counter+=1;
            return b_assembler(line,reg_map,program_counter,label_map);
        }
        else if(is_uinstruction(line)){
            program_counter+=1;
            return u_assembler(line,reg_map);
        }
        else if(is_iinstruction(line)){
            program_counter+=1;
            return i_assembler(line,reg_map);
        }
        else if(is_sinstruction(line)){
            program_counter+=1;
            return s_assembler(line,reg_map);
        }
        else if(is_jinstruction(line,label_map)){
            program_counter+=1;
            return j_assembler(line,reg_map,program_counter,label_map);
        }
     
        else{
        //cout<< "invalid syntax";
        //cout << "is not r type instruc \n";
        //program_counter+=1;
        cerr<<"Invalid Syntax at line "<<program_counter;
        exit(1);
        return "Invalid Syntax";
    }
    }

}
bool adress_label(string &line, unordered_map<string,string> &reg_map,int &program_counter1,  unordered_map <string , int> &label_map){
    if(is_label(line)){
        //cout<<"is label"<<endl;
        string extrac = label_instruc_extrac(line, label_map);
        string label_name = label_name_extrac(line, label_map);
        cout<<label_name<<endl;
        cout<<extrac;
        program_counter1+=1;
        label_map[label_name] = program_counter1;

            //cout<<"adrees label";
            //cout<<"Invalid Syntax at line "<<program_counter1 + 1;
            //exit(1);
        return true;
            
    }
    else{
        
        //else{
        //    program_counter1+=1;
        //    cerr<<"Invalid Syntax at line "<<program_counter1+1;
        //    exit(1);
        //}
        program_counter1+=1;
        return false;

    }
}


int main(){

    int program_counter1 = -1;
    int program_counter = -1;

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
    string line1;
    ifstream input_file("inp.txt");
    //a map that store line string as key and hexadecimal adress as value

    unordered_map<string,int> label_map;
    while(getline(input_file, line1)){
        if(!line1.empty()){
            if(adress_label(line1, register_map, program_counter1, label_map)){
                int a = 2;
            }
            else{
                continue;
            }
        }

        
    }
    //label_map["start"] = 2;
    input_file.close();
    for (const auto& pair : label_map) {
        cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
    
   
    ifstream inputfile("inp.txt");
    ofstream output_file("out.txt");


    string line;

    if(!containsVirHalt(Ifilename, Vir_halt)){
        cerr << "Missing halt instruction in input file"; 
        exit(1);
    }
    else{
        
        if(inputfile.is_open()){
            if(checkLastLine(Ifilename, Vir_halt)){
                while(getline(inputfile, line)){
                    if(!line.empty()){
                        string result = classifier(line, register_map, program_counter,label_map);
                        if(result != "Invalid Syntax"){
                           output_file<<result<<endl;; 
                        }
                        else{
                            program_counter++;
                            cerr<< result<<" at line "<<program_counter;
                            exit(1);

                        }
    
                    }
                }
                output_file.close();
            }
            else{
                cerr<< "No Virtual Halt at end of input instructions";
                exit(1);
            }
    
        }
        else{
            cerr<< "error in opening file";
            exit(1);
        }
    }
    inputfile.close();

    return 0;
}







 

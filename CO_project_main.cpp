//If you're unsure about whether your code is correct or not, first add it in here as a comment and then later after discussion you can omit any changes you want
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <unordered_map>
#include <bitset>
using namespace std;


bool checkLastLine(const string& filename, const regex& regexPattern){
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


bool is_rinstruction(const string& line){
    string low_line = line;
    transform(low_line.begin(), low_line.end(), low_line.begin(), ::tolower);
   
    regex instRegex("\\s*(add|sub|slt|sltu|xor|sll|srl|or|and)\\s+(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*,\\s*(zero|ra|sp|gp|tp|t[0-6]|s[0-9]|a[0-7])\\s*");

    smatch match;

    
    if(regex_match(low_line,match,instRegex)){
        
        return true;
    } 
    else{
        cout<< "Invalid instruction syntax.";
        return false;
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

    //string line ="   slt  x2,     x3,        x1";
    string Ifilename= "";
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



checking the base condition for s-type instruction ;
/*
bool is_sinstruction(const string& line) {
    string low_line = line;
    transform(low_line.begin(), low_line.end(), low_line.begin(), ::tolower);

    //  S-type instruction format
    regex instRegex("\\s*(sw)\\s+(x[0-9]{1,2})\\s*,\\s*(imm[0-9]{1,2})\\(\\s*(x[0-9]{1,2})\\s*\\)\\s*");

    smatch match;

    if (regex_match(low_line, match, instRegex)) {
        
        if (!is_validRegister(match[2]) || !is_validImmediate(match[3]) || !is_validRegister(match[4])) {
            cout << "Invalid register address or immediate value in the instruction.";
            return false;
        }

        return true;
    }
    else {
        cout << "Invalid instruction syntax.";
        return false;
    }
}

int main() {
    // Testing the is_sinstruction function //
    string instruction;
    cout << "Enter the S-type instruction: ";
    getline(cin, instruction);

    if (is_sinstruction(instruction)) {
        cout << "Valid S-type instruction." <<;
    }
    else {
        cout << "Invalid S-type instruction." <<;
    }

    return 0;
}
*/


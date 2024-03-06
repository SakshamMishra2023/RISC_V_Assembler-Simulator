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


bool is_validRegister(const string& registerName){
    
    regex reg("(x[0-9]|x[1-2][0-9]|x31|x30)");
    return regex_match(registerName, reg);
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


    regex instruc("\\s*(add|sub|slt|sltu|xor|sll|srl|or|and)\\s+(x[0-9]|x[1-2][0-9]|x31|x30)\\s*,\\s*(x[0-9]|x[1-2][0-9]|x31|x30)\\s*,\\s*(x[0-9]|x[1-2][0-9]|x31|x30)\\s*");

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
   
    regex instRegex("\\s*(add|sub|slt|sltu|xor|sll|srl|or|and)\\s+(x[0-9]{1,2})\\s*,\\s*(x[0-9]{1,2})\\s*,\\s*(x[0-9]{1,2})\\s*");

    smatch match;

    
    if(regex_match(low_line,match,instRegex)){
        
        if(!is_validRegister(match[2])||!is_validRegister(match[3])||!is_validRegister(match[4])){
            cout<<"Invalid register address in the instruction.";
            return false;
        }

        return true;
    } 
    else{
        cout<< "Invalid instruction syntax.";
        return false;
    }
}


int main(){

    unordered_map<string, string> register_map;

    for (int i = 0; i <= 31; ++i) {
        std::string register_name = "x" + to_string(i);
        std::string binary = std::bitset<7>(i).to_string();
        register_map[register_name] = binary;
    }




    regex Vir_halt("\\s*beq\\s+(zero)\\s*,\\s*(zero)\\s*,\\s*(0x00000000)\\s*");

    //string line ="   slt  x2,     x3,        x1";
    string Ifilename= "";
    ifstream input_file(Ifilename);
    ofstream output_file("");
    string line;

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
    

    return 0;
}



checking base condition for s-type instruction 
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

*/

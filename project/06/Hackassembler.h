#include <fstream>
#include <string>
#include <map>
using namespace std;
class Hackassembler {
public:
    Hackassembler(string infile, string outfile); //接收「輸入檔名」和「輸出檔名」
    ~Hackassembler(); //解構子
    void assemble(); 
private:
    enum CommandType {EMPTY,A_INSTRUCTION,C_INSTRUCTION,L_INSTRUCTION}; //定義一個列舉型態，用來標示目前指令的種類：A 指令、C 指令、L（標籤）指令、或空行。
    map<string, string> compTable = {
        {"0", "101010"}, {"1", "111111"}, {"-1", "111010"},
        {"D", "001100"}, {"A", "110000"}, {"!D", "001101"},
        {"!A", "110001"}, {"-D", "001111"}, {"-A", "110011"},
        {"D+1", "011111"}, {"A+1", "110111"}, {"D-1", "001110"},
        {"A-1", "110010"}, {"D+A", "000010"}, {"D-A", "010011"},
        {"A-D", "000111"}, {"D&A", "000000"}, {"D|A", "010101"},
        {"M", "110000"}, {"!M", "110001"}, {"-M", "110011"},
        {"M+1", "110111"}, {"M-1", "110010"}, {"D+M", "000010"},
        {"D-M", "010011"}, {"M-D", "000111"}, {"D&M", "000000"},
        {"D|M", "010101"}
    };

    map<string, string> destTable = {
        {"", "000"}, {"M", "001"}, {"D", "010"},
        {"MD", "011"}, {"A", "100"}, {"AM", "101"},
        {"AD", "110"}, {"AMD", "111"}
    };

    map<string, string> jumpTable = {
        {"", "000"}, {"JGT", "001"}, {"JEQ", "010"},
        {"JGE", "011"}, {"JLT", "100"}, {"JNE", "101"},
        {"JLE", "110"}, {"JMP", "111"}
    };

    map<string, int> symbolTable={
        {"SP",0}, {"LCL",1}, {"ARG",2}, {"THIS",3}, {"THAT",4},
        {"R0",0}, {"R1",1}, {"R2",2}, {"R3",3}, {"R4",4},
        {"R5",5}, {"R6",6}, {"R7",7}, {"R8",8}, {"R9",9},
        {"R10",10}, {"R11",11}, {"R12",12}, {"R13",13},
        {"R14",14}, {"R15",15}, {"SCREEN",16384}, {"KBD",24576}
    };
    ifstream infile ; //輸入檔案串流
    ofstream outfile ; //輸出檔案串流
    int nextVariableAddress = 16; //下一個可用的變數記憶體位址，初始值為 16
    CommandType instructionType(string); //判斷指令類型
    string removeWhitespaceAndComments(string); //移除空白和註解
    };
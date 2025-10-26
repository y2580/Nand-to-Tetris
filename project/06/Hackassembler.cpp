#include <iostream>
#include <bitset>
#include "Hackassembler.h"
using namespace std;
//g++ -Wall -O2 -std=gnu++17 Hackassembler.cpp -o Hackassembler -g


Hackassembler::Hackassembler(string asmfile, string hackfile){
    infile.open(asmfile); 
    outfile.open(hackfile); 
}

Hackassembler::~Hackassembler(){
    infile.close(); 
    outfile.close(); 
}

Hackassembler::CommandType Hackassembler::instructionType(string line){
    if(line.empty()){
        return EMPTY;
    }
    else if(line.size()>=2&&line[0]=='/'&&line[1]=='/'){
        return EMPTY;
    }
    else if(line[0]=='@'){
        return A_INSTRUCTION;
    }
    else if(line[0]=='(' && line.back()==')'){
        return L_INSTRUCTION;
    }
    else{
        return C_INSTRUCTION;
    }
}

string Hackassembler::removeWhitespaceAndComments(string line0){
    string result;
    for(int i=0;i<line0.size();i++){ //刪除所有註解和空白
        if(line0[i]=='/' && i+1<line0.size() && line0[i+1]=='/'){  // \n 和 \r 是「換行控制符號」，在用 getline() 讀檔時會自動被移除，所以你不需要再手動刪它們。
            break;
        }
        else if(line0[i]==' '){ 
            continue;
        }
        result+=line0[i];
    }
    return result;
}

void Hackassembler::assemble(){
    string line;
    string line0;
    int lineNumber=0; //用來記錄目前的指令行數（不包含標籤和空行）
    while (getline(infile,line0)) //讀取每一行
    {
        line = removeWhitespaceAndComments(line0);
        Hackassembler::CommandType type = instructionType(line); 
        if(type==EMPTY) continue; 
        else if(type==L_INSTRUCTION){   //第一趟只處理Label
            string symbol = line.substr(1,line.size()-2);
            if(symbolTable.find(symbol)==symbolTable.end()){
                symbolTable[symbol]=lineNumber; //將Label加入符號表，對應的值是目前的指令行數
            }
        }
        else{
            lineNumber++; 
        }
    }

    infile.clear(); //清除 EOF 標誌，以便能夠重新讀取檔案
    infile.seekg(0, ios::beg); //將檔案指標移回檔案開頭

    while (getline(infile,line0)){ //第二趟處理 A 指令和 C 指令
        line = removeWhitespaceAndComments(line0);
        Hackassembler::CommandType type = instructionType(line); 
        if(type==EMPTY || type==L_INSTRUCTION) continue; 
        else if(type==A_INSTRUCTION){ //A Instruction: @value or @symbol
            string address = line.substr(1);
            int addressValue;
            if(address[0]>='0' && address[0]<='9'){ 
            addressValue = stoi(address); 
            } 
            else if(symbolTable.find(address)!=symbolTable.end()){  //符號已存在於符號表中
            addressValue = symbolTable[address]; //取得對應的記憶體位址
            }
            else{ //符號不存在於符號表中，分配新的記憶體位址
                symbolTable[address] = nextVariableAddress;
                addressValue = nextVariableAddress;
                nextVariableAddress++;
            }        
            string binaryCode = "0" + bitset<15>(addressValue).to_string(); //將整數轉換為 15 位元的二進位字串，並在前面加上 '0'
            outfile << binaryCode << endl;  //寫入輸出檔案
        }
        else if(type==C_INSTRUCTION){  //C Instruction: dest=comp;jump
            string dest, comp, jump;
            size_t eqPos = line.find('='); 
            size_t scPos = line.find(';');
            if(eqPos!=string::npos){ 
                dest = line.substr(0,eqPos); 
                if(scPos!=string::npos){ 
                    comp = line.substr(eqPos+1,scPos-eqPos-1);
                    jump = line.substr(scPos+1);
                }
                else{ 
                    comp = line.substr(eqPos+1);
                    jump = "";
                }
            }
            else{ //沒有等號
                dest = "";
                if(scPos!=string::npos){ 
                    comp = line.substr(0,scPos);
                    jump = line.substr(scPos+1);
                }
                else{ 
                    comp = line;
                    jump = "";
                }
            }
            char abit = (comp.find('M') != string::npos) ? '1' : '0'; // 判斷是否使用 M 作為運算元
            string compBits = compTable[comp];
            string destBits = destTable[dest];
            string jumpBits = jumpTable[jump];
            outfile << "111" << abit << compBits << destBits << jumpBits << endl ; 
        }
    }
}

int main(int argc, char* argv[]){  //接收使用者在命令列輸入的「附加文字」，argc參數個數、argv參數值陣列 
    if(argc!=2){ //檢查參數數量是否正確
        cout << "Usage: Hackassembler <inputfile>.asm" << endl;
        return 1; //非正常結束程式
    }
    string inputFile = argv[1]; 
    if(inputFile.size()<4 || inputFile.substr(inputFile.size()-4)!=".asm"){ //檢查副檔名是否為 .asm
        cout << "Input file must have .asm extension." << endl;
        return 1; 
    }
    string outputFile = inputFile.substr(0,inputFile.size()-4)+".hack"; //產生輸出檔名
    Hackassembler assembler(inputFile, outputFile); //建立 Hackassembler 物件
    assembler.assemble(); //執行組譯
    return 0; //~Assembler() 會自動被呼叫，關閉檔案
}


/*
$ ./Hackassembler Rect.asm

↓
main()
 ├─ 檢查參數數量 (argc==2?)
 ├─ 確認副檔名是 .asm
 ├─ 產生 Rect.hack
 ├─ 建立 HackAssembler(Rect.asm, Rect.hack)
 └─ 呼叫 convert()
       ├─ 第一趟：建立符號表
       ├─ 第二趟：輸出二進位機器碼
       └─ 寫入 Rect.hack
*/



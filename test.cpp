#include<iostream>
#include<cmath>
#include<string>
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<fstream>
#include<set>
#include<sstream>
#include "test.hh"

using namespace std;

int d=0;
unordered_map<string,string> alias = {
    {"zero", "x0"}, {"ra", "x1"}, {"sp", "x2"}, {"gp", "x3"}, {"tp", "x4"},{"t0", "x5"}, {"t1", "x6"}, {"t2", "x7"}, {"s0", "x8"}, {"fp", "x8"},{"s1", "x9"}, {"a0", "x10"}, {"a1", "x11"}, {"a2", "x12"}, {"a3", "x13"},{"a4", "x14"}, {"a5", "x15"}, {"a6", "x16"}, {"a7", "x17"}, {"s2", "x18"},{"s3", "x19"}, {"s4", "x20"}, {"s5", "x21"}, {"s6", "x22"}, {"s7", "x23"},{"s8", "x24"}, {"s9", "x25"}, {"s10", "x26"}, {"s11", "x27"}, {"t3", "x28"},{"t4", "x29"}, {"t5", "x30"}, {"t6", "x31"}
};
vector<string> validRegisters = {
    "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9","x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19","x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29","x30", "x31"
};

vector<pair<string,string>> memory;
vector<pair<string,int>> labels;
int counter=0;

string removeLeadingZeros1(const string &str) {
    size_t nonZeroPos = str.find_first_not_of('0');
    
    // If all characters are '0' or string is empty, return a single '0'
    if (nonZeroPos==string::npos) {
        return "0";
    }

    // Return the substring starting from the first non-zero character
    return str.substr(nonZeroPos);
}

string labeloutput(int n){
    string k="";
    for(size_t i=0;i<labels.size();i++){
        if(n==labels[i].second){
            k=labels[i].first;
            break;
        }
    }
    return k;
}

long long int hexToDecimal(const string& hex) {
    long long int decimalValue = 0;
    int base = 1; // 16^0

    // Start from the last character of the hexadecimal string
    for (int i = hex.length() - 1; i >= 0; --i) {
        char hexDigit = hex[i];

        // Convert hexadecimal character to integer
        if (hexDigit >= '0' && hexDigit <= '9') {
            decimalValue += (hexDigit - '0') * base;
        } else if (hexDigit >= 'A' && hexDigit <= 'F') {
            decimalValue += (hexDigit - 'A' + 10) * base;
        } else if (hexDigit >= 'a' && hexDigit <= 'f') {
            decimalValue += (hexDigit - 'a' + 10) * base;
        }

        // Update the base (multiply by 16)
        base *= 16;
    }

    return decimalValue;
}

// Function to convert decimal (integer) to hex string
string decimalToHex(long long int decimalValue) {
    stringstream ss;
    ss <<hex<< decimalValue;

    string hexStr = ss.str();

    // Ensure the result has at least `minWidth` characters (pad with zeros if necessary)
    if (hexStr.length() < 16) {
        hexStr.insert(hexStr.begin(), 16 - hexStr.length(), '0');
    }
    else if(hexStr.length() > 16){
        hexStr.substr(hexStr.size()-16);
    }

    return hexStr;
}

// Function to add two hex numbers
string addHex(const string& hex1, const string& hex2) {
    // Convert hex strings to decimal
    long long int val1 = hexToDecimal(hex1);
    long long int val2 = hexToDecimal(hex2);

    // Add the two decimal values
    long long int sum = val1 + val2;

    // Convert the sum back to a hex string
    return decimalToHex(sum);
}

long long int subtractHex(const string& hex1, const string& hex2) {
    // Convert hex strings to decimal
    long long int val1 = hexToDecimal(hex1);
    long long int val2 = hexToDecimal(hex2);

    // Subtract the two decimal values
    long long int sum1 = val1 - val2;

    // Convert the sum back to a hex string
    return sum1;
}

//checks for the tokens if they registers or not in R format.
string checkR(vector<string> tokens,int z){
    if(tokens.size()!=4) {cout<<"Enter complete command. Error at line "<<z<<endl;return "no";}
    int r=0;
    if (alias.find(tokens[1]) != alias.end()) {
        tokens[1] = alias[tokens[1]];
    }
    if (alias.find(tokens[2]) != alias.end()) {
        tokens[2] = alias[tokens[2]];
    }
    if (alias.find(tokens[3]) != alias.end()) {
        tokens[3] = alias[tokens[3]];
    }
    for(size_t i=0;i<validRegisters.size();i++){
        if(tokens[1]==validRegisters[i]) r++;
        if(tokens[2]==validRegisters[i]) r++;
        if(tokens[3]==validRegisters[i]) r++;
    }
    if(r!=3) {
        cout<<"Error in line "<< z<<" :Invalid register"<<endl;
        return "no";
    }
        
    if (tokens.size() != 4) {
        return "no"; 
    }
    return "yes";
}

//I format Function to check the token 1 and 2 to be valid registers and token 3 to be the valid immediate having all the digits or hex number.
string checkI(vector<string> tokens,int z){
    if(tokens.size()!=4) {cout<<"Enter complete command. Error at line "<<z<<endl;return "no";}
    int r=0;
    if (alias.find(tokens[1]) != alias.end()) {
        tokens[1] = alias[tokens[1]];
    }
    if (alias.find(tokens[2]) != alias.end()) {
        tokens[2] = alias[tokens[2]];
    }
    for(size_t i=0;i<validRegisters.size();i++){
        if(tokens[1]==validRegisters[i]) r++;
        if(tokens[2]==validRegisters[i]) r++;
    }
    int j=all_of(tokens[3].begin(),tokens[3].end() ,::isdigit);
    if(tokens[3][0]=='-') j=all_of(tokens[3].begin()+1,tokens[3].end() ,::isdigit);
    if(tokens[3][0]=='0' && tokens[3][1]=='x'&& all_of(tokens[3].begin()+2,tokens[3].end() ,::isxdigit)) j=1;
    if(j==1) r++;
    if(r!=3) {
        if(j!=1) cout<<"Error in line "<< z<<" :Invalid immediate value"<<endl;
        else cout<<"Error in line "<< z<<" :Invalid register"<<endl;
        return "no";
    }
    return "yes";
}

//S format Function to check the token 1 to be valid registers and token 2 to be the valid immediate having all the digits or hex number along woth the register.
string checkS(vector<string> tokens,int z){
    int r=0;string imm="";
    size_t i=0;
    if(tokens.size()!=3) {cout<<"Enter complete command. Error at line "<<z<<endl;return "no";}
    while(tokens[2][i]!='(' && i!=tokens[2].size()){
        imm=imm+tokens[2][i];
        i++;
        if(tokens[2][i]=='('){
            i+=1;
            break;
        }
    }
    string rs1="";
    while(tokens[2][i]!=')' && i!=tokens[2].size()){
        if(tokens[2][i]==')'){
            break;
        }
        rs1=rs1+tokens[2][i];
        i++;
    }
    if (alias.find(tokens[1]) != alias.end()) {
        tokens[1] = alias[tokens[1]];
    }
    if (alias.find(rs1) != alias.end()) {
        rs1 = alias[rs1];
    }
    for(size_t i=0;i<validRegisters.size();i++){
        if(tokens[1]==validRegisters[i]) r++;
        if(rs1==validRegisters[i]) r++;
    }
    int j=0;
    if(imm[0]=='-'){
        j=all_of(imm.begin()+1,imm.end() ,::isdigit);
    }
    else{
        j=all_of(imm.begin()+1,imm.end() ,::isdigit);
    }
    if(imm[0]=='0' && imm[1]=='x') j=all_of(imm.begin()+2,imm.end() ,::isxdigit);
    if(j==1) r++;
    if(r!=3) {
        if(j!=1) cout<<"Error in line "<< z<<" :Invalid immediate value"<<endl;
        else cout<<"Error in line "<< z<<" :Invalid register"<<endl;
        return "no";
    }
    return "yes";
}

//B format Function to check the token 1 and 2 to be valid registers
string checkB(vector<string> tokens,int z){
    if(tokens.size()!=4) {cout<<"Enter complete command. Error at line "<<z<<endl;return "no";}
    int r=0;
    if (alias.find(tokens[1]) != alias.end()) {
        tokens[1] = alias[tokens[1]];
    }
    if (alias.find(tokens[2]) != alias.end()) {
        tokens[2] = alias[tokens[2]];
    }
    for(size_t i=0;i<validRegisters.size();i++){
        if(tokens[1]==validRegisters[i]) r++;
        if(tokens[2]==validRegisters[i]) r++;
    }
    if(r!=2) {
        cout<<"Error in line "<< z<<" :Invalid register"<<endl;
        return "no";
    }
    return "yes";
} 

//Function to split the instructions on basis of space and comma
vector<string> split(const string& str, char delim1, char delim2, char delim3) {
    vector<string> tokens;
    string token;
    
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == delim1 || str[i] == delim2 || str[i] == delim3) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += str[i];
        }
    }
    
    if (!token.empty()) {
        tokens.push_back(token);
    }
    
    return tokens;
}

//function to convert decimal to binary number
string decimal_binary(int a){
    string z;
    while(a!=0){
        z = to_string(a%2) + z;
        a=a/2;
    }
    while (z.size() < 5) {
        z = "0" + z;
    }
    return z;
}

//Function to convert binary to hex number
string binary_hex(string a){
    string z="";
    int l=0;
    for(size_t i=0;i<a.size();i++){
        if(i%4==0){
            l=0;
            int k=a[i]-'0';
            k=k*8;
            l+=k;
        }
        if(i%4==1){
            int k=a[i]-'0';
            k=k*4;
            l+=k;
        }
        if(i%4==2){
            int k=a[i]-'0';
            k=k*2;
            l+=k;
        }
        if(i%4==3){
            int k=a[i]-'0';
            k=k*1;
            l+=k;
            if(l==10) z+='a';
            else if(l==11) z+='b';
            else if(l==12) z+='c';
            else if(l==13) z+='d';
            else if(l==14) z+='e';
            else if(l==15) z+='f';
            else z+=to_string(l);
            l=0;
        }
    }
    return z;
}

//Function to convert hex to binary
string hex_binary(const string& hex) {
    unordered_map<char,string> hex_map{
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"}, {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
        {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"}, {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"},
        {'a', "1010"}, {'b', "1011"}, {'c', "1100"}, {'d', "1101"}, {'e', "1110"}, {'f', "1111"}
    };

    string binary = "";

    for (char c : hex) {
        binary += hex_map[c];
    }

    return binary;
}

//Function to convert hex to decimal number
long long int hex_dec(const string& hexStr) {
    long long int decimalValue = 0;
    long long int base = 1; 

    for (int i = hexStr.size() - 1; i >= 0; --i) {
        char c = hexStr[i];
        int hexDigit;
        if (isdigit(c)) {
            hexDigit = c - '0';
        } else if (isalpha(c)) {
            c = toupper(c); 
            hexDigit = c - 'A' + 10;
        } else {
            cerr << "Invalid character in hexadecimal string." << endl;
            return -1; 
        }
        decimalValue += hexDigit * base;
        base *= 16;
    }
    return decimalValue;
}

//Function to take 1s complement
string binary_add_one(string binary) {
    int carry = 1;  // Start with 1 to add to the binary number
    string result = binary;

    for (int i = binary.size() - 1; i >= 0; --i) {
        if (binary[i] == '1' && carry == 1) {
            result[i] = '0';
        } else if (binary[i] == '0' && carry == 1) {
            result[i] = '1';
            carry = 0;  // No more carry needed
        }
    }

    return result;
}

//Function to find tw0's complement
string find_twos_complement(string binary) {
    // Compute the 1's complement
    string ones_complement;
    for (char bit : binary) {
        ones_complement += (bit == '1') ? '0' : '1';
    }

    // Compute the 2's complement by adding 1 to the 1's complement
    return binary_add_one(ones_complement);
}

//function ot check the negative or positive number and convert to binary 
string negorpos(vector<string> tokens,string imm){
    string c;
    if(imm[0]=='-'){
        tokens[3]=tokens[3].substr(1);
        imm=tokens[3];
        long long int imm_1 = stoll(imm);
        string a=decimal_binary(imm_1);
        while (a.size() < 12) {
            a = "0" + a;
        }
        c=find_twos_complement(a);
    }
    else{
        long long int imm_1 = stoll(imm);
        c=decimal_binary(imm_1);
        while (c.size() < 12) {
            c = "0" + c;
        }
    }
    return c;
}

//function ot check the negative or positive number and convert to binary 
string negorpos_imm(string imm){
    if(imm[0]=='-'){
        imm=imm.substr(1);
        int imm_1 = stoi(imm);
        string c=decimal_binary(imm_1);
        while (c.size() < 12) {
            c = "0" + c;
        }
        c=find_twos_complement(c);
        return c;
    }
    else{
        int imm_1 = stoi(imm);
        string c=decimal_binary(imm_1);
        while (c.size() < 12) {
            c = "0" + c;
        }
        return c;
    }
    
}

//Function for the R format instructions
string R(vector<string> tokens,int z){
    if(tokens[0]=="add"){
        if(checkR(tokens,z)=="no") return "no";
        string opcode = "0110011" ;
        string funct3= "000";
        string funct7 = "0000000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string rs2 = tokens[3];
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string num_rs2 = rs2.substr(1);
        int rs_2 = stoi(num_rs2);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(rs_2);
        string k=funct7+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="sub"){
        if(checkR(tokens,z)=="no") return "no";
        string opcode = "0110011" ;
        string funct3= "000";
        string funct7 = "0100000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string rs2 = tokens[3];
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string num_rs2 = rs2.substr(1);
        int rs_2 = stoi(num_rs2);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(rs_2);
        string k=funct7+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="xor"){
        if(checkR(tokens,z)=="no") return "no";
        string opcode = "0110011" ;
        string funct3= "100";
        string funct7 = "0000000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string rs2 = tokens[3];
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string num_rs2 = rs2.substr(1);
        int rs_2 = stoi(num_rs2);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(rs_2);
        string k=funct7+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="or"){
        if(checkR(tokens,z)=="no") return "no";
        string opcode = "0110011" ;
        string funct3= "110";
        string funct7 = "0000000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string rs2 = tokens[3];
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string num_rs2 = rs2.substr(1);
        int rs_2 = stoi(num_rs2);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(rs_2);
        string k=funct7+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="and"){
        if(checkR(tokens,z)=="no") return "no";
        string opcode = "0110011" ;
        string funct3= "111";
        string funct7 = "0000000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string rs2 = tokens[3];
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string num_rs2 = rs2.substr(1);
        int rs_2 = stoi(num_rs2);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(rs_2);
        string k=funct7+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="sll"){
        if(checkR(tokens,z)=="no") return "no";
        string opcode = "0110011" ;
        string funct3= "001";
        string funct7 = "0000000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string rs2 = tokens[3];
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string num_rs2 = rs2.substr(1);
        int rs_2 = stoi(num_rs2);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(rs_2);
        string k=funct7+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="srl"){
        if(checkR(tokens,z)=="no") return "no";
        string opcode = "0110011" ;
        string funct3= "101";
        string funct7 = "0000000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string rs2 = tokens[3];
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string num_rs2 = rs2.substr(1);
        int rs_2 = stoi(num_rs2);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(rs_2);
        string k=funct7+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="sra"){
        if(checkR(tokens,z)=="no") return "no";
        string opcode = "0110011" ;
        string funct3= "101";
        string funct7 = "0100000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string rs2 = tokens[3];
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string num_rs2 = rs2.substr(1);
        int rs_2 = stoi(num_rs2);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(rs_2);
        string k=funct7+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="slt"){
        if(checkR(tokens,z)=="no") return "no";
        string opcode = "0110011" ;
        string funct3= "010";
        string funct7 = "0000000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string rs2 = tokens[3];
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string num_rs2 = rs2.substr(1);
        int rs_2 = stoi(num_rs2);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(rs_2);
        string k=funct7+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="sltu"){
        if(checkR(tokens,z)=="no") return "no";
        string opcode = "0110011" ;
        string funct3= "011";
        string funct7 = "0000000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string rs2 = tokens[3];
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string num_rs2 = rs2.substr(1);
        int rs_2 = stoi(num_rs2);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(rs_2);
        string k=funct7+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    return "no";
}

//FUnction for I format instructions
string I(vector<string> tokens,int z){
    if(tokens[0]=="addi"){
        if(checkI(tokens,z)=="no") return "no";
        string opcode = "0010011" ;
        string funct3= "000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string imm=tokens[3];
        if(imm[1]=='x' && imm[0]=='0'){//hex check
            imm=imm.substr(2);
	        if(imm.size()>3) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            while(imm.size()<3) imm='0'+imm;
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=b+funct3+a+opcode;
            string z=binary_hex(k);
            return imm+z;
        }
        if(stoi(imm)>2047 || stoi(imm)<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string c=negorpos(tokens,imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="xori"){
        if(checkI(tokens,z)=="no") return "no";
        string opcode = "0010011" ;
        string funct3= "100";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string imm=tokens[3];
        if(imm[1]=='x' && imm[0]=='0'){//hex check
            imm=imm.substr(2);
	        if(imm.size()>3) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            while(imm.size()<3) imm='0'+imm;
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=b+funct3+a+opcode;
            string z=binary_hex(k);
            return imm+z;
        }
        if(stoi(imm)>2047 || stoi(imm)<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string c=negorpos(tokens,imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);   
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="ori"){
        if(checkI(tokens,z)=="no") return "no";
        string opcode = "0010011" ;
        string funct3= "110";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string imm=tokens[3];
        if(imm[1]=='x' && imm[0]=='0'){//hex check
            imm=imm.substr(2);
	        if(imm.size()>3) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            while(imm.size()<3) imm='0'+imm;
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=b+funct3+a+opcode;
            string z=binary_hex(k);
            return imm+z;
        }
        if(stoi(imm)>2047 || stoi(imm)<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string c=negorpos(tokens,imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="andi"){
        if(checkI(tokens,z)=="no") return "no";
        string opcode = "0010011" ;
        string funct3= "111";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string imm=tokens[3];
        if(imm[1]=='x' && imm[0]=='0'){//hex check
            imm=imm.substr(2);
	        if(imm.size()>3) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            while(imm.size()<3) imm='0'+imm;
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=b+funct3+a+opcode;
            string z=binary_hex(k);
            return imm+z;
        }
        if(stoi(imm)>2047 || stoi(imm)<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string c=negorpos(tokens,imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="slti"){
        if(checkI(tokens,z)=="no") return "no";
        string opcode = "0010011" ;
        string funct3= "010";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string imm=tokens[3];
        if(imm[1]=='x' && imm[0]=='0'){//hex check
            imm=imm.substr(2);
	        if(imm.size()>3) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            while(imm.size()<3) imm='0'+imm;
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=b+funct3+a+opcode;
            string z=binary_hex(k);
            return imm+z;
        }
        if(stoi(imm)>2047 || stoi(imm)<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string c=negorpos(tokens,imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="sltiu"){
        if(checkI(tokens,z)=="no") return "no";
        string opcode = "0010011" ;
        string funct3= "011";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string imm=tokens[3];
        if(imm[1]=='x' && imm[0]=='0'){//hex check
            imm=imm.substr(2);
	        if(imm.size()>3) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            while(imm.size()<3) imm='0'+imm;
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=b+funct3+a+opcode;
            string z=binary_hex(k);
            return imm+z;
        }
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=negorpos(tokens,imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        if(stoi(imm)>2047 || stoi(imm)<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        while (c.size() < 12) {
            c = "0" + c;
        }
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="srai"){
        if(checkI(tokens,z)=="no") return "no";
        string opcode = "0010011" ;
        string funct3= "101";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string imm=tokens[3];
        string funct6 = "010000";
        string num_rd = rd.substr(1);
        if(imm[1]=='x' && imm[0]=='0'){ //hex check
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>63 || k1<0){cout<<"Error at line "<<z<<" Immediate ranges from 0 to 63"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>6) {cout<<"Error at line "<<z<<" Immediate exceeding 6 bits"<<endl;return "no";}
            if(v[0]=='1' && v.size()==6) return "no";
            while (v.size() < 6) {
                v = "0" + v;
            }
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=funct6+v+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        int imm_1 = stoi(imm);
        if(imm_1>63 || imm_1<0){cout<<"Error at line "<<z<<" Immediate ranges from 0 to 63"<<endl;return "no";}
        if(imm_1<0) return "no";
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(imm_1);
	    if(c.size()>6) {cout<<"Error at line "<<z<<" Immediate exceeding 6 bits"<<endl;return "no";}
        while (c.size() < 6) {
            c = "0" + c;
        }
        string k=funct6+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="srli"){
        if(checkI(tokens,z)=="no") return "no";
        string opcode = "0010011" ;
        string funct3= "101";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string imm=tokens[3];
        string funct6 = "000000";
        string num_rd = rd.substr(1);
        if(imm[1]=='x' && imm[0]=='0'){//hex check
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>63 || k1<0){cout<<"Error at line "<<z<<" Immediate ranges from 0 to 63"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>6) {cout<<"Error at line "<<z<<" Immediate exceeding 6 bits"<<endl;return "no";}
            if(v[0]=='1' && v.size()==6) return "no";
            while (v.size() < 6) {
                v = "0" + v;
            }
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=funct6+v+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        int imm_1 = stoi(imm);
        if(imm_1>63 || imm_1<0){cout<<"Error at line "<<z<<" Immediate ranges from 0 to 63"<<endl;return "no";}
        if(imm_1<0) return "no";
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=decimal_binary(imm_1);
	    if(c.size()>6) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        while (c.size() < 6) {
            c = "0" + c;
        }
        string k=funct6+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="slli"){
        if(checkI(tokens,z)=="no") return "no";
        string c;
        string opcode = "0010011" ;
        string funct3= "001";
        string funct6 = "000000";
        string rd= tokens[1];
        string rs1 = tokens[2];
        string imm=tokens[3];   
        if(imm[1]=='x' && imm[0]=='0'){//hex check
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>63 || k1<0){cout<<"Error at line "<<z<<" Immediate ranges from 0 to 63"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>6) {cout<<"Error at line "<<z<<" Immediate exceeding 6 bits"<<endl;return "no";}
            if(v[0]=='1' && v.size()==6) return "no";
            while (v.size() < 6) {
                v = "0" + v;
            }
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=funct6+v+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        int imm_1 = stoi(imm);
        if(imm_1>63 || imm_1<0){cout<<"Error at line "<<z<<" Immediate ranges from 0 to 63"<<endl;return "no";}
        c=decimal_binary(imm_1);
	    if(c.size()>6) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        while (c.size() < 6) {
            c = "0" + c;
        }
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1);
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);       
        string k=funct6+c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="lb"){
        if(checkS(tokens,z)=="no") return "no";
        string opcode ="0000011";
        string funct3= "000";
        string imm="";
        size_t i=0;
        string rd= tokens[1];
        while(tokens[2][i]!='(' && i<tokens[2].size()){
            imm=imm+tokens[2][i];
            i++;
            if(tokens[2][i]=='('){
                i+=1;
                break;
            }
        }
        string rs1="";
        while(tokens[2][i]!=')' && i<tokens[2].size()){
            if(tokens[2][i]==')'){
                break;
            }
            rs1=rs1+tokens[2][i];
            i++;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        if(imm==""){cout<<"Error at line "<<z<<" Enter immediate "<<endl;;return "no";}
        if(imm[1]=='x' && imm[0]=='0'){
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (v.size() < 12) {
                v = "0" + v;
            }
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=v+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        int k1=stoi(imm);
        if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=negorpos_imm(imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="lh"){
        if(checkS(tokens,z)=="no") return "no";
        string opcode ="0000011";
        string funct3= "001";
        string imm="";
        size_t i=0;
        string rd= tokens[1];
        while(tokens[2][i]!='('){
            imm=imm+tokens[2][i];
            i++;
            if(tokens[2][i]=='('){
                i+=1;
                break;
            }
        }
        string rs1="";
        while(tokens[2][i]!=')'){
            if(tokens[2][i]==')'){
                break;
            }
            rs1=rs1+tokens[2][i];
            i++;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        if(imm==""){cout<<"Error at line "<<z<<" Enter immediate "<<endl;;return "no";}
        if(imm[1]=='x' && imm[0]=='0'){
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (v.size() < 12) {
                v = "0" + v;
            }
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=v+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        int k1=stoi(imm);
        if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=negorpos_imm(imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="lw"){
        if(checkS(tokens,z)=="no") return "no";
        string opcode ="0000011";
        string funct3= "010";
        string imm="";
        size_t i=0;
        string rd= tokens[1];
        while(tokens[2][i]!='('){
            imm=imm+tokens[2][i];
            i++;
            if(tokens[2][i]=='('){
                i+=1;
                break;
            }
        }
        string rs1="";
        while(tokens[2][i]!=')'){
            if(tokens[2][i]==')'){
                break;
            }
            rs1=rs1+tokens[2][i];
            i++;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        if(imm==""){cout<<"Error at line "<<z<<" Enter immediate "<<endl;;return "no";}
        if(imm[1]=='x' && imm[0]=='0'){
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (v.size() < 12) {
                v = "0" + v;
            }
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=v+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        int k1=stoi(imm);
        if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=negorpos_imm(imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="ld"){
        if(checkS(tokens,z)=="no") return "no";
        string opcode ="0000011";
        string funct3= "011";
        string imm="";
        size_t i=0;
        string rd= tokens[1];
        while(tokens[2][i]!='('){
            imm=imm+tokens[2][i];
            i++;
            if(tokens[2][i]=='('){
                i+=1;
                break;
            }
        }
        string rs1="";
        while(tokens[2][i]!=')'){
            if(tokens[2][i]==')'){
                break;
            }
            rs1=rs1+tokens[2][i];
            i++;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        if(imm==""){cout<<"Error at line "<<z<<" Enter immediate "<<endl;;return "no";}
        if(imm[1]=='x' && imm[0]=='0'){
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (v.size() < 12) {
                v = "0" + v;
            }
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=v+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        int k1=stoi(imm);
        if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=negorpos_imm(imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="lbu"){
        if(checkS(tokens,z)=="no") return "no";
        string opcode ="0000011";
        string funct3= "100";
        string imm="";
        size_t i=0;
        string rd= tokens[1];
        while(tokens[2][i]!='('){
            imm=imm+tokens[2][i];
            i++;
            if(tokens[2][i]=='('){
                i+=1;
                break;
            }
        }
        string rs1="";
        while(tokens[2][i]!=')'){
            if(tokens[2][i]==')'){
                break;
            }
            rs1=rs1+tokens[2][i];
            i++;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        if(imm==""){cout<<"Error at line "<<z<<" Enter immediate "<<endl;;return "no";}
        if(imm[1]=='x' && imm[0]=='0'){
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (v.size() < 12) {
                v = "0" + v;
            }
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=v+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        int k1=stoi(imm);
        if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=negorpos_imm(imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="lhu"){
        if(checkS(tokens,z)=="no") return "no";
        string opcode ="0000011";
        string funct3= "101";
        string imm="";
        size_t i=0;
        string rd= tokens[1];
        while(tokens[2][i]!='('){
            imm=imm+tokens[2][i];
            i++;
            if(tokens[2][i]=='('){
                i+=1;
                break;
            }
        }
        string rs1="";
        while(tokens[2][i]!=')'){
            if(tokens[2][i]==')'){
                break;
            }
            rs1=rs1+tokens[2][i];
            i++;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        if(imm==""){cout<<"Error at line "<<z<<" Enter immediate "<<endl;;return "no";}
        if(imm[1]=='x' && imm[0]=='0'){
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (v.size() < 12) {
                v = "0" + v;
            }
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=v+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        int k1=stoi(imm);
        if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=negorpos_imm(imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    else if(tokens[0]=="lwu"){
        if(checkS(tokens,z)=="no") return "no";
        string opcode ="0000011";
        string funct3= "110";
        string imm="";
        size_t i=0;
        string rd= tokens[1];
        while(tokens[2][i]!='('){
            imm=imm+tokens[2][i];
            i++;
            if(tokens[2][i]=='('){
                i+=1;
                break;
            }
        }
        string rs1="";
        while(tokens[2][i]!=')'){
            if(tokens[2][i]==')'){
                break;
            }
            rs1=rs1+tokens[2][i];
            i++;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        if(imm==""){cout<<"Error at line "<<z<<" Enter immediate "<<endl;;return "no";}
        if(imm[1]=='x' && imm[0]=='0'){
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (v.size() < 12) {
                v = "0" + v;
            }
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string k=v+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        int k1=stoi(imm);
        if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string num_rd = rd.substr(1);
        int rd_1 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rd_1);
        string b=decimal_binary(rs_1);
        string c=negorpos_imm(imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string k=c+b+funct3+a+opcode;
        string z=binary_hex(k);
        return z;
    }
    if(tokens[0]=="jalr"){
        string opcode="1100111";
        string funct3="000";
        string rd= tokens[1];
        size_t i=0;
        string imm="";
        if(tokens[2][0]=='x'){
            if(checkI(tokens,z)=="no") return "no";
            string rs1 = tokens[2];
            imm=tokens[3];
            string c="";
            if(imm[0]=='0' && imm[1]=='x') {imm=imm.substr(2); int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}if(imm.size()>3){cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";} else {while(imm.size()<3) {imm='0'+imm;}c=imm;}}
            else c=negorpos(tokens,imm);
            int k1=stoi(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
	        if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            if(c.size()==3){
                string k=b+funct3+a+opcode;
                string z=c+binary_hex(k);
                return z;
            }
            string k=c+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        else{
            while(tokens[2][i]!='(' && i!=tokens[2].size()){
                imm=imm+tokens[2][i];
                i++;
                if(tokens[2][i]=='('){
                    i+=1;
                    break;
                }
            }
            string rs1="";
            while(tokens[2][i]!=')' && i!=tokens[2].size()){
                if(tokens[2][i]==')'){
                    break;
                }
                rs1=rs1+tokens[2][i];
                i++;
            }
            if (alias.find(rs1) != alias.end()) {
                rs1 = alias[rs1];
            }
            if(imm==""){cout<<"Error at line "<<z<<" Enter immediate "<<endl;;return "no";}
            string c="";
            if(imm[0]=='0' && imm[1]=='x') {imm=imm.substr(2); int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}if(imm.size()>3){cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";} else {while(imm.size()<3) {imm='0'+imm;}c=imm;}}
            long long int k1=stoll(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            else c=negorpos(tokens,imm);
	        if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            string num_rd = rd.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            if(c.size()==3){
                string k=b+funct3+a+opcode;
                string z=c+binary_hex(k);
                return z;
            }
            string k=c+b+funct3+a+opcode;
            string z=binary_hex(k);
            return z;
        }
        // else if(tokens[2][0]=='x' && isdigit(tokens[2][1]) && ((tokens[2][2]=='(' || isdigit(tokens[2][2])) || tokens[2][3]==')')){
        //     if(checkS(tokens,z)=="no") return "no";
        //     string rs1="";
        //     while(tokens[2][i]!='(' && i!=tokens[2].size()){
        //         rs1=rs1+tokens[2][i];
        //         i++;
        //         if(tokens[2][i]=='('){
        //             i+=1;
        //             break;
        //         }
        //     }
        //     while(tokens[2][i]!=')' && i!=tokens[2].size()){
        //         if(tokens[2][i]==')'){
        //             break;
        //         }
        //         imm=imm+tokens[2][i];
        //         i++;
        //     }
        //     if (alias.find(rs1) != alias.end()) {
        //         rs1 = alias[rs1];
        //     }
        //     if(imm==""){cout<<"Error at line "<<z<<" Enter immediate "<<endl;;return "no";}
        //     string c="";
        //     if(imm[0]=='0' && imm[1]=='x') {imm=imm.substr(2); int k1=hex_dec(imm);
        //     if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}if(imm.size()>3){cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";} else {while(imm.size()<3) {imm='0'+imm;}c=imm;}}
        //     long long int k1=stoll(imm);
        //     if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        //     else c=negorpos(tokens,imm);
	    //     if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        //     string num_rd = rd.substr(1);
        //     int rd_1 = stoi(num_rd);
        //     string num_rs1 = rs1.substr(1);
        //     int rs_1 = stoi(num_rs1);
        //     string a=decimal_binary(rd_1);
        //     string b=decimal_binary(rs_1);
        //     if(c.size()==3){
        //         string k=b+funct3+a+opcode;
        //         string z=c+binary_hex(k);
        //         return z;
        //     }
        //     string k=c+b+funct3+a+opcode;
        //     string z=binary_hex(k);
        //     return z;
        // }
    }

    return "no";
}

//Function for the S format instructions
string S(vector<string> tokens,int z){
    if(tokens[0]=="sb"){
        if(checkS(tokens,z)=="no") return "no";
        string opcode="0100011";
        string funct3="000";
        string rs2=tokens[1];
        size_t i=0;
        string imm="";
        while(tokens[2][i]!='(' && i!=tokens[2].size()){
            imm=imm+tokens[2][i];
            i++;
            if(tokens[2][i]=='('){
                i+=1;
                break;
            }
        }
        string rs1="";
        while(tokens[2][i]!=')' && i!=tokens[2].size()){
            if(tokens[2][i]==')'){
                break;
            }
            rs1=rs1+tokens[2][i];
            i++;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        if(imm[1]=='x' && imm[0]=='0'){
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (v.size() < 12) {
                v = "0" + v;
            }
            string num_rd = rs2.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string immeletofive=v.substr(0,7);
            string immfourtozero=v.substr(7);
            string k=immeletofive+a+b+funct3+immfourtozero+opcode;
            string z=binary_hex(k);
            return z;
        }
        int k1=stoi(imm);
        if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string num_rd = rs2.substr(1);
        int rs_2 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rs_2);
        string b=decimal_binary(rs_1);
        string c=negorpos_imm(imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string immeletofive=c.substr(0,7);
        string immfourtozero=c.substr(7);
        string k=immeletofive+a+b+funct3+immfourtozero+opcode;
        string z=binary_hex(k);
        return z;
    }
    if(tokens[0]=="sh"){
        if(checkS(tokens,z)=="no") return "no";
        string opcode="0100011";
        string funct3="001";
        string rs2=tokens[1];
        size_t i=0;
        string imm="";
        while(tokens[2][i]!='('){
            imm=imm+tokens[2][i];
            i++;
            if(tokens[2][i]=='(' && i!=tokens[2].size()){
                i+=1;
                break;
            }
        }
        string rs1="";
        while(tokens[2][i]!=')' && i!=tokens[2].size()){
            if(tokens[2][i]==')'){
                break;
            }
            rs1=rs1+tokens[2][i];
            i++;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        if(imm[1]=='x' && imm[0]=='0'){
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (v.size() < 12) {
                v = "0" + v;
            }
            string num_rd = rs2.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string immeletofive=v.substr(0,7);
            string immfourtozero=v.substr(7);
            string k=immeletofive+a+b+funct3+immfourtozero+opcode;
            string z=binary_hex(k);
            return z;
        }
        int k1=stoi(imm);
        if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string num_rd = rs2.substr(1);
        int rs_2 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rs_2);
        string b=decimal_binary(rs_1);
        string c=negorpos_imm(imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string immeletofive=c.substr(0,7);
        string immfourtozero=c.substr(7);
        string k=immeletofive+a+b+funct3+immfourtozero+opcode;
        string z=binary_hex(k);
        return z;
    }
    if(tokens[0]=="sw"){
        if(checkS(tokens,z)=="no") return "no";
        string opcode="0100011";
        string funct3="010";
        string rs2=tokens[1];
        size_t i=0;
        string imm="";
        while(tokens[2][i]!='(' && i!=tokens[2].size()){
            imm=imm+tokens[2][i];
            i++;
            if(tokens[2][i]=='('){
                i+=1;
                break;
            }
        }
        string rs1="";
        while(tokens[2][i]!=')' && i!=tokens[2].size()){
            if(tokens[2][i]==')'){
                break;
            }
            rs1=rs1+tokens[2][i];
            i++;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        if(imm[1]=='x' && imm[0]=='0'){
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (v.size() < 12) {
                v = "0" + v;
            }
            string num_rd = rs2.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string immeletofive=v.substr(0,7);
            string immfourtozero=v.substr(7);
            string k=immeletofive+a+b+funct3+immfourtozero+opcode;
            string z=binary_hex(k);
            return z;
        }
        int k1=stoi(imm);
        if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string num_rd = rs2.substr(1);
        int rs_2 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rs_2);
        string b=decimal_binary(rs_1);
        string c=negorpos_imm(imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string immeletofive=c.substr(0,7);
        string immfourtozero=c.substr(7);
        string k=immeletofive+a+b+funct3+immfourtozero+opcode;
        string z=binary_hex(k);
        return z;
    }
    if(tokens[0]=="sd"){
        if(checkS(tokens,z)=="no") return "no";
        string opcode="0100011";
        string funct3="011";
        string rs2=tokens[1];
        size_t i=0;
        string imm="";
        while(tokens[2][i]!='(' && i!=tokens[2].size()){
            imm=imm+tokens[2][i];
            i++;
            if(tokens[2][i]=='('){
                i+=1;
                break;
            }
        }
        string rs1="";
        while(tokens[2][i]!=')' && i!=tokens[2].size()){
            if(tokens[2][i]==')'){
                break;
            }
            rs1=rs1+tokens[2][i];
            i++;
        }
        if (alias.find(rs1) != alias.end()) {
            rs1 = alias[rs1];
        }
        if(imm[1]=='x' && imm[0]=='0'){
            imm=imm.substr(2);
            int k1=hex_dec(imm);
            if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
            string v=hex_binary(imm);
	        if(v.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (v.size() < 12) {
                v = "0" + v;
            }
            string num_rd = rs2.substr(1);
            int rd_1 = stoi(num_rd);
            string num_rs1 = rs1.substr(1);
            int rs_1 = stoi(num_rs1);
            string a=decimal_binary(rd_1);
            string b=decimal_binary(rs_1);
            string immeletofive=v.substr(0,7);
            string immfourtozero=v.substr(7);
            string k=immeletofive+a+b+funct3+immfourtozero+opcode;
            string z=binary_hex(k);
            return z;
        }
        int k1=stoi(imm);
        if(k1>2047 || k1<-2048){cout<<"Error at line "<<z<<" Immediate ranges from -2048 to 2047"<<endl;return "no";}
        string num_rd = rs2.substr(1);
        int rs_2 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rs_2);
        string b=decimal_binary(rs_1);
        string c=negorpos_imm(imm);
	    if(c.size()>12) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        string immeletofive=c.substr(0,7);
        string immfourtozero=c.substr(7);
        string k=immeletofive+a+b+funct3+immfourtozero+opcode;
        string z=binary_hex(k);
        return z;
    }
    return "no";
}

//Function for the B format instructions
string B(vector<string> tokens,vector<pair<string,int>> labels,int z){
    if(tokens[0]=="beq"){
        if(checkB(tokens,z)=="no") return "no";
        string opcode="1100011";
        string funct3="000";
        string rs1=tokens[1];
        string rs2=tokens[2];
        int imm=-1;
        for(size_t i=0;i<labels.size();i++){
            if(labels[i].first==tokens[3]){
                imm=(labels[i].second-z)*4;
                break;
            }
        }
        string c="";
        string imm_1=to_string(imm);
        int r=0,j=0;
        if(imm==-1) {
            if(tokens[3][0]=='0' && tokens[3][1]=='x'){
                if(tokens[3].substr(2).size()>3) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                if(tokens[3][0]=='0' && tokens[3][1]=='x'&& all_of(tokens[3].begin()+2,tokens[3].end() ,::isxdigit)) j=1;
                if(j==1) r++;
                if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                string z1=hex_binary(tokens[3].substr(2));
                c=z1;
                int k1=hex_dec(tokens[3].substr(2));
                if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 13 bits"<<endl;return "no";}
                while(c.size()<13) c='0'+c;
            } 
            else{
                string imm_1=tokens[3];
                if(imm_1[0]=='-') {
                    int j=all_of(tokens[3].begin()+1,tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(tokens[3].begin(),tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            }   
        }
        else{
            if(imm>4094 || imm<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
            string imm_1=to_string(imm);
                if(imm_1[0]=='-') {
                    int j=all_of(imm_1.begin()+1,imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(imm_1.begin(),imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        }
        string num_rd = rs2.substr(1);
        int rs_2 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rs_2);
        string b=decimal_binary(rs_1);
        string immtentofive=c.substr(2,6);
        string immoneftofour=c.substr(8,4);
        string twelve=c.substr(0,1);
        string eleven=c.substr(1,1);
        string k=twelve+immtentofive+a+b+funct3+immoneftofour+eleven+opcode;
        string z1=binary_hex(k);
        return z1;
    }
    if(tokens[0]=="bne"){
        if(checkB(tokens,z)=="no") return "no";
        string opcode="1100011";
        string funct3="001";
        string rs1=tokens[1];
        string rs2=tokens[2];
        int imm=-1;
        for(size_t i=0;i<labels.size();i++){
            if(labels[i].first==tokens[3]){
                imm=(labels[i].second-z)*4;
                break;
            }
        }
        string c="";
        string imm_1=to_string(imm);
        int r=0,j=0;
        if(imm==-1) {
            if(tokens[3][0]=='0' && tokens[3][1]=='x'){
                if(tokens[3].substr(2).size()>3) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                if(tokens[3][0]=='0' && tokens[3][1]=='x'&& all_of(tokens[3].begin()+2,tokens[3].end() ,::isxdigit)) j=1;
                if(j==1) r++;
                if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                string z1=hex_binary(tokens[3].substr(2));
                c=z1;
                int k1=hex_dec(tokens[3].substr(2));
                if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 13 bits"<<endl;return "no";}
                while(c.size()<13) c='0'+c;
            } 
            else{
                string imm_1=tokens[3];
                if(imm_1[0]=='-') {
                    int j=all_of(tokens[3].begin()+1,tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(tokens[3].begin(),tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            }   
        }
        else{
            if(imm>4094 || imm<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
            string imm_1=to_string(imm);
                if(imm_1[0]=='-') {
                    int j=all_of(imm_1.begin()+1,imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(imm_1.begin(),imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        }
        string num_rd = rs2.substr(1);
        int rs_2 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rs_2);
        string b=decimal_binary(rs_1);
        string immtentofive=c.substr(2,6);
        string immoneftofour=c.substr(8,4);
        string twelve=c.substr(0,1);
        string eleven=c.substr(1,1);
        string k=twelve+immtentofive+a+b+funct3+immoneftofour+eleven+opcode;
        string z=binary_hex(k);
        return z;
    }
    if(tokens[0]=="blt"){
        if(checkB(tokens,z)=="no") return "no";
        string opcode="1100011";
        string funct3="100";
        string rs1=tokens[1];
        string rs2=tokens[2];
        int imm=-1;
        for(size_t i=0;i<labels.size();i++){
            if(labels[i].first==tokens[3]){
                imm=(labels[i].second-z)*4;
                break;
            }
        }
        string c="";
        string imm_1=to_string(imm);
        int r=0,j=0;
        if(imm==-1) {
            if(tokens[3][0]=='0' && tokens[3][1]=='x'){
                if(tokens[3].substr(2).size()>3) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                if(tokens[3][0]=='0' && tokens[3][1]=='x'&& all_of(tokens[3].begin()+2,tokens[3].end() ,::isxdigit)) j=1;
                if(j==1) r++;
                if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                string z1=hex_binary(tokens[3].substr(2));
                c=z1;
                int k1=hex_dec(tokens[3].substr(2));
                if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 13 bits"<<endl;return "no";}
                while(c.size()<13) c='0'+c;
            } 
            else{
                string imm_1=tokens[3];
                if(imm_1[0]=='-') {
                    int j=all_of(tokens[3].begin()+1,tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(tokens[3].begin(),tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            }   
        }
        else{
            if(imm>4094 || imm<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
            string imm_1=to_string(imm);
                if(imm_1[0]=='-') {
                    int j=all_of(imm_1.begin()+1,imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(imm_1.begin(),imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        }
        string num_rd = rs2.substr(1);
        int rs_2 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rs_2);
        string b=decimal_binary(rs_1);
        string immtentofive=c.substr(2,6);
        string immoneftofour=c.substr(8,4);
        string twelve=c.substr(0,1);
        string eleven=c.substr(1,1);
        string k=twelve+immtentofive+a+b+funct3+immoneftofour+eleven+opcode;
        string z=binary_hex(k);
        return z;
    }
    if(tokens[0]=="bge"){
        if(checkB(tokens,z)=="no") return "no";
        string opcode="1100011";
        string funct3="101";
        string rs1=tokens[1];
        string rs2=tokens[2];
        int imm=-1;
        for(size_t i=0;i<labels.size();i++){
            if(labels[i].first==tokens[3]){
                imm=(labels[i].second-z)*4;
                break;
            }
        }
        string c="";
        string imm_1=to_string(imm);
        int r=0,j=0;
        if(imm==-1) {
            if(tokens[3][0]=='0' && tokens[3][1]=='x'){
                if(tokens[3].substr(2).size()>3) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                if(tokens[3][0]=='0' && tokens[3][1]=='x'&& all_of(tokens[3].begin()+2,tokens[3].end() ,::isxdigit)) j=1;
                if(j==1) r++;
                if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                string z1=hex_binary(tokens[3].substr(2));
                c=z1;
                int k1=hex_dec(tokens[3].substr(2));
                if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 13 bits"<<endl;return "no";}
                while(c.size()<13) c='0'+c;
            } 
            else{
                string imm_1=tokens[3];
                if(imm_1[0]=='-') {
                    int j=all_of(tokens[3].begin()+1,tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(tokens[3].begin(),tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            }   
        }
        else{
            if(imm>4094 || imm<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
            string imm_1=to_string(imm);
                if(imm_1[0]=='-') {
                    int j=all_of(imm_1.begin()+1,imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(imm_1.begin(),imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        }
        string num_rd = rs2.substr(1);
        int rs_2 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rs_2);
        string b=decimal_binary(rs_1);
        string immtentofive=c.substr(2,6);
        string immoneftofour=c.substr(8,4);
        string twelve=c.substr(0,1);
        string eleven=c.substr(1,1);
        string k=twelve+immtentofive+a+b+funct3+immoneftofour+eleven+opcode;
        string z=binary_hex(k);
        return z;
    }
    if(tokens[0]=="bltu"){
        if(checkB(tokens,z)=="no") return "no";
        string opcode="1100011";
        string funct3="110";
        string rs1=tokens[1];
        string rs2=tokens[2];
        int imm=-1;
        for(size_t i=0;i<labels.size();i++){
            if(labels[i].first==tokens[3]){
                imm=(labels[i].second-z)*4;
                break;
            }
        }
        string c="";
        string imm_1=to_string(imm);
        int r=0,j=0;
        if(imm==-1) {
            if(tokens[3][0]=='0' && tokens[3][1]=='x'){
                if(tokens[3].substr(2).size()>3) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                if(tokens[3][0]=='0' && tokens[3][1]=='x'&& all_of(tokens[3].begin()+2,tokens[3].end() ,::isxdigit)) j=1;
                if(j==1) r++;
                if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                string z1=hex_binary(tokens[3].substr(2));
                c=z1;
                int k1=hex_dec(tokens[3].substr(2));
                if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4095"<<endl;return "no";}
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 13 bits"<<endl;return "no";}
                while(c.size()<13) c='0'+c;
            } 
            else{
                string imm_1=tokens[3];
                if(imm_1[0]=='-') {
                    int j=all_of(tokens[3].begin()+1,tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(tokens[3].begin(),tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            }   
        }
        else{
            if(imm>4094 || imm<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
            string imm_1=to_string(imm);
                if(imm_1[0]=='-') {
                    int j=all_of(imm_1.begin()+1,imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(imm_1.begin(),imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        }
        string num_rd = rs2.substr(1);
        int rs_2 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rs_2);
        string b=decimal_binary(rs_1);
        string immtentofive=c.substr(2,6);
        string immoneftofour=c.substr(8,4);
        string twelve=c.substr(0,1);
        string eleven=c.substr(1,1);
        string k=twelve+immtentofive+a+b+funct3+immoneftofour+eleven+opcode;
        string z=binary_hex(k);
        return z;
    }
    if(tokens[0]=="bgeu"){
        if(checkB(tokens,z)=="no") return "no";
        string opcode="1100011";
        string funct3="111";
        string rs1=tokens[1];
        string rs2=tokens[2];
        int imm=-1;
        for(size_t i=0;i<labels.size();i++){
            if(labels[i].first==tokens[3]){
                imm=(labels[i].second-z)*4;
                break;
            }
        }
        string c="";
        string imm_1=to_string(imm);
        int r=0,j=0;
        if(imm==-1) {
            if(tokens[3][0]=='0' && tokens[3][1]=='x'){
                if(tokens[3].substr(2).size()>3) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                if(tokens[3][0]=='0' && tokens[3][1]=='x'&& all_of(tokens[3].begin()+2,tokens[3].end() ,::isxdigit)) j=1;
                if(j==1) r++;
                if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                string z1=hex_binary(tokens[3].substr(2));
                c=z1;
                int k1=hex_dec(tokens[3].substr(2));
                if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 13 bits"<<endl;return "no";}
                while(c.size()<13) c='0'+c;
            } 
            else{
                string imm_1=tokens[3];
                if(imm_1[0]=='-') {
                    int j=all_of(tokens[3].begin()+1,tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(tokens[3].begin(),tokens[3].end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=stoi(tokens[3]);
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            }   
        }
        else{
            if(imm>4094 || imm<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
            string imm_1=to_string(imm);
                if(imm_1[0]=='-') {
                    int j=all_of(imm_1.begin()+1,imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1.substr(1));
                    string a=decimal_binary(imm);
                    while (a.size() < 13) {
                        a = "0" + a;
                    }
                    c=find_twos_complement(a);
                }
                else{
                    int j=all_of(imm_1.begin(),imm_1.end() ,::isdigit);
                    if(j==1) r++;
                    if(r!=1) { cout<<"Error at line "<<z<<" Enter valid immediate(No label exists)"<<endl; return "no";}
                    int k1=imm;
                    if(k1>4094 || k1<-4096){cout<<"Error at line "<<z<<" Immediate ranges from -4096 to 4094"<<endl;return "no";}
                    imm = stoll(imm_1);
                    c=decimal_binary(imm);
                    while (c.size() < 13) {
                        c = "0" + c;
                    }
                }
                // c=c+'0';
                // c=negorpos_imm(imm_1);
                if(c.size()>13) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
        }
        string num_rd = rs2.substr(1);
        int rs_2 = stoi(num_rd);
        string num_rs1 = rs1.substr(1);
        int rs_1 = stoi(num_rs1); 
        string a=decimal_binary(rs_2);
        string b=decimal_binary(rs_1);
        string immtentofive=c.substr(2,6);
        string immoneftofour=c.substr(8,4);
        string twelve=c.substr(0,1);
        string eleven=c.substr(1,1);
        string k=twelve+immtentofive+a+b+funct3+immoneftofour+eleven+opcode;
        string z=binary_hex(k);
        return z;
    }
    return "no";
}

//Function for the U format instructions
string U(vector<string> tokens,vector<pair<string,int>> labels,int z){
    if(tokens[0]=="lui"){ int r=0;
        if(tokens.size()!=3) {cout<<"Enter complete command. Error at line "<<z<<endl;return "no";}
        for(size_t i=0;i<validRegisters.size();i++){
            if(tokens[1]==validRegisters[i]) r++;
        }
        if(r!=1) return "no";
        string opcode="0110111";
        string rd=tokens[1];
        string imm=tokens[2];
        string z3="";
        if(imm[1]=='x' && imm[0]=='0'){
            int j=0;
            if(tokens[2][0]=='0' && tokens[2][1]=='x' && all_of(tokens[2].begin()+2,tokens[2].end() ,::isxdigit)) j=1;
            if(j!=1) return "no";
            imm=imm.substr(2);
            if(imm.size()>8) {cout<<"Error at line "<< z <<" Immediate exceeding 32 bits"<<endl;return "no";}
            if(imm.size()<=8 && imm.size()>5) imm=imm.substr(imm.size()-5);
            for(size_t i=imm.size()-1;i>=0;i--){
                if(imm.size()<5){
                    while(imm.size()!=5) imm='0'+imm;
                }
                else{
                    break;
                }
            }
            
            long long int z1=hex_dec(imm);
            if(z1>4294967295 || z1<0){cout<<"Error at line "<<z<<" Immediate range is from 0 to 4294967295=2^32-1 and first 20 bits are copnsidered"<<endl;return "no";}
            string num_rd = rd.substr(1);
            int rs_2 = stoi(num_rd);
            string a=decimal_binary(rs_2);
            return imm+binary_hex(a+opcode);
        }
        else{
            long long int imm_1=stoll(imm);
            if(imm_1>4294967295 || imm_1<0){cout<<"Error at line "<<z<<" Immediate range is from 0 to 4294967295=2^32-1 and first 20 bits are considred"<<endl;return "no";}
            string imm_11=decimal_binary(imm_1);
            if(imm_11.size()>32) {cout<<"Error at line "<<z<<" Immediate exceeding 20 bits"<<endl;return "no";}
            if(imm_11.size()>20 && imm_11.size()<32){imm_11=imm_11.substr(imm_11.size()-20);}
            while(imm_11.size()!=20){
                imm_11='0'+imm_11;
            }
            string num_rd = rd.substr(1);
            int rs_2 = stoi(num_rd);
            string a=decimal_binary(rs_2);
            return binary_hex(imm_11+a+opcode);
        }
    }
    else if(tokens[0]=="auipc"){
        string opcode="0010111";
        string rd=tokens[1];
        string imm=tokens[2];
        string z="";
        if(imm[1]=='x'){
            imm=imm.substr(2);
            if(z.size()>5) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            for(size_t i=imm.size()-1;i>=0;i--){
                if(imm.size()<5){
                    while(imm.size()!=5) imm='0'+imm;
                }
                else{
                    z=imm[i]+z;
                    if(z.size()==5){
                        imm=z;
                        break;
                    }
                }
            }
            string num_rd = rd.substr(1);
            int rs_2 = stoi(num_rd);
            string a=decimal_binary(rs_2);
            return imm+binary_hex(a+opcode);
        }
        else{
            int imm_1=stoi(imm);
            while(imm_1!=0){
                z = to_string(imm_1%2) + z;
                imm_1=imm_1/2;
            }
            if(z.size()>20){
                z=z.substr(z.size()-20);
            }
            while (z.size() <= 20) {
                z = "0" + z;
            }
            z=z.substr(1);
            string num_rd = rd.substr(1);
            int rs_2 = stoi(num_rd);
            string a=decimal_binary(rs_2);
            return binary_hex(z+a+opcode);
        }
    }
    return "no";
}

//Function for the J format instructions
string J(vector<string> tokens,vector<pair<string,int>> labels,int z){
    if(tokens[0]=="jal"){
        if(tokens.size()!=3) {cout<<"Enter complete command. Error at line "<<z<<endl;return "no";}
        string opcode="1101111";
        string rd= tokens[1];
        string imm="";
        if (alias.find(tokens[1]) != alias.end()) {
            tokens[1] = alias[tokens[1]];
        }
        int count=0;
        for(size_t i=0;i<validRegisters.size();i++){
            if(tokens[1]==validRegisters[i]){
                count=1;
                break;
            }
        }
        if(count!=1) {cout<<"Register "<<tokens[1]<<" is not a valid register"<<endl; return "no";}
        if(tokens[2][1]=='x' && tokens[2][0]=='0'){
            string imm=tokens[2];
            imm=imm.substr(2);
            long long int imm_1=hex_dec(imm);
            if(imm_1>1048575 || imm_1<-1048576){cout<<"Error at line "<<z<<" Immediate range is from -1048576 to 1048575=2^20-1"<<endl;return "no";}
            string a="";
            a=hex_binary(imm);
            cout<<a<<endl;
            if(a.size()>22) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
            while (a.size() < 22) 
                a = "0" + a;
            string num_rd = rd.substr(1);
            int rs_2 = stoi(num_rd);
            string b=decimal_binary(rs_2);
            string immtentoone=a.substr(11,10);
            string immnineteentotwelve=a.substr(2,8);
            string eleven=a.substr(10,1);
            string k=a[0]+immtentoone+eleven+immnineteentotwelve+b+opcode;
            string z=binary_hex(k);
            return z;
        }
        else if(all_of(tokens[2].begin(), tokens[2].end(), ::isdigit) || (tokens[2][0] == '-' && all_of(tokens[2].begin()+1, tokens[2].end(), ::isdigit))){
            string imm=tokens[2];
            string a="";
            if(imm[0]=='-'){
                imm=imm.substr(1);
                int imm_11 = stoi(imm);
                if(imm_11>1048575 || imm_11<-1048576){cout<<"Error at line "<<z<<" Immediate range is from -1048576 to 1048575=2^20-1"<<endl;return "no";}
                a=decimal_binary(imm_11);
                if(a.size()>22) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
                while (a.size() < 22) {
                    a='0'+a;
                }
                a=find_twos_complement(a);
            }
            else{
                // a=decimal_binary(imm);
                a="";
                int imm_11 = stoi(imm);
                if(imm_11>1048575 || imm_11<-1048576){cout<<"Error at line "<<z<<" Immediate range is from -1048576 to 1048575=2^20-1"<<endl;return "no";}
                a=decimal_binary(imm_11);
                if(a.size()>22) {cout<<"Error at line "<<z<<" Immediate exceeding 12 bits"<<endl;return "no";}
                while (a.size() < 22) {
                    a = "0" + a;
                }
                // a=find_twos_complement(a);
            }
            string num_rd = rd.substr(1);
            int rs_2 = stoi(num_rd);
            string b=decimal_binary(rs_2);
            string immtentoone=a.substr(11,10);
            string immnineteentotwelve=a.substr(2,8);
            string eleven=a.substr(10,1);
            string k=a[0]+immtentoone+eleven+immnineteentotwelve+b+opcode;
            string z=binary_hex(k);
            return z;
        }
        else{
            int imm=-1;
            string a="";
            for(size_t i=0;i<labels.size();i++){
                if(labels[i].first==tokens[2]){
                    imm=(labels[i].second-z)*4;
                    break;
                }
            }
            string imm_1=to_string(imm);
            if(imm_1[0]=='-'){
                imm_1=imm_1.substr(1);
                int imm_11 = stoi(imm_1);
                if(imm_11>1048575 || imm_11<-1048576){cout<<"Error at line "<<z<<" Immediate range is from  to 1048575=2^20-1"<<endl;return "no";}
                a=decimal_binary(imm_11);
                while (a.size() < 22) {
                    a='0'+a;
                }
                a=find_twos_complement(a);
            }
            else{
                if(imm>1048575 || imm<-1048576){cout<<"Error at line "<<z<<" Immediate range is from 0 to 1048575=2^20-1"<<endl;return "no";}
                a=decimal_binary(imm);
                while (a.size() < 22) {
                    a = "0" + a;
                }
            }
            string num_rd = rd.substr(1);
            int rs_2 = stoi(num_rd);
            string b=decimal_binary(rs_2);
            string immtentoone=a.substr(11,10);
            string immnineteentotwelve=a.substr(1,8);
            string eleven=a.substr(10,1);
            string k=a[0]+immtentoone+eleven+immnineteentotwelve+b+opcode;
            string z=binary_hex(k);
            return z;
        }
    }
    return "no";
}

void processFile(const string &filename) {
    ofstream hi("output.hex");
    ifstream file(filename);  // Open the .s file to check for the labels
    string input = "";

    if (!file.is_open()) {
        cerr << "Unable to open file" << endl;
        return;
    }

    vector<char> delimiters = {' ', ','};  // Specify delimiters
    int z=1;
    set<string> seenlabels;
    while (getline(file, input)) {  
        vector<string> tokens = split(input, ' ', ',' , '\t');  // Split line into tokens
        for(size_t i=0;i<tokens[0].size();i++){
            if(tokens[0][i]==':' && i!= tokens[0].size()-1){
                labels.push_back(make_pair(tokens[0].substr(0,i),z));
                tokens[0]=tokens[0].substr(i+1);
                break;
            }
        }
        if(tokens[0].back()==':'){
            labels.emplace_back(tokens[0].substr(0, tokens[0].length() - 1), z);
        }
        z++;
    }
    set<string> label_names;
    for (const auto& label : labels) {
        label_names.insert(label.first);
    }

    // Compare the size of the set with the size of the vector
    if (label_names.size() != labels.size()) {
        cout << "Error: Duplicate labels found" << endl;
        return;  // Exit with an error code
    }
    file.close();
    //Opening file again to check the functions
    file.open(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file" << endl;
        return;
    }
    string mem_write="0000000000010000";
    while(mem_write!="0000000000050000"){
        memory.emplace_back(addHex(mem_write,"0"),"00");
        mem_write=addHex(mem_write,"1");
    }
    mem_write="0000000000010000";
    z=1;
    int s=0;
    while (getline(file, input)) { 
        vector<string> tokens = split(input, ' ', ',','\t');
        if(tokens[0]==".data"){s=1;counter++;z++;continue;}
        if(s!=1 && (tokens[0]==".dword" || tokens[0]==".word" || tokens[0]==".half" || tokens[0]==".byte")){
            cout<<"Data section needs to be mentioned in .s file for loading data in memory"<<endl;
            return;
        }
        if(s==1){
            if (tokens[0] == ".dword") {
                d = 0;
                if(tokens.size()==1){
                    cout<<"Invalid data"<<endl;
                    return;
                }
                for (size_t h = 1; h < tokens.size(); h++) {
                    int x = 0;
                    if (tokens[h][0] == '0' && tokens[h][1] == 'x') {
                        tokens[h] = tokens[h].substr(2);
                        if (tokens[h].size() > 16 || tokens[h].size() <= 0) {
                            cout << "Value in memory exceeds 64 bits" << endl;
                            return;
                        }
                        if (all_of(tokens[h].begin(), tokens[h].end(), ::isxdigit)) {
                            x = 1;
                        }
                        if (x == 1) {
                            while (tokens[h].size() < 16) tokens[h] = '0' + tokens[h];  // Fix the padding logic
                        }
                    } else {
                        if (tokens[h].size() > 160000 || tokens[h].size() <= 0) {
                            cout << "Value in memory exceeds 64 bits" << endl;
                            return;
                        }
                        if (all_of(tokens[h].begin(), tokens[h].end(), ::isdigit)) {
                            x = 1;
                        }
                        tokens[h] = decimalToHex(stoll(tokens[h]));
                        if (x == 1) {
                            while (tokens[h].size() < 16) tokens[h] = '0' + tokens[h];  // Fix padding
                        }
                    }

                    // Insert bytes into memory
                    for (int j = tokens[h].size() - 2; j >= 0; j -= 2) {
                        string byte = tokens[h].substr(j, 2);
                        string address = addHex(mem_write, "0");
                        for (auto& pair : memory) {
                            if (pair.first == mem_write) {   // Find the address in memory
                                pair.second = byte; 
                                break;                       // Stop once you find and update the value
                            }
                        }  // Insert full address without substr
                        mem_write = addHex(mem_write, "1");  // Update memory pointer
                    }
                }
                z++;
                counter++;
                continue;
            }

            if(tokens[0]==".word"){
                d=1;
                if(tokens.size()==1){
                    cout<<"Invalid data"<<endl;
                    return;
                }
                for(size_t h=1;h<tokens.size();h++){
                    int x=0;
                    if(tokens[h][0]=='0' && tokens[h][1]=='x'){
                        tokens[h]=tokens[h].substr(2);
                        if(tokens[h].size()>8 || tokens[h].size()<=0) {cout<<"Value in memory exceeds 32 bits"<<endl;return;}
                        if(all_of(tokens[h].begin(),tokens[h].end() ,::isxdigit)){
                            x=1;
                        }
                        if(x==1){
                            tokens[h]=tokens[h].substr(2);
                            if(tokens[h].size()<8){
                                while(tokens[h].size()<=8) tokens[h]='0'+tokens[h];
                            }
                        }
                    }
                    else{
                        if(tokens[h].size()>8000 || tokens[h].size()<=0) {cout<<"Value in memory exceeds 32 bits"<<endl;return;}
                        if(all_of(tokens[h].begin(),tokens[h].end() ,::isdigit)){
                            x=1;
                        }
                        tokens[h]=removeLeadingZeros1(decimalToHex(stoll(tokens[h])));
                        if(x==1){
                            if(tokens[h].size()<8){
                                while(tokens[h].size()<=8) tokens[h]='0'+tokens[h];
                            }
                        }
                    }
                    for (int j = tokens[h].size() - 2; j >= 0; j -= 2) {
                        string byte = tokens[h].substr(j, 2);
                        string address = addHex(mem_write, "0");
                        for (auto& pair : memory) {
                            if (pair.first == mem_write) {  // Find the address in memory
                                pair.second = byte; 
                                break;                       // Stop once you find and update the value
                            }
                        }  // Insert full address without substr
                        mem_write = addHex(mem_write, "1");  // Update memory pointer
                    }
                }
                z++;
                counter++;
                continue;
            }
            if(tokens[0]==".half"){
                d=2;
                if(tokens.size()==1){
                    cout<<"Invalid data"<<endl;
                    return;
                }
                for(size_t h=1;h<tokens.size();h++){
                    int x=0;
                    if(tokens[h][0]=='0' && tokens[h][1]=='x'){
                        tokens[h]=tokens[h].substr(2);
                        if(tokens[h].size()>4 || tokens[h].size()<=0) {cout<<"Value in memory exceeds 16 bits"<<endl;return;}
                        if(all_of(tokens[h].begin(),tokens[h].end() ,::isxdigit)){
                            x=1;
                        }
                        if(x==1){
                            if(tokens[h].size()<4){
                                while(tokens[h].size()<=4) tokens[h]='0'+tokens[h];
                            }
                        }
                    }
                    else{
                        if(tokens[h].size()>4000 || tokens[h].size()<=0) {cout<<"Value in memory exceeds 16 bits"<<endl;return;}
                        if(all_of(tokens[h].begin(),tokens[h].end() ,::isdigit)){
                            x=1;
                        }
                        tokens[h]=removeLeadingZeros1(decimalToHex(stoll(tokens[h])));
                        if(x==1){
                            if(tokens[h].size()<4){
                                while(tokens[h].size()<=4) tokens[h]='0'+tokens[h];
                            }
                        }
                    }
                    for (int j = tokens[h].size() - 2; j >= 0; j -= 2) {
                        string byte = tokens[h].substr(j, 2);
                        string address = addHex(mem_write, "0");
                        for (auto& pair : memory) {
                            if (pair.first == mem_write) {   // Find the address in memory
                                pair.second = byte; 
                                break;                       // Stop once you find and update the value
                            }
                        }  // Insert full address without substr
                        mem_write = addHex(mem_write, "1");  // Update memory pointer
                    }
                }
                z++;
                counter++;
                continue;
            }
            if(tokens[0]==".byte"){
                d=3;
                if(tokens.size()==1){
                    cout<<"Invalid data"<<endl;
                    return;
                }
                for(size_t h=1;h<tokens.size();h++){
                    int x=0;
                    if(tokens[h][0]=='0' && tokens[h][1]=='x'){
                        tokens[h]=tokens[h].substr(2);
                        if(tokens[h].size()>2 || tokens[h].size()<=0) {cout<<"Value in memory exceeds 8 bits"<<endl;return;}
                        if(all_of(tokens[h].begin(),tokens[h].end() ,::isxdigit)){
                            x=1;
                        }
                        if(x==1){
                            if(tokens[h].size()<2){
                                while(tokens[h].size()<=2) tokens[h]='0'+tokens[h];
                            }
                        }
                    }
                    else{
                        if(tokens[h].size()>2000 || tokens[h].size()<=0) {cout<<"Value in memory exceeds 8 bits"<<endl;return;}
                        if(all_of(tokens[h].begin(),tokens[h].end() ,::isdigit)){
                            x=1;
                        }
                        tokens[h]=removeLeadingZeros1(decimalToHex(stoll(tokens[h])));
                        if(x==1){
                            if(tokens[h].size()<2){
                                while(tokens[h].size()<=2) tokens[h]='0'+tokens[h];
                            }
                        }
                    }
                    for (int j = tokens[h].size() - 2; j >= 0; j -= 2) {
                        string byte = tokens[h].substr(j, 2);
                        string address = addHex(mem_write, "0");
                        for (auto& pair : memory) {
                            if (pair.first == mem_write) {   // Find the address in memory
                                pair.second = byte; 
                                break;                       // Stop once you find and update the value
                            }
                        }  // Insert full address without substr
                        mem_write = addHex(mem_write, "1");  // Update memory pointer
                    }
                }
                z++;
                counter++;
                continue;
            }
        }
        
        if(tokens[0]==".section .text"){counter++;z++;continue;}
        if(tokens[0]==".text"){counter++;z++;continue;}
        for(size_t i=0;i<tokens[0].size();i++){
            if(tokens[0][i]==':' && i!= tokens[0].size()-1){
                tokens[0]=tokens[0].substr(i+1);
                break;
            }
        }
        
        for(size_t i=0;i<tokens[0].size();i++){
            if(tokens[0][i]==':' && i!= tokens[0].size()-1){
                tokens[0]=tokens[0].substr(i+1);
                break;
            }
        }
        if(tokens.size()==1 && tokens[0]!=".text" && tokens[0]!=".data"){
            cout<<"Error at line "<<z<< " label: ??"<<endl;
            return;
        }
        if(tokens[0].back()==':'){
            tokens[0]=tokens[1];
            if(tokens[0].size()>4 && tokens[0]!="sltiu" && tokens[0]!="auipc"){
                cout<<"Error at line "<<z-1<<" Opcode undefined "<<endl;
                return;
            }
            if(tokens.size()>4){
                tokens[1]=tokens[2];
                tokens[2]=tokens[3];
                tokens[3]=tokens[4];
                tokens.erase(tokens.begin() + 4);
            }
            else if(tokens.size()>3){
                tokens[1]=tokens[2];
                tokens[2]=tokens[3];
                tokens.erase(tokens.begin() + 3);
            }
            else if(tokens.size()>2){
                tokens[1]=tokens[2];
                tokens.erase(tokens.begin() + 2);
            }
            else{
                cout<<"Error in command at line "<<z<<" Give complete command."<<endl;
                break;
            }
        }

        for (size_t i = 1; i < tokens.size(); i++) {
            if (alias.find(tokens[i]) != alias.end()) {
                tokens[i] = alias[tokens[i]];
            }
        }
        if(R(tokens,z)!="no") hi<<R(tokens,z)<<endl;
        else if(I(tokens,z)!="no") hi<<I(tokens,z)<<endl;
        else if(S(tokens,z)!="no") hi<<S(tokens,z)<<endl;
        else if(B(tokens,labels,z)!="no") hi<<B(tokens,labels,z)<<endl;
        else if(U(tokens,labels,z)!="no") hi<<U(tokens,labels,z)<<endl;
        else if(J(tokens,labels,z)!="no") hi << J(tokens,labels,z) << endl;
        else {hi<<"Error at line "<<z<<endl;cout<<"Enter proper instruction format. "<<"Error at line "<<z<<endl;}
        

        z++;
    }
    file.close(); 
    hi.close();
}
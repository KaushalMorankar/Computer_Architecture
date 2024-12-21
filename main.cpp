#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include<cmath>
#include <iomanip>
#include<sstream> 
#include <thread>
#include<stack>
#include<queue>
#include <list>
#include <random>
#include "test.hh"

using namespace std;
int djwala=0;
map<string,vector<string>> return_adresses;
map<string,int> curr_ret;
int curr_line=0;
vector<string> actual_lines;
long long int lines;
long long int block_sizes=0;
int hits=0,misses=0;
int flag=0;
//Direct mapped cache
string filename;
bool cache_on=false;
//Direct mapped cache
long long int associate=0;  // Set associativity level
struct CacheEntry {
    vector<string> tags;   // Array of tags, size defined by associate
    vector<string> data;   // Array of data, size defined by associate
    vector<bool> valid;    // Valid bits for each entry
    vector<bool> dirty_bit;       //Dirty bit for each entry
    // Constructor to initialize vectors based on associate
    CacheEntry() : tags(associate), data(associate), valid(associate, false),dirty_bit(associate,false){}
};

vector<pair<string, queue<pair<string, int>>>> fifo;
vector<pair<string, vector<pair<int, string>>>> lfu;
list<pair<string,vector<string>>> lru;
string replacement_policy="",write_policy="";
// A map to represent the direct-mapped cache with index as key
map<string, CacheEntry> direct_mapped_cache; // Index as key
long long int indexi=0;
long long int biteoffset=0;

string trimTrailing(const string &str) {
    return str; 
}

string removeLeadingZeros(const string &str) {
    size_t nonZeroPos=str.find_first_not_of('0');
    if (nonZeroPos==string::npos) {
        return "0";
    }
    return str.substr(nonZeroPos);
}

string removeLeadingSpaces(const string& str) {
    size_t start=0;
    while (start<str.size() && isspace(str[start])) {
        start++;
    }
    return str.substr(start);
}

// Function to convert binary string to decimal manually
int binaryToDecimal(string binarystr){
    int decimal=0;
    int base=1;
    int length=binarystr.length();
    bool isnegative=false;
    if(binarystr[0]=='1'){
        isnegative=true;
    };
    for(int i=length-1;i>=0;i--){
        if(binarystr[i]=='1'){
            decimal += base;
        }
        base *= 2;
    }
    if(isnegative) decimal-=base;
    return decimal;
}

int binaryToDecimalpos(const string& binary) {
    int decimal=0;
    int length=binary.length();
    
    // Loop through each character in the binary string
    for (int i=0; i<length; ++i) {
        // If the character is '1', add the corresponding power of 2
        if (binary[i]=='1') {
            decimal += pow(2, length-1-i);
        }
    }
    
    return decimal;
}

string get_rd(string hello){
    string rs1=hello.substr(12,5);
    string rs1_1="x"+to_string(binaryToDecimalpos(rs1));
    return rs1_1;
}

// List of valid registers
map<string, string> regvals={
    {"x0", "0"}, {"x1", "0"}, {"x2", "0"}, {"x3", "0"}, {"x4", "0"}, {"x5", "0"}, {"x6", "0"}, {"x7", "0"},
    {"x8", "0"}, {"x9", "0"}, {"x10", "0"}, {"x11", "0"}, {"x12", "0"}, {"x13", "0"}, {"x14", "0"}, {"x15", "0"},
    {"x16", "0"}, {"x17", "0"}, {"x18", "0"}, {"x19", "0"}, {"x20", "0"}, {"x21", "0"}, {"x22", "0"}, {"x23", "0"},
    {"x24", "0"}, {"x25", "0"}, {"x26", "0"}, {"x27", "0"}, {"x28", "0"}, {"x29", "0"}, {"x30", "0"}, {"x31", "0"}
};

void fifo_cache(){

}

void lru_cache(){

}

void random_cache(){

}

void R(string hello,string &pc){
    string funct7=hello.substr(0,7);
    string rs2=hello.substr(7,5);
    string rs1=hello.substr(12,5);
    string funct3=hello.substr(17,3);
    string rd=hello.substr(20,5);
    string rs1_1="x"+to_string(binaryToDecimalpos(rs1));
    string rs2_1="x"+to_string(binaryToDecimalpos(rs2));
    string rd_1="x"+to_string(binaryToDecimalpos(rd));
    string operation="";
    string val1=to_string(hex_dec(regvals[rs1_1]));
    string val2=to_string(hex_dec(regvals[rs2_1]));
    if(rd_1!="x0"){
        if(funct3=="000" && funct7=="0000000") {operation+="add"; regvals[rd_1]=decimalToHex(stoll(val1)+stoll(val2));}
        if(funct3=="000" && funct7=="0100000") {operation+="sub"; regvals[rd_1]=decimalToHex(stoll(val1)-stoll(val2)); }
        if(funct3=="100" && funct7=="0000000") {operation+="xor"; regvals[rd_1]=decimalToHex(stoll(val1) ^ stoll(val2));}
        if(funct3=="110" && funct7=="0000000") {operation+="or";  regvals[rd_1]=decimalToHex(stoll(val1)|stoll(val2));}
        if(funct3=="111" && funct7=="0000000") {operation+="and"; regvals[rd_1]=decimalToHex(stoll(val1)&stoll(val2));}
        if(funct3=="001" && funct7=="0000000") {operation+="sll"; regvals[rd_1]=decimalToHex(stoll(val1)*static_cast<int>(pow(2,stoll(val2))));}
        if(funct3=="101" && funct7=="0000000") {operation+="srl"; regvals[rd_1]=decimalToHex(stoll(val1)/static_cast<int>(pow(2,stoll(val2))));}
        if(funct3=="101" && funct7=="0100000") {operation+="sra"; regvals[rd_1]=decimalToHex(stoll(val1)/static_cast<int>(pow(2,stoll(val2))));}
        if(funct3=="010" && funct7=="0000000") {operation+="slt"; regvals[rd_1]=stoll(val1)-stoll(val2)>0?"0":"1";}
        if(funct3=="011" && funct7=="0000000") {operation+="sltu";regvals[rd_1]=stoll(val1)-stoll(val2)>0?"0":"1";}
    }
    else{
        if(rs1_1=="x0" && rs2_1=="x0"){
            operation+="add";
        }
    }
    while(regvals[rd_1].size()!=16) regvals[rd_1]="0"+regvals[rd_1];
    cout<<"Executed "<<removeLeadingSpaces(trimTrailing(actual_lines[curr_line]))<<"; pc=0x"<<pc.substr(8)<<endl;
    pc=addHex(pc,"4");
}

void I(string hello,string &pc){
    string rs1=hello.substr(12,5);
    string funct3=hello.substr(17,3);
    string rd=hello.substr(20,5);
    string rs1_1="x"+to_string(binaryToDecimalpos(rs1));
    string rd_1="x"+to_string(binaryToDecimalpos(rd));
    string val1=to_string(hex_dec(regvals[rs1_1]));
    string operation="",imm="";
    if(rd_1!="x0"){
        if(funct3=="000") {operation+="addi"; imm=hello.substr(0,12); int z=binaryToDecimal(imm); regvals[rd_1]=decimalToHex(stoll(val1)+z);}
        if(funct3=="100") {operation+="xori"; imm=hello.substr(0,12); int z=binaryToDecimal(imm); regvals[rd_1]=decimalToHex(stoll(val1)^z);}
        if(funct3=="110") {operation+="ori"; imm=hello.substr(0,12); int z=binaryToDecimal(imm); regvals[rd_1]=decimalToHex(stoll(val1)|z);}
        if(funct3=="111") {operation+="andi"; imm=hello.substr(0,12); int z=binaryToDecimal(imm); regvals[rd_1]=decimalToHex(stoll(val1)&z);}
        if(funct3=="001") {operation+="slli"; imm=hello.substr(6,6); string funct6=hello.substr(0,6); int z=binaryToDecimalpos(imm); regvals[rd_1]=decimalToHex(stoll(val1)<<z);}
        if(funct3=="101") {operation+="srli"; imm=hello.substr(6,6); string funct6=hello.substr(0,6); int z=binaryToDecimalpos(imm); regvals[rd_1]=decimalToHex(stoll(val1)/static_cast<int>(pow(2,z)));}
        if(funct3=="101") {operation+="srai"; imm=hello.substr(6,6); string funct6=hello.substr(0,6); int z=binaryToDecimal(imm); regvals[rd_1]=decimalToHex(stoll(val1)/static_cast<int>(pow(2,z)));}
        if(funct3=="010") {operation+="slti"; imm=hello.substr(0,12); int z=binaryToDecimal(imm); regvals[rd_1]=stoll(val1)-z>0?"0":"1";}
        if(funct3=="011") {operation+="sltiu"; imm=hello.substr(0,12); int z=binaryToDecimal(imm); regvals[rd_1]=stoll(val1)-z>0?"0":"1";}
    }
    else{
        if(rs1_1=="x0" && removeLeadingZeros(imm)==""){
            operation+="add";
        }
    }
    while(regvals[rd_1].size()!=16) regvals[rd_1]="0"+regvals[rd_1];
    cout<<"Executed "<<removeLeadingSpaces(trimTrailing(actual_lines[curr_line]))<<"; pc=0x"<<pc.substr(8)<<endl;
    pc=addHex(pc,"4");
}

void Iload(string hello,string &pc,ofstream &otpfile){
    string imm=hello.substr(0,12);
    string rs1=hello.substr(12,5);
    string funct3=hello.substr(17,3);
    string rd=hello.substr(20,5);
    string rs1_1="x"+to_string(binaryToDecimalpos(rs1));
    string rd_1="x"+to_string(binaryToDecimalpos(rd));
    string val1=regvals[rs1_1];
    string operation="";
    if(rd_1!="x0"){
        if(funct3=="000") {
            operation+="lb"; 
            imm=hello.substr(0,12); 
            long long int z=binaryToDecimal(imm);
            int k=0;
            for(int i=0;i<(int)memory.size();i++){
                if(memory[i].first==val1){
                    int g=i,l=1;
                    regvals[rd_1]="";
                    if(z>0){
                        while(g<(int)memory.size() && z>0){
                            g=g+1;
                            z--;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g+1;
                                z--;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-1){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=0;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    //cout<<"Cache Miss: Index not found"<<endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    } 
                    else{
                        while(g<(int)memory.size() && z<0){
                            g=g-1;
                            z++;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g-1;
                                z++;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-1){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=0;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    //cout<<"Cache Miss: Index not found"<<endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    }
                    if(regvals[rd_1].size()<16){
                        if(hex_binary(string(1, regvals[rd_1][0]))[0]=='0') {
                            while(regvals[rd_1].size()!=16) regvals[rd_1]="0"+regvals[rd_1];
                        }
                        else{
                            while(regvals[rd_1].size()!=16) regvals[rd_1]="f"+regvals[rd_1];
                        }
                    }
                    k=1;
                    break;
                }
            }
            if(k==0){
                regvals[rd_1]="0";
            }
        }
        if(funct3=="001") {
            operation+="lh"; 
            imm=hello.substr(0,12); 
            long long int z=binaryToDecimal(imm);
            int k=0;
            for(int i=0;i<(int)memory.size();i++){
                if(memory[i].first==val1){
                    int g=i,l=2;
                    regvals[rd_1]="";
                    if(z>0){
                        while(g<(int)memory.size() && z>0){
                            g=g+1;
                            z--;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g+1;
                                z--;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-2){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=2;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    //cout<<"Cache Miss: Index not found"<<endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        } 
                    }
                    else{
                        while(g<(int)memory.size() && z<0){
                            g=g-1;
                            z++;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g-1;
                                z++;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-2){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=2;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    //cout<<"Cache Miss: Index not found"<<endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    }
                    if(regvals[rd_1].size()<16){
                        if(hex_binary(string(1, regvals[rd_1][0]))[0]=='0') {
                            while(regvals[rd_1].size()!=16) regvals[rd_1]="0"+regvals[rd_1];
                        }
                        else{
                            while(regvals[rd_1].size()!=16) regvals[rd_1]="f"+regvals[rd_1];
                        }
                    }
                }
                k=1;
                break;
            }
            if(k==0){
                regvals[rd_1]="0";
            }
        }
        if(funct3=="010") {
            operation+="lw"; 
            imm=hello.substr(0,12); 
            long long int z=binaryToDecimal(imm);
            int k=0;
            for(int i=0;i<(int)memory.size();i++){
                if(memory[i].first==val1){
                    int g=i,l=4;
                    regvals[rd_1]="";
                    if(z>0){
                        while(g<(int)memory.size() && z>0){
                            g=g+1;
                            z--;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g+1;
                                z--;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-6){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=6;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    //cout<<"Cache Miss: Index not found"<<endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    } 
                    else{
                        while(g<(int)memory.size() && z<0){
                            g=g-1;
                            z++;
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-6){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=6;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    }
                    if(regvals[rd_1].size()<16){
                        if(hex_binary(string(1, regvals[rd_1][0]))[0]=='0') {
                            while(regvals[rd_1].size()!=16) regvals[rd_1]="0"+regvals[rd_1];
                        }
                        else{
                            while(regvals[rd_1].size()!=16) regvals[rd_1]="f"+regvals[rd_1];
                        }
                    }
                    k=1;
                    break;
                }
            }
            if(k==0){
                regvals[rd_1]="0";
            }
        }
        if(funct3=="011") {
            operation+="ld"; 
            imm=hello.substr(0,12); 
            long long int z=binaryToDecimal(imm);
            int k=0;
            for(int i=0;i<(int)memory.size();i++){
                if(memory[i].first==val1){
                    int g=i,l=8;
                    regvals[rd_1]="";
                    if(z>=0){
                        while(g<(int)memory.size() && z>0){
                            g=g+1;
                            z--;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g+1;
                                z--;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-8){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=14;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;
                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); 
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    } 
                    else{
                        while(g<(int)memory.size() && z<0){
                            g=g-1;
                            z++;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g-1;
                                z++;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-8){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=14;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;
                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); 
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    }
                    if(regvals[rd_1].size()<16){
                        if(hex_binary(string(1, regvals[rd_1][0]))[0]=='0') {
                            while(regvals[rd_1].size()!=16) regvals[rd_1]="0"+regvals[rd_1];
                        }
                        else{
                            while(regvals[rd_1].size()!=16) regvals[rd_1]="f"+regvals[rd_1];
                        }
                    }
                    k=1;
                    break;
                }
            }
            if(k==0){
                regvals[rd_1]="0";
            }
        }
        if(funct3=="100") {
            operation+="lbu"; 
            imm=hello.substr(0,12); 
            long long int z=binaryToDecimal(imm);
            int k=0;
            for(int i=0;i<(int)memory.size();i++){
                if(memory[i].first==val1){
                    int g=i,l=1;
                    regvals[rd_1]="";
                    if(z>0){
                        while(g<(int)memory.size() && z>0){
                            g=g+1;
                            z--;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g+1;
                                z--;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-1){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=0;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    //cout<<"Cache Miss: Index not found"<<endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    } 
                    else{
                        while(g<(int)memory.size() && z<0){
                            g=g-1;
                            z++;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g-1;
                                z++;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-1){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=0;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    //cout<<"Cache Miss: Index not found"<<endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    }
                    if(regvals[rd_1].size()<16){
                        while(regvals[rd_1].size()!=16) regvals[rd_1]="0"+regvals[rd_1];
                    }
                    k=1;
                    break;
                }
            }
            if(k==0){
                regvals[rd_1]="0";
            }
        }
        if(funct3=="101") {
            operation+="lhu"; 
            imm=hello.substr(0,12); 
            long long int z=binaryToDecimal(imm);
            int k=0;
            for(int i=0;i<(int)memory.size();i++){
                if(memory[i].first==val1){
                    int g=i,l=2;
                    regvals[rd_1]="";
                    if(z>0){
                        while(g<(int)memory.size() && z>0){
                            g=g+1;
                            z--;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g+1;
                                z--;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-2){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=2;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    //cout<<"Cache Miss: Index not found"<<endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    } 
                    else{
                        while(g<(int)memory.size() && z<0){
                            g=g-1;
                            z++;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g-1;
                                z++;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-2){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=2;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    //cout<<"Cache Miss: Index not found"<<endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    }
                    if(regvals[rd_1].size()<16){
                        while(regvals[rd_1].size()!=16) regvals[rd_1]="0"+regvals[rd_1];
                    }
                    k=1;
                    break;
                }
            }
            if(k==0){
                regvals[rd_1]="0";
            }

        }
        if(funct3=="110") {
            operation+="lwu"; 
            imm=hello.substr(0,12); 
            long long int z=binaryToDecimal(imm);
            int k=0;
            for(int i=0;i<(int)memory.size();i++){
                if(memory[i].first==val1){
                    int g=i,l=4;
                    regvals[rd_1]="";
                    if(z>0){
                        while(g<(int)memory.size() && z>0){
                            g=g+1;
                            z--;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g+1;
                                z--;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-6){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=6;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    //cout<<"Cache Miss: Index not found"<<endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    } 
                    else{
                        while(g<(int)memory.size() && z<0){
                            g=g-1;
                            z++;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g-1;
                                z++;
                            }
                        }
                        if(cache_on){
                            long long int a=hexToDecimal(memory[g].first);
                            if(a%block_sizes>block_sizes-6){
                                misses++;
                                cout<<"Error . Out of cache block range"<<endl;
                            }
                            else{
                                long long int a=hexToDecimal(memory[g].first);
                                int t=a%block_sizes;
                                int djwalababa=a-a%block_sizes;
                                string fu=hex_binary(decimalToHex(djwalababa));
                                int start=32-indexi-biteoffset; 
                                string index1=fu.substr(start+32, indexi); // Extract the index from the input
                                string tag=fu.substr(32, start); // Extract the tag from the input
                                if(index1.size()%4!=0){
                                    while(index1.size()%4!=0){
                                        index1="0"+index1;
                                    }
                                }
                                while(tag.size()%4!=0){
                                    tag="0"+tag;
                                }
                                auto it=direct_mapped_cache.find(index1);
                                if (it != direct_mapped_cache.end()) {
                                    bool found=false;
                                    for (int i=0; i<associate; i++) {
                                        if (it->second.valid[i] && it->second.tags[i]==tag){
                                            hits++;  // Cache hit
                                            if(it->second.dirty_bit[i]) otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                            else otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                            if(replacement_policy=="LRU"){
                                                for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                    if (it->first == index1) { 
                                                        auto& tags = it->second;
                                                        auto tagIt = find(tags.begin(), tags.end(), tag);

                                                        if (tagIt != tags.end()) { // If the tag is found
                                                            // Move the matched tag to the front of the vector
                                                            tags.erase(tagIt);           // Remove the tag from its current position
                                                            tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                            // No need to move the whole pair in the list now, only update the vector
                                                            break; // Exit after updating the vector
                                                        }
                                                    }
                                                }
                                            }
                                            if (replacement_policy == "LFU") {
                                                bool indexFound = false;

                                                // Step 1: Locate the index in `lfu`
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) {  // Match the index
                                                        bool tagFound = false;

                                                        // Step 2: Search for the tag within this index
                                                        for (auto &tagEntry : entry.second) {
                                                            if (tagEntry.second == tag) {  // Match the tag
                                                                tagEntry.first++;          // Increment the access count
                                                                tagFound = true;
                                                                break;
                                                            }
                                                        }
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first;
                                                        });

                                                        break;
                                                    }
                                                }
                                            }
                                            string temp=it->second.data[i].substr(2*(binaryToDecimal(imm)%block_sizes),2*l);
                                            string result="";
                                            int arnav=6;
                                            while(arnav>=0){
                                                result=result+temp.substr(arnav,2);
                                                arnav-=2;
                                            }
                                            temp=removeLeadingZeros(result);
                                            regvals[rd_1] = result;
                                            found=true;
                                            break;
                                        }
                                    }

                                    if (!found) {
                                        // Cache miss (valid entry, but no matching tag)
                                        misses++;
                                        
                                        otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                        //cout<<"Cache Miss: Tag mismatch"<<endl;

                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        
                                        int don=g;
                                        don=don-a%block_sizes;
                                        g-=1;
                                        if(g<(int)memory.size() && l!=0){
                                            while(g<(int)memory.size()-1 && l!=0){
                                                g=g+1;
                                                regvals[rd_1]=memory[g].second+regvals[rd_1];
                                                l--; 
                                            }
                                        }
                                        string res="";
                                        for(int i=0;i<block_sizes;i++){
                                            res=res+memory[don].second;
                                            don++;
                                        }
                                        if (replacement_policy == "FIFO") {
                                            if(replaceIndex!=0){
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto& entry : fifo) {
                                                    if (entry.first == index1) {
                                                        int replacei=0;
                                                        if (!entry.second.empty()) {
                                                            replacei = entry.second.front().second;
                                                            if(it->second.dirty_bit[replacei]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            entry.second.pop();  
                                                        }
                                                        entry.second.push(make_pair(tag,replacei));
                                                        it->second.tags[replacei]=tag;
                                                        it->second.data[replacei]=res; 
                                                        it->second.valid[replacei]=true;
                                                        it->second.dirty_bit[replacei]=false;
                                                        break;
                                                    }
                                                }

                                            }
                                        }
                                        else if(replacement_policy=="LRU"){
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                for (auto &entry : lru) {
                                                    if (entry.first == index1) { 
                                                        auto lastelem=entry.second.back();
                                                        entry.second.pop_back();
                                                        entry.second.insert(entry.second.begin(), tag);
                                                        auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
                
                                                        if (tagIt != it->second.tags.end()) {
                                                            size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                            if(it->second.dirty_bit[index]){
                                                                string b=decimalToHex(a+t);
                                                                int tempeg=a+t-65536;
                                                                int arnav=0;
                                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                                while(tempeg<(int)memory.size()-1 && l!=0){
                                                                    tempeg=tempeg+1;
                                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                                    l--; arnav+=2;
                                                                }
                                                            }
                                                            // Now update the corresponding tags, data, and valid fields
                                                            it->second.tags[index] = tag;
                                                            it->second.data[index] = res;
                                                            it->second.valid[index] = true;
                                                            it->second.dirty_bit[index]=false;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        else if (replacement_policy == "LFU") {
                                            if(replaceIndex!=0){
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        it->second.tags[replaceIndex]=tag;
                                                        it->second.data[replaceIndex]=res; 
                                                        it->second.valid[replaceIndex]=true;
                                                        break;
                                                    }
                                                }
                                            }
                                            else{
                                                bool indexFound = false;
                                                for (auto &entry : lfu) {
                                                    if (entry.first == index1) { 
                                                        auto &tagi = entry.second;
                                                        auto &lastent=tagi.back();
                                                        string replaci = lastent.second;
                                                        tagi.pop_back();
                                                        
                                                        indexFound = true;
                                                        entry.second.emplace_back(1, tag);
                                                        sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                            return a.first > b.first; 
                                                        });
                                                        auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                        *iti=tag;
                                                        int index = distance(it->second.tags.begin(), iti);
                                                        it->second.data[index] = res; 
                                                        it->second.valid[index] = true; 
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        if(replacement_policy=="RANDOM"){
                                            random_device rd;  // Initialize random device for seeding
                                            mt19937 gen(rd()); // Mersenne Twister engine
                                            uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                            if(replaceIndex!=0){
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                            }
                                            else{
                                                // Generate a random index between 0 and associate-1
                                                int replaceIndex = distrib(gen);
                                                if(it->second.dirty_bit[replaceIndex]){
                                                    string b=decimalToHex(a+t);
                                                    int tempeg=a+t-65536;
                                                    int arnav=0;
                                                    if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                    while(tempeg<(int)memory.size()-1 && l!=0){
                                                        tempeg=tempeg+1;
                                                        memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                        l--; arnav+=2;
                                                    }
                                                }
                                                // Now use the replaceIndex to update tags, data, and valid arrays
                                                it->second.tags[replaceIndex] = tag;
                                                it->second.data[replaceIndex] = res;
                                                it->second.valid[replaceIndex] = true;
                                                it->second.dirty_bit[replaceIndex]=false;
                                            }
                                        }
                                    }

                                } else {
                                    // Cache miss (index not found)
                                    misses++;
                                    
                                    otpfile << "R: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                    //cout<<"Cache Miss: Index not found"<<endl;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;
                                    if(g<(int)memory.size() && l!=0){
                                        while(g<(int)memory.size()-1 && l!=0){
                                            g=g+1;
                                            regvals[rd_1]=memory[g].second+regvals[rd_1];
                                            l--; 
                                        }
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        // Check if the queue exists for the given index
                                        bool found1 = false;
                                        for (auto& entry : fifo) {
                                            if (entry.first == index1) {
                                                entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                                found1 = true;
                                                break;
                                            }
                                        }
                                        
                                        // If the queue for index1 doesn't exist, create a new pair
                                        if (!found1) {
                                            queue<pair<string,int>> newQueue;
                                            newQueue.push(make_pair(tag,0));
                                            fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                        }
                                    }
                                    if(replacement_policy=="LRU"){
                                        vector<string> hi={tag};
                                        lru.emplace_back(index1,hi);
                                    }
                                    if (replacement_policy == "LFU") {
                                        lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                    }
                                    // Create a new entry in the cache with the specified associativity
                                    CacheEntry newEntry;
                                    newEntry.tags[0]=tag;
                                    newEntry.data[0]=res;  // Replace with the actual data
                                    newEntry.valid[0]=true;
                                    direct_mapped_cache[index1]=newEntry;
                                }
                            }
                        }
                        else{
                            g-=1;
                            if(g<(int)memory.size() && l!=0){
                                while(g<(int)memory.size()-1 && l!=0){
                                    g=g+1;
                                    regvals[rd_1]=memory[g].second+regvals[rd_1];
                                    l--; 
                                }
                            }
                        }
                    }
                    while(regvals[rd_1].size()!=16) regvals[rd_1]="0"+regvals[rd_1];
                    k=1;
                    break;
                }
            }
            if(k==0){
                regvals[rd_1]="0";
            }
        }
    }
    cout<<"Executed "<<trimTrailing(actual_lines[curr_line])<<"; pc=0x"<<pc.substr(8)<<endl;
    pc=addHex(pc,"4");
}

string B(string hello,string pc){
    string rs1=hello.substr(12,5);
    string rs2=hello.substr(7,5);
    string funct3=hello.substr(17,3);
    string rs1_1="x"+to_string(binaryToDecimalpos(rs1));
    string rs2_1="x"+to_string(binaryToDecimalpos(rs2));
    string val1=to_string(hex_dec(regvals[rs1_1]));
    string val2=to_string(hex_dec(regvals[rs2_1]));
    string operation="",imm="";
    imm+=hello[24];
    imm+=hello.substr(1,6);
    imm+=hello.substr(20,4);
    imm+="0";
    if(funct3=="000"){operation += "beq";if(stoll(val1) != stoll(val2)) imm="4";} 
    else if(funct3=="001"){operation += "bne";if(stoll(val1)==stoll(val2)) imm="4";} 
    else if(funct3=="100"){operation += "blt";if(stoll(val1) >= stoll(val2)) imm="4";} 
    else if(funct3=="101"){operation += "bge";if(stoll(val1)<stoll(val2)) imm="4";} 
    else if(funct3=="110"){operation += "bltu";if((unsigned)stoll(val1) >= (unsigned)stoll(val2))imm="4";} 
    else if(funct3=="111"){operation += "bgeu";if((unsigned)stoll(val1)<(unsigned)stoll(val2))imm="4";}
    cout<<"Executed "<<trimTrailing(actual_lines[curr_line])<< "; ";
    return imm;
}

int J(string hello,string &pc){
    string imm="";
    string operation="jal";
    string rd=hello.substr(20,5);
    string rd_1="x"+to_string(binaryToDecimalpos(rd));
    imm+=hello.substr(12,8);
    imm+=hello[12];
    imm+=hello.substr(1,10);
    imm+="0";
    regvals[rd_1]=addHex(pc,"4");
    string key=rd_1;
    if(return_adresses.find(key)!=return_adresses.end()){
        return_adresses[key].push_back(regvals[rd_1]);
        curr_ret[key]++;
    } 
    else{
        return_adresses[key].push_back(regvals[rd_1]);
        curr_ret[key]=0;
    }
    
    cout<<"Executed "<<trimTrailing(actual_lines[curr_line])<<"; ";
    // cout<<binaryToDecimal(imm)<<endl;
    return binaryToDecimal(imm);
}

void U(string hello,string &pc){
    string imm=hello.substr(0,20);
    string rd=hello.substr(20,5);
    string rd_1="x"+to_string(binaryToDecimalpos(rd));
    string operation="";
    operation+="lui";
    string new1=binary_hex(imm);
    new1=removeLeadingZeros(new1)+"000";
    if(rd_1!="x0") regvals[rd_1]=addHex(regvals[rd_1],new1);
    cout<<"Executed "<<trimTrailing(actual_lines[curr_line])<<"; pc=0x"<<pc.substr(8)<<endl;
    pc=addHex(pc,"4");
}

void S(string hello,string &pc,ofstream &otpfile){
    string imm1=hello.substr(0,7);
    string imm2=hello.substr(20,5);
    string imm=imm1+imm2;
    string rs1=hello.substr(12,5);
    string funct3=hello.substr(17,3);
    string rd=hello.substr(7,5);
    string rs1_1="x"+to_string(binaryToDecimalpos(rs1));
    string rd_1="x"+to_string(binaryToDecimalpos(rd));
    string val1=regvals[rs1_1];
    string operation="";
    if(rd_1!="x69"){
        if(funct3=="000"){
            operation+="sb";
            long long int z=binaryToDecimal(imm);
            int k=0,g=0;
            for(int i=0;i<(int)memory.size();i++){
                if(memory[i].first==val1){
                    int l=1;
                    g=i;
                    if(z>0){
                        while(g<(int)memory.size() && z>0){
                            g=g+1;
                            z--;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g+1;
                                z--;
                            }
                        }
                    }
                    else{
                        while(g<(int)memory.size() && z<0){
                            g=g-1;
                            z++;
                        }
                        if(z<0){
                            while(z!=0){
                                g=g-1;
                                z++;
                            }
                        }
                    }
                    if(cache_on){
                        long long int a=hexToDecimal(memory[g].first);
                        int t=a%block_sizes;
                        int djwalababa=a-a%block_sizes;
                        string fu=hex_binary(decimalToHex(djwalababa));
                        int start=32-indexi-biteoffset; 
                        string index1=fu.substr(start+32, indexi); // Extract the index from the input
                        string tag=fu.substr(32, start); // Extract the tag from the input
                        if(index1.size()%4!=0){
                            while(index1.size()%4!=0){
                                index1="0"+index1;
                            }
                        }
                        while(tag.size()%4!=0){
                            tag="0"+tag;
                        }
                        bool miss=true;
                        if(write_policy=="WT"){
                            auto it=direct_mapped_cache.find(index1);
                            if (it != direct_mapped_cache.end()) {
                                for (int i=0; i<associate; i++) {
                                    if (it->second.valid[i] && it->second.tags[i]==tag){
                                        hits++;
                                        otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;
                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        if(replacement_policy=="LRU"){
                                            for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                if (it->first == index1) { // Check if the entry matches the index
                                                    // Search within the vector of tags for a match
                                                    auto& tags = it->second;
                                                    auto tagIt = find(tags.begin(), tags.end(), tag);

                                                    if (tagIt != tags.end()) { // If the tag is found
                                                        // Move the matched tag to the front of the vector
                                                        tags.erase(tagIt);           // Remove the tag from its current position
                                                        tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                        // No need to move the whole pair in the list now, only update the vector
                                                        break; // Exit after updating the vector
                                                    }
                                                }
                                            }
                                        }
                                        miss=false;
                                        break;
                                    }
                                }  
                                if(miss){
                                    misses++;
                                    otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                }                                           
                            }
                            else{
                                misses++;
                                otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  

                            }
                            g-=1; int start=14;
                            if(regvals[rd_1].size()<16){
                            while(regvals[rd_1].size()!=16){
                                regvals[rd_1]="0"+regvals[rd_1];
                                }
                            }
                            if(g>=(int)memory.size()-1) {k=0;break;}
                            while(g<(int)memory.size()-1 && l!=0){
                                g=g+1;
                                memory[g].second=regvals[rd_1].substr(start,2);
                                l--; start-=2;
                            }
                            string mem_write1=memory[g].first;
                            while(l!=0){
                                mem_write1=addHex(mem_write1,"1");
                                memory.emplace_back(addHex(mem_write1,"0"),regvals[rd_1].substr(start,2));
                                l--; start-=2;
                            }
                            k=1;  
                        }
                        if(write_policy=="WB"){
                            int mine=l;
                            auto it=direct_mapped_cache.find(index1);
                            if(it != direct_mapped_cache.end()){
                                for (int i=0; i<associate; i++) {
                                    if (it->second.valid[i] && it->second.tags[i]==tag){
                                        hits++;
                                        otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                        int arnav=14;string residence="";
                                        while(arnav>=0){
                                            residence=residence+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                            arnav-=2;
                                        }
                                        it->second.data[i].replace(2*t, 2*mine, residence.substr(16 - 2*mine, 2*mine));
                                        if(it->second.dirty_bit[i]){
                                            string b=decimalToHex(a+t);
                                            int tempeg=a-65536;
                                            int arnav=0;
                                            tempeg-=1;
                                            int hihi=block_sizes;
                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                            while(tempeg<(int)memory.size()-1 && hihi!=0){
                                                tempeg=tempeg+1;
                                                memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                hihi--; arnav+=2;
                                            }
                                        }
                                        k=1;
                                        
                                        it->second.dirty_bit[i]=true;
                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        if(replacement_policy=="LRU"){
                                            for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                if (it->first == index1) { // Check if the entry matches the index
                                                    // Search within the vector of tags for a match
                                                    auto& tags = it->second;
                                                    auto tagIt = find(tags.begin(), tags.end(), tag);

                                                    if (tagIt != tags.end()) { // If the tag is found
                                                        // Move the matched tag to the front of the vector
                                                        tags.erase(tagIt);           // Remove the tag from its current position
                                                        tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                        // No need to move the whole pair in the list now, only update the vector
                                                        break; // Exit after updating the vector
                                                    }
                                                }
                                            }
                                        }
                                        miss=false;
                                        break;
                                        
                                    }
                                }
                                if(miss){
                                    otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;
                                    misses++;
                                    g-=1; start=14;
                                    if(regvals[rd_1].size()<16){
                                        while(regvals[rd_1].size()!=16){
                                            regvals[rd_1]="0"+regvals[rd_1];
                                        }
                                    }
                                    if(g>=(int)memory.size()-1) {k=0;break;}
                                    while(g<(int)memory.size()-1 && l!=0){
                                        g=g+1;
                                        l--; start-=2;
                                    }
                                    k=1;
                                    //Tag mismatch
                                    int replaceIndex=0;
                                    for (int i=0; i<associate; i++) {
                                        if (!it->second.valid[i]) {
                                            replaceIndex=i;
                                            break;
                                        }
                                    }
                                    int arnav=14;string residence="";
                                    while(arnav>=0){
                                        residence=residence+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                        arnav-=2;
                                    }
                                    miss=false;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;start=14;
                                    if(g>=(int)memory.size()-1) {k=0;break;}
                                    while(g<(int)memory.size()-1 && l!=0){
                                        g=g+1;
                                        memory[g].second=regvals[rd_1].substr(start,2);
                                        l--; start-=2;
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        if(replaceIndex!=0){
                                            for (auto& entry : fifo) {
                                                if (entry.first == index1) {
                                                    entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                    it->second.tags[replaceIndex]=tag;
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    it->second.dirty_bit[replaceIndex]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            for (auto& entry : fifo) {
                                                if (entry.first == index1) {
                                                    int replacei=0;
                                                    if (!entry.second.empty()) {
                                                        replacei = entry.second.front().second;
                                                        if(it->second.dirty_bit[replacei]){
                                                            string b=decimalToHex(a+t);
                                                            int tempeg=a+t-65536;
                                                            int arnav=0;
                                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                            int djman=mine;
                                                            while(tempeg<(int)memory.size()-1 && djman!=0){
                                                                tempeg=tempeg+1;
                                                                memory[tempeg].second=it->second.data[replacei].substr(arnav,2);
                                                                djman--; arnav+=2;
                                                            }
                                                        }
                                                        entry.second.pop();  
                                                    }
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    entry.second.push(make_pair(tag,replacei));
                                                    it->second.tags[replacei]=tag;
                                                    it->second.data[replacei]=res; 
                                                    it->second.valid[replacei]=true;
                                                    it->second.dirty_bit[replacei]=true;
                                                    break;
                                                }
                                            }

                                        }
                                    }
                                    else if(replacement_policy=="LRU"){
                                        if(replaceIndex!=0){
                                            bool indexFound = false;
                                            for (auto &entry : lru) {
                                                if (entry.first == index1 || entry.first=="") { 
                                                    indexFound = true;
                                                    entry.second.insert(entry.second.begin(), tag);
                                                    it->second.tags[replaceIndex]=tag;
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    it->second.dirty_bit[replaceIndex]=true;
                                                    it->second.dirty_bit[i]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            for (auto &entry : lru) {
                                                if (entry.first == index1 || entry.first.size()==0) { 
                                                    auto lastelem=entry.second.back();
                                                    entry.second.pop_back();
                                                    entry.second.insert(entry.second.begin(), tag);
                                                    auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
            
                                                    if (tagIt != it->second.tags.end()) {
                                                        size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                        if(it->second.dirty_bit[index]){
                                                            string b=decimalToHex(a+t);
                                                            int tempeg=a+t-65536;
                                                            int arnav=0;
                                                            tempeg-=1;
                                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                            int djman=mine;
                                                            while(tempeg<(int)memory.size()-1 && djman!=0){
                                                                tempeg=tempeg+1;
                                                                memory[tempeg].second=it->second.data[index].substr(arnav,2);
                                                                djman--; arnav+=2;
                                                            }
                                                        }
                                                        string result = "";
                                                        int arnav=14;
                                                        while(arnav>=0){
                                                            result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                            arnav-=2;
                                                        }
                                                        res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                        it->second.tags[index] = tag;
                                                        it->second.data[index] = res;
                                                        it->second.dirty_bit[index]=true;
                                                        it->second.valid[index] = true;
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    else if (replacement_policy == "LFU") {
                                        if(replaceIndex!=0){
                                            bool indexFound = false;
                                            for (auto &entry : lfu) {
                                                if (entry.first == index1) { 
                                                    indexFound = true;
                                                    entry.second.emplace_back(1, tag);
                                                    sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                        return a.first > b.first; 
                                                    });
                                                    it->second.tags[replaceIndex]=tag;
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            bool indexFound = false;
                                            for (auto &entry : lfu) {
                                                if (entry.first == index1) { 
                                                    auto &tagi = entry.second;
                                                    auto &lastent=tagi.back();
                                                    string replaci = lastent.second;
                                                    tagi.pop_back();
                                                    
                                                    indexFound = true;
                                                    entry.second.emplace_back(1, tag);
                                                    sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                        return a.first > b.first; 
                                                    });
                                                    auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                    *iti=tag;
                                                    int index = distance(it->second.tags.begin(), iti);
                                                    it->second.data[index] = res; 
                                                    it->second.valid[index] = true; 
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    if(replacement_policy=="RANDOM"){
                                        random_device rd;  // Initialize random device for seeding
                                        mt19937 gen(rd()); // Mersenne Twister engine
                                        uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                        if(replaceIndex!=0){
                                            it->second.tags[replaceIndex]=tag;
                                            string result = "";
                                            int arnav=14;
                                            while(arnav>=0){
                                                result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                arnav-=2;
                                            }
                                            res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                            it->second.data[replaceIndex]=res; 
                                            it->second.valid[replaceIndex]=true;
                                            it->second.dirty_bit[replaceIndex]=true;
                                            break;
                                        }
                                        else{
                                            // Generate a random index between 0 and associate-1
                                            int replaceIndex = distrib(gen);
                                            if(it->second.dirty_bit[replaceIndex]){
                                                string b=decimalToHex(a+t);
                                                int tempeg=a+t-65536;
                                                int arnav=0;
                                                int djman=mine;
                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                while(tempeg<(int)memory.size()-1 && djman!=0){
                                                    tempeg=tempeg+1;
                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                    djman--; arnav+=2;
                                                }
                                            }    
                                            string result = "";
                                            int arnav=14;
                                            while(arnav>=0){
                                                result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                arnav-=2;
                                            }
                                            res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                            // Now use the replaceIndex to update tags, data, and valid arrays
                                            it->second.tags[replaceIndex] = tag;
                                            it->second.data[replaceIndex] = res;
                                            it->second.valid[replaceIndex] = true;
                                        }
                                    }
                                }
                            }
                            else{
                                misses++;
                                otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;
                                int don=g;
                                don=don-a%block_sizes;
                                g-=1;
                                start=14;
                                if(regvals[rd_1].size()<16){
                                    while(regvals[rd_1].size()!=16){
                                        regvals[rd_1]="0"+regvals[rd_1];
                                    }
                                }
                                string res="";
                                for(int i=0;i<block_sizes;i++){
                                    res=res+memory[don].second;
                                    don++;
                                }
                                if (replacement_policy == "FIFO") {
                                    // Check if the queue exists for the given index
                                    bool found1 = false;
                                    for (auto& entry : fifo) {
                                        if (entry.first == index1) {
                                            entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                            found1 = true;
                                            break;
                                        }
                                    }
                                    
                                    // If the queue for index1 doesn't exist, create a new pair
                                    if (!found1) {
                                        queue<pair<string,int>> newQueue;
                                        newQueue.push(make_pair(tag,0));
                                        fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                    }
                                }
                                if(replacement_policy=="LRU"){
                                    vector<string> hi={tag};
                                    lru.emplace_back(index1,hi);
                                }
                                if (replacement_policy == "LFU") {
                                    lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                }
                                // Create a new entry in the cache with the specified associativity
                                
                                CacheEntry newEntry;
                                newEntry.tags[0]=tag;
                                newEntry.data[0]=res; 
                                newEntry.valid[0]=true;
                                string result = "";
                                int arnav=14;
                                while(arnav>=0){
                                    result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                    arnav-=2;
                                }
                                res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                direct_mapped_cache[index1]=newEntry;
                                newEntry.dirty_bit[0]=true;
                            }
                        }
                    }
                    else{
                          
                        g-=1; int start=14;
                        if(regvals[rd_1].size()<16){
                            while(regvals[rd_1].size()!=16){
                                regvals[rd_1]="0"+regvals[rd_1];
                            }
                        }
                        if(g>=(int)memory.size()-1) {k=0;break;}
                        while(g<(int)memory.size()-1 && l!=0){
                            g=g+1;
                            memory[g].second=regvals[rd_1].substr(start,2);
                            l--; start-=2;
                        }
                        string mem_write1=memory[g].first;
                        while(l!=0){
                            mem_write1=addHex(mem_write1,"1");
                            memory.emplace_back(addHex(mem_write1,"0"),regvals[rd_1].substr(start,2));
                            l--; start-=2;
                        }
                    }
                    k=1;
                    break;
                }
            }
            if(k==0){
                string mem_write=memory[g].first;
                while(memory[g].first!=addHex(val1,binary_hex(imm))){
                    g=g+1;
                    memory.emplace_back(addHex(mem_write,"0"),"00");
                    mem_write=addHex(mem_write,"1");
                }
                memory[g].second=regvals[rd_1];
            }
        }
        if(funct3=="001"){
            operation+="sh";
            long long int z=binaryToDecimal(imm);
            int k=0,l=1,g=0;
            for(int i=0;i<(int)memory.size();i++){
                if(memory[i].first==val1){
                    int l=2;
                    g=i;
                    if(z>0){
                        while(g<(int)memory.size() && z>0){
                            g=g+1;
                            z--;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g+1;
                                z--;
                            }
                        }

                    }
                    else{
                        while(g<(int)memory.size() && z<0){
                            g=g-1;
                            z++;
                        }
                        if(z<0){
                            while(z!=0){
                                g=g-1;
                                z++;
                            }
                        }
                    }
                    if(cache_on){
                        long long int a=hexToDecimal(memory[g].first);
                        int t=a%block_sizes;
                        int djwalababa=a-a%block_sizes;
                        string fu=hex_binary(decimalToHex(djwalababa));
                        int start=32-indexi-biteoffset; 
                        string index1=fu.substr(start+32, indexi); // Extract the index from the input
                        string tag=fu.substr(32, start); // Extract the tag from the input
                        if(index1.size()%4!=0){
                            while(index1.size()%4!=0){
                                index1="0"+index1;
                            }
                        }
                        while(tag.size()%4!=0){
                            tag="0"+tag;
                        }
                        bool miss=true;
                        if(write_policy=="WT"){
                            auto it=direct_mapped_cache.find(index1);
                            if (it != direct_mapped_cache.end()) {
                                for (int i=0; i<associate; i++) {
                                    if (it->second.valid[i] && it->second.tags[i]==tag){
                                        hits++;
                                        otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;
                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        if(replacement_policy=="LRU"){
                                            for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                if (it->first == index1) { // Check if the entry matches the index
                                                    // Search within the vector of tags for a match
                                                    auto& tags = it->second;
                                                    auto tagIt = find(tags.begin(), tags.end(), tag);

                                                    if (tagIt != tags.end()) { // If the tag is found
                                                        // Move the matched tag to the front of the vector
                                                        tags.erase(tagIt);           // Remove the tag from its current position
                                                        tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                        // No need to move the whole pair in the list now, only update the vector
                                                        break; // Exit after updating the vector
                                                    }
                                                }
                                            }
                                        }
                                        miss=false;
                                        break;
                                    }
                                }  
                                if(miss){
                                    misses++;
                                    otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                }                                           
                            }
                            else{
                                misses++;
                                otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  

                            }
                            g-=1; int start=14;
                            if(regvals[rd_1].size()<16){
                            while(regvals[rd_1].size()!=16){
                                regvals[rd_1]="0"+regvals[rd_1];
                                }
                            }
                            if(g>=(int)memory.size()-1) {k=0;break;}
                            while(g<(int)memory.size()-1 && l!=0){
                                g=g+1;
                                memory[g].second=regvals[rd_1].substr(start,2);
                                l--; start-=2;
                            }
                            string mem_write1=memory[g].first;
                            while(l!=0){
                                mem_write1=addHex(mem_write1,"1");
                                memory.emplace_back(addHex(mem_write1,"0"),regvals[rd_1].substr(start,2));
                                l--; start-=2;
                            }
                            k=1;  
                        }
                        if(write_policy=="WB"){
                            int mine=l;
                            auto it=direct_mapped_cache.find(index1);
                            if(it != direct_mapped_cache.end()){
                                for (int i=0; i<associate; i++) {
                                    if (it->second.valid[i] && it->second.tags[i]==tag){
                                        hits++;
                                        otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                        int arnav=14;string residence="";
                                        while(arnav>=0){
                                            residence=residence+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                            arnav-=2;
                                        }
                                        it->second.data[i].replace(2*t, 2*mine, residence.substr(16 - 2*mine, 2*mine));
                                        if(it->second.dirty_bit[i]){
                                            string b=decimalToHex(a+t);
                                            int tempeg=a-65536;
                                            int arnav=0;
                                            tempeg-=1;
                                            int hihi=block_sizes;
                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                            while(tempeg<(int)memory.size()-1 && hihi!=0){
                                                tempeg=tempeg+1;
                                                memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                hihi--; arnav+=2;
                                            }
                                        }
                                        k=1;
                                        
                                        it->second.dirty_bit[i]=true;
                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        if(replacement_policy=="LRU"){
                                            for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                if (it->first == index1) { // Check if the entry matches the index
                                                    // Search within the vector of tags for a match
                                                    auto& tags = it->second;
                                                    auto tagIt = find(tags.begin(), tags.end(), tag);

                                                    if (tagIt != tags.end()) { // If the tag is found
                                                        // Move the matched tag to the front of the vector
                                                        tags.erase(tagIt);           // Remove the tag from its current position
                                                        tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                        // No need to move the whole pair in the list now, only update the vector
                                                        break; // Exit after updating the vector
                                                    }
                                                }
                                            }
                                        }
                                        miss=false;
                                        break;
                                        
                                    }
                                }
                                if(miss){
                                    otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;
                                    misses++;
                                    g-=1; start=14;
                                    if(regvals[rd_1].size()<16){
                                        while(regvals[rd_1].size()!=16){
                                            regvals[rd_1]="0"+regvals[rd_1];
                                        }
                                    }
                                    if(g>=(int)memory.size()-1) {k=0;break;}
                                    while(g<(int)memory.size()-1 && l!=0){
                                        g=g+1;
                                        l--; start-=2;
                                    }
                                    k=1;
                                    //Tag mismatch
                                    int replaceIndex=0;
                                    for (int i=0; i<associate; i++) {
                                        if (!it->second.valid[i]) {
                                            replaceIndex=i;
                                            break;
                                        }
                                    }
                                    int arnav=14;string residence="";
                                    while(arnav>=0){
                                        residence=residence+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                        arnav-=2;
                                    }
                                    miss=false;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;start=14;
                                    if(g>=(int)memory.size()-1) {k=0;break;}
                                    while(g<(int)memory.size()-1 && l!=0){
                                        g=g+1;
                                        memory[g].second=regvals[rd_1].substr(start,2);
                                        l--; start-=2;
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        if(replaceIndex!=0){
                                            for (auto& entry : fifo) {
                                                if (entry.first == index1) {
                                                    entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                    it->second.tags[replaceIndex]=tag;
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    it->second.dirty_bit[replaceIndex]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            for (auto& entry : fifo) {
                                                if (entry.first == index1) {
                                                    int replacei=0;
                                                    if (!entry.second.empty()) {
                                                        replacei = entry.second.front().second;
                                                        if(it->second.dirty_bit[replacei]){
                                                            string b=decimalToHex(a+t);
                                                            int tempeg=a+t-65536;
                                                            int arnav=0;
                                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                            int djman=mine;
                                                            while(tempeg<(int)memory.size()-1 && djman!=0){
                                                                tempeg=tempeg+1;
                                                                memory[tempeg].second=it->second.data[replacei].substr(arnav,2);
                                                                djman--; arnav+=2;
                                                            }
                                                        }
                                                        entry.second.pop();  
                                                    }
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    entry.second.push(make_pair(tag,replacei));
                                                    it->second.tags[replacei]=tag;
                                                    it->second.data[replacei]=res; 
                                                    it->second.valid[replacei]=true;
                                                    it->second.dirty_bit[replacei]=true;
                                                    break;
                                                }
                                            }

                                        }
                                    }
                                    else if(replacement_policy=="LRU"){
                                        if(replaceIndex!=0){
                                            bool indexFound = false;
                                            for (auto &entry : lru) {
                                                if (entry.first == index1 || entry.first=="") { 
                                                    indexFound = true;
                                                    entry.second.insert(entry.second.begin(), tag);
                                                    it->second.tags[replaceIndex]=tag;
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    it->second.dirty_bit[replaceIndex]=true;
                                                    it->second.dirty_bit[i]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            for (auto &entry : lru) {
                                                if (entry.first == index1 || entry.first.size()==0) { 
                                                    auto lastelem=entry.second.back();
                                                    entry.second.pop_back();
                                                    entry.second.insert(entry.second.begin(), tag);
                                                    auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
            
                                                    if (tagIt != it->second.tags.end()) {
                                                        size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                        if(it->second.dirty_bit[index]){
                                                            string b=decimalToHex(a+t);
                                                            int tempeg=a+t-65536;
                                                            int arnav=0;
                                                            tempeg-=1;
                                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                            int djman=mine;
                                                            while(tempeg<(int)memory.size()-1 && djman!=0){
                                                                tempeg=tempeg+1;
                                                                memory[tempeg].second=it->second.data[index].substr(arnav,2);
                                                                djman--; arnav+=2;
                                                            }
                                                        }
                                                        string result = "";
                                                        int arnav=14;
                                                        while(arnav>=0){
                                                            result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                            arnav-=2;
                                                        }
                                                        res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                        it->second.tags[index] = tag;
                                                        it->second.data[index] = res;
                                                        it->second.dirty_bit[index]=true;
                                                        it->second.valid[index] = true;
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    else if (replacement_policy == "LFU") {
                                        if(replaceIndex!=0){
                                            bool indexFound = false;
                                            for (auto &entry : lfu) {
                                                if (entry.first == index1) { 
                                                    indexFound = true;
                                                    entry.second.emplace_back(1, tag);
                                                    sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                        return a.first > b.first; 
                                                    });
                                                    it->second.tags[replaceIndex]=tag;
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            bool indexFound = false;
                                            for (auto &entry : lfu) {
                                                if (entry.first == index1) { 
                                                    auto &tagi = entry.second;
                                                    auto &lastent=tagi.back();
                                                    string replaci = lastent.second;
                                                    tagi.pop_back();
                                                    
                                                    indexFound = true;
                                                    entry.second.emplace_back(1, tag);
                                                    sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                        return a.first > b.first; 
                                                    });
                                                    auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                    *iti=tag;
                                                    int index = distance(it->second.tags.begin(), iti);
                                                    it->second.data[index] = res; 
                                                    it->second.valid[index] = true; 
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    if(replacement_policy=="RANDOM"){
                                        random_device rd;  // Initialize random device for seeding
                                        mt19937 gen(rd()); // Mersenne Twister engine
                                        uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                        if(replaceIndex!=0){
                                            it->second.tags[replaceIndex]=tag;
                                            string result = "";
                                            int arnav=14;
                                            while(arnav>=0){
                                                result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                arnav-=2;
                                            }
                                            res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                            it->second.data[replaceIndex]=res; 
                                            it->second.valid[replaceIndex]=true;
                                            it->second.dirty_bit[replaceIndex]=true;
                                            break;
                                        }
                                        else{
                                            // Generate a random index between 0 and associate-1
                                            int replaceIndex = distrib(gen);
                                            if(it->second.dirty_bit[replaceIndex]){
                                                string b=decimalToHex(a+t);
                                                int tempeg=a+t-65536;
                                                int arnav=0;
                                                int djman=mine;
                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                while(tempeg<(int)memory.size()-1 && djman!=0){
                                                    tempeg=tempeg+1;
                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                    djman--; arnav+=2;
                                                }
                                            }    
                                            string result = "";
                                            int arnav=14;
                                            while(arnav>=0){
                                                result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                arnav-=2;
                                            }
                                            res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                            // Now use the replaceIndex to update tags, data, and valid arrays
                                            it->second.tags[replaceIndex] = tag;
                                            it->second.data[replaceIndex] = res;
                                            it->second.valid[replaceIndex] = true;
                                        }
                                    }
                                }
                            }
                            else{
                                misses++;
                                otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;
                                int don=g;
                                don=don-a%block_sizes;
                                g-=1;
                                start=14;
                                if(regvals[rd_1].size()<16){
                                    while(regvals[rd_1].size()!=16){
                                        regvals[rd_1]="0"+regvals[rd_1];
                                    }
                                }
                                string res="";
                                for(int i=0;i<block_sizes;i++){
                                    res=res+memory[don].second;
                                    don++;
                                }
                                if (replacement_policy == "FIFO") {
                                    // Check if the queue exists for the given index
                                    bool found1 = false;
                                    for (auto& entry : fifo) {
                                        if (entry.first == index1) {
                                            entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                            found1 = true;
                                            break;
                                        }
                                    }
                                    
                                    // If the queue for index1 doesn't exist, create a new pair
                                    if (!found1) {
                                        queue<pair<string,int>> newQueue;
                                        newQueue.push(make_pair(tag,0));
                                        fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                    }
                                }
                                if(replacement_policy=="LRU"){
                                    vector<string> hi={tag};
                                    lru.emplace_back(index1,hi);
                                }
                                if (replacement_policy == "LFU") {
                                    lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                }
                                // Create a new entry in the cache with the specified associativity
                                
                                CacheEntry newEntry;
                                newEntry.tags[0]=tag;
                                newEntry.data[0]=res; 
                                newEntry.valid[0]=true;
                                string result = "";
                                int arnav=14;
                                while(arnav>=0){
                                    result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                    arnav-=2;
                                }
                                res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                direct_mapped_cache[index1]=newEntry;
                                newEntry.dirty_bit[0]=true;
                            }
                        }
                    }
                    else{
                          
                        g-=1; int start=14;
                        if(regvals[rd_1].size()<16){
                            while(regvals[rd_1].size()!=16){
                                regvals[rd_1]="0"+regvals[rd_1];
                            }
                        }
                        if(g>=(int)memory.size()-1) {k=0;break;}
                        while(g<(int)memory.size()-1 && l!=0){
                            g=g+1;
                            memory[g].second=regvals[rd_1].substr(start,2);
                            l--; start-=2;
                        }
                        string mem_write1=memory[g].first;
                        while(l!=0){
                            mem_write1=addHex(mem_write1,"1");
                            memory.emplace_back(addHex(mem_write1,"0"),regvals[rd_1].substr(start,2));
                            l--; start-=2;
                        }
                    }
                    k=1;
                    break;
                }
            }
            if(k==0){
                string mem_write=memory[g].first;
                while(memory[g].first!=addHex(val1,binary_hex(imm))){
                    g=g+1;
                    memory.emplace_back(addHex(mem_write,"0"),"00");
                    mem_write=addHex(mem_write,"1");
                } l=2;
                int start=14;
                while(l!=0){
                    memory[g].second=regvals[rd_1].substr(start,2);
                    g+=1; l--; start-=2;
                }
            }
        }
        if(funct3=="010"){
            operation+="sw";
            long long int z=binaryToDecimal(imm);
            int k=0,l=1,g=0;
            for(size_t i=0;i<memory.size();i++){
                if(memory[i].first==val1){
                    int l=4;
                    g=i;
                    if(z>0){
                        while(g<(int)memory.size() && z>0){
                            g=g+1;
                            z--;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g+1;
                                z--;
                            }
                        }
                    }
                    else{
                        while(g<(int)memory.size() && z<0){
                            g=g-1;
                            z++;
                        }
                        if(z<0){
                            while(z!=0){
                                g=g-1;
                                z++;
                            }
                        }
                    }
                    if(cache_on){
                        long long int a=hexToDecimal(memory[g].first);
                        int t=a%block_sizes;
                        int djwalababa=a-a%block_sizes;
                        string fu=hex_binary(decimalToHex(djwalababa));
                        int start=32-indexi-biteoffset; 
                        string index1=fu.substr(start+32, indexi); // Extract the index from the input
                        string tag=fu.substr(32, start); // Extract the tag from the input
                        if(index1.size()%4!=0){
                            while(index1.size()%4!=0){
                                index1="0"+index1;
                            }
                        }
                        while(tag.size()%4!=0){
                            tag="0"+tag;
                        }
                        bool miss=true;
                        if(write_policy=="WT"){
                            auto it=direct_mapped_cache.find(index1);
                            if (it != direct_mapped_cache.end()) {
                                for (int i=0; i<associate; i++) {
                                    if (it->second.valid[i] && it->second.tags[i]==tag){
                                        hits++;
                                        otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;
                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        if(replacement_policy=="LRU"){
                                            for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                if (it->first == index1) { // Check if the entry matches the index
                                                    // Search within the vector of tags for a match
                                                    auto& tags = it->second;
                                                    auto tagIt = find(tags.begin(), tags.end(), tag);

                                                    if (tagIt != tags.end()) { // If the tag is found
                                                        // Move the matched tag to the front of the vector
                                                        tags.erase(tagIt);           // Remove the tag from its current position
                                                        tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                        // No need to move the whole pair in the list now, only update the vector
                                                        break; // Exit after updating the vector
                                                    }
                                                }
                                            }
                                        }
                                        miss=false;
                                        break;
                                    }
                                }  
                                if(miss){
                                    misses++;
                                    otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                }                                           
                            }
                            else{
                                misses++;
                                otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  

                            }
                            g-=1; int start=14;
                            if(regvals[rd_1].size()<16){
                            while(regvals[rd_1].size()!=16){
                                regvals[rd_1]="0"+regvals[rd_1];
                                }
                            }
                            if(g>=(int)memory.size()-1) {k=0;break;}
                            while(g<(int)memory.size()-1 && l!=0){
                                g=g+1;
                                memory[g].second=regvals[rd_1].substr(start,2);
                                l--; start-=2;
                            }
                            string mem_write1=memory[g].first;
                            while(l!=0){
                                mem_write1=addHex(mem_write1,"1");
                                memory.emplace_back(addHex(mem_write1,"0"),regvals[rd_1].substr(start,2));
                                l--; start-=2;
                            }
                            k=1;  
                        }
                        if(write_policy=="WB"){
                            int mine=l;
                            auto it=direct_mapped_cache.find(index1);
                            if(it != direct_mapped_cache.end()){
                                for (int i=0; i<associate; i++) {
                                    if (it->second.valid[i] && it->second.tags[i]==tag){
                                        hits++;
                                        otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                        int arnav=14;string residence="";
                                        while(arnav>=0){
                                            residence=residence+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                            arnav-=2;
                                        }
                                        it->second.data[i].replace(2*t, 2*mine, residence.substr(16 - 2*mine, 2*mine));
                                        if(it->second.dirty_bit[i]){
                                            string b=decimalToHex(a+t);
                                            int tempeg=a-65536;
                                            int arnav=0;
                                            tempeg-=1;
                                            int hihi=block_sizes;
                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                            while(tempeg<(int)memory.size()-1 && hihi!=0){
                                                tempeg=tempeg+1;
                                                memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                hihi--; arnav+=2;
                                            }
                                        }
                                        k=1;
                                        
                                        it->second.dirty_bit[i]=true;
                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        if(replacement_policy=="LRU"){
                                            for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                if (it->first == index1) { // Check if the entry matches the index
                                                    // Search within the vector of tags for a match
                                                    auto& tags = it->second;
                                                    auto tagIt = find(tags.begin(), tags.end(), tag);

                                                    if (tagIt != tags.end()) { // If the tag is found
                                                        // Move the matched tag to the front of the vector
                                                        tags.erase(tagIt);           // Remove the tag from its current position
                                                        tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                        // No need to move the whole pair in the list now, only update the vector
                                                        break; // Exit after updating the vector
                                                    }
                                                }
                                            }
                                        }
                                        miss=false;
                                        break;
                                        
                                    }
                                }
                                if(miss){
                                    otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;
                                    misses++;
                                    g-=1; start=14;
                                    if(regvals[rd_1].size()<16){
                                        while(regvals[rd_1].size()!=16){
                                            regvals[rd_1]="0"+regvals[rd_1];
                                        }
                                    }
                                    if(g>=(int)memory.size()-1) {k=0;break;}
                                    while(g<(int)memory.size()-1 && l!=0){
                                        g=g+1;
                                        l--; start-=2;
                                    }
                                    k=1;
                                    //Tag mismatch
                                    int replaceIndex=0;
                                    for (int i=0; i<associate; i++) {
                                        if (!it->second.valid[i]) {
                                            replaceIndex=i;
                                            break;
                                        }
                                    }
                                    int arnav=14;string residence="";
                                    while(arnav>=0){
                                        residence=residence+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                        arnav-=2;
                                    }
                                    miss=false;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;start=14;
                                    if(g>=(int)memory.size()-1) {k=0;break;}
                                    while(g<(int)memory.size()-1 && l!=0){
                                        g=g+1;
                                        memory[g].second=regvals[rd_1].substr(start,2);
                                        l--; start-=2;
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        if(replaceIndex!=0){
                                            for (auto& entry : fifo) {
                                                if (entry.first == index1) {
                                                    entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                    it->second.tags[replaceIndex]=tag;
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    it->second.dirty_bit[replaceIndex]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            for (auto& entry : fifo) {
                                                if (entry.first == index1) {
                                                    int replacei=0;
                                                    if (!entry.second.empty()) {
                                                        replacei = entry.second.front().second;
                                                        if(it->second.dirty_bit[replacei]){
                                                            string b=decimalToHex(a+t);
                                                            int tempeg=a+t-65536;
                                                            int arnav=0;
                                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                            int djman=mine;
                                                            while(tempeg<(int)memory.size()-1 && djman!=0){
                                                                tempeg=tempeg+1;
                                                                memory[tempeg].second=it->second.data[replacei].substr(arnav,2);
                                                                djman--; arnav+=2;
                                                            }
                                                        }
                                                        entry.second.pop();  
                                                    }
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    entry.second.push(make_pair(tag,replacei));
                                                    it->second.tags[replacei]=tag;
                                                    it->second.data[replacei]=res; 
                                                    it->second.valid[replacei]=true;
                                                    it->second.dirty_bit[replacei]=true;
                                                    break;
                                                }
                                            }

                                        }
                                    }
                                    else if(replacement_policy=="LRU"){
                                        if(replaceIndex!=0){
                                            bool indexFound = false;
                                            for (auto &entry : lru) {
                                                if (entry.first == index1 || entry.first=="") { 
                                                    indexFound = true;
                                                    entry.second.insert(entry.second.begin(), tag);
                                                    it->second.tags[replaceIndex]=tag;
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    it->second.dirty_bit[replaceIndex]=true;
                                                    it->second.dirty_bit[i]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            for (auto &entry : lru) {
                                                if (entry.first == index1 || entry.first.size()==0) { 
                                                    auto lastelem=entry.second.back();
                                                    entry.second.pop_back();
                                                    entry.second.insert(entry.second.begin(), tag);
                                                    auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
            
                                                    if (tagIt != it->second.tags.end()) {
                                                        size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                        if(it->second.dirty_bit[index]){
                                                            string b=decimalToHex(a+t);
                                                            int tempeg=a+t-65536;
                                                            int arnav=0;
                                                            tempeg-=1;
                                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                            int djman=mine;
                                                            while(tempeg<(int)memory.size()-1 && djman!=0){
                                                                tempeg=tempeg+1;
                                                                memory[tempeg].second=it->second.data[index].substr(arnav,2);
                                                                djman--; arnav+=2;
                                                            }
                                                        }
                                                        string result = "";
                                                        int arnav=14;
                                                        while(arnav>=0){
                                                            result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                            arnav-=2;
                                                        }
                                                        res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                        it->second.tags[index] = tag;
                                                        it->second.data[index] = res;
                                                        it->second.dirty_bit[index]=true;
                                                        it->second.valid[index] = true;
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    else if (replacement_policy == "LFU") {
                                        if(replaceIndex!=0){
                                            bool indexFound = false;
                                            for (auto &entry : lfu) {
                                                if (entry.first == index1) { 
                                                    indexFound = true;
                                                    entry.second.emplace_back(1, tag);
                                                    sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                        return a.first > b.first; 
                                                    });
                                                    it->second.tags[replaceIndex]=tag;
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            bool indexFound = false;
                                            for (auto &entry : lfu) {
                                                if (entry.first == index1) { 
                                                    auto &tagi = entry.second;
                                                    auto &lastent=tagi.back();
                                                    string replaci = lastent.second;
                                                    tagi.pop_back();
                                                    
                                                    indexFound = true;
                                                    entry.second.emplace_back(1, tag);
                                                    sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                        return a.first > b.first; 
                                                    });
                                                    auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                    *iti=tag;
                                                    int index = distance(it->second.tags.begin(), iti);
                                                    it->second.data[index] = res; 
                                                    it->second.valid[index] = true; 
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    if(replacement_policy=="RANDOM"){
                                        random_device rd;  // Initialize random device for seeding
                                        mt19937 gen(rd()); // Mersenne Twister engine
                                        uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                        if(replaceIndex!=0){
                                            it->second.tags[replaceIndex]=tag;
                                            string result = "";
                                            int arnav=14;
                                            while(arnav>=0){
                                                result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                arnav-=2;
                                            }
                                            res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                            it->second.data[replaceIndex]=res; 
                                            it->second.valid[replaceIndex]=true;
                                            it->second.dirty_bit[replaceIndex]=true;
                                            break;
                                        }
                                        else{
                                            // Generate a random index between 0 and associate-1
                                            int replaceIndex = distrib(gen);
                                            if(it->second.dirty_bit[replaceIndex]){
                                                string b=decimalToHex(a+t);
                                                int tempeg=a+t-65536;
                                                int arnav=0;
                                                int djman=mine;
                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                while(tempeg<(int)memory.size()-1 && djman!=0){
                                                    tempeg=tempeg+1;
                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                    djman--; arnav+=2;
                                                }
                                            }    
                                            string result = "";
                                            int arnav=14;
                                            while(arnav>=0){
                                                result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                arnav-=2;
                                            }
                                            res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                            // Now use the replaceIndex to update tags, data, and valid arrays
                                            it->second.tags[replaceIndex] = tag;
                                            it->second.data[replaceIndex] = res;
                                            it->second.valid[replaceIndex] = true;
                                        }
                                    }
                                }
                            }
                            else{
                                misses++;
                                otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;
                                int don=g;
                                don=don-a%block_sizes;
                                g-=1;
                                start=14;
                                if(regvals[rd_1].size()<16){
                                    while(regvals[rd_1].size()!=16){
                                        regvals[rd_1]="0"+regvals[rd_1];
                                    }
                                }
                                string res="";
                                for(int i=0;i<block_sizes;i++){
                                    res=res+memory[don].second;
                                    don++;
                                }
                                if (replacement_policy == "FIFO") {
                                    // Check if the queue exists for the given index
                                    bool found1 = false;
                                    for (auto& entry : fifo) {
                                        if (entry.first == index1) {
                                            entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                            found1 = true;
                                            break;
                                        }
                                    }
                                    
                                    // If the queue for index1 doesn't exist, create a new pair
                                    if (!found1) {
                                        queue<pair<string,int>> newQueue;
                                        newQueue.push(make_pair(tag,0));
                                        fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                    }
                                }
                                if(replacement_policy=="LRU"){
                                    vector<string> hi={tag};
                                    lru.emplace_back(index1,hi);
                                }
                                if (replacement_policy == "LFU") {
                                    lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                }
                                // Create a new entry in the cache with the specified associativity
                                
                                CacheEntry newEntry;
                                newEntry.tags[0]=tag;
                                newEntry.data[0]=res; 
                                newEntry.valid[0]=true;
                                string result = "";
                                int arnav=14;
                                while(arnav>=0){
                                    result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                    arnav-=2;
                                }
                                res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                direct_mapped_cache[index1]=newEntry;
                                newEntry.dirty_bit[0]=true;
                            }
                        }
                    }
                    else{
                          
                        g-=1; int start=14;
                        if(regvals[rd_1].size()<16){
                            while(regvals[rd_1].size()!=16){
                                regvals[rd_1]="0"+regvals[rd_1];
                            }
                        }
                        if(g>=(int)memory.size()-1) {k=0;break;}
                        while(g<(int)memory.size()-1 && l!=0){
                            g=g+1;
                            memory[g].second=regvals[rd_1].substr(start,2);
                            l--; start-=2;
                        }
                        string mem_write1=memory[g].first;
                        while(l!=0){
                            mem_write1=addHex(mem_write1,"1");
                            memory.emplace_back(addHex(mem_write1,"0"),regvals[rd_1].substr(start,2));
                            l--; start-=2;
                        }
                    }
                    k=1;
                    break;
                }
            }
            if(k==0){
                string mem_write=memory[g].first;
                while(memory[g].first!=addHex(val1,binary_hex(imm))){
                    g=g+1;
                    memory.emplace_back(addHex(mem_write,"0"),"00");
                    mem_write=addHex(mem_write,"1");
                }
                l=4;
                int start=14;
                while(l!=0){
                    memory[g].second=regvals[rd_1].substr(start,2);
                    g+=1; l--; start-=2;
                }
            }
        }
        if(funct3=="011"){
            operation+="sd";
            long long int z=binaryToDecimal(imm);
            int k=0,l=1,g=0;
            for(size_t i=0;i<memory.size();i++){
                if(memory[i].first==val1){
                    int l=8;
                    g=i;
                    if(z>0){
                        while(g<(int)memory.size() && z>0){
                            g=g+1;
                            z--;
                        }
                        if(z>0){
                            while(z!=0){
                                g=g+1;
                                z--;
                            }
                        }
                    }
                    else{
                        while(g<(int)memory.size() && z<0){
                            g=g-1;
                            z++;
                        }
                        if(z<0){
                            while(z!=0){
                                g=g-1;
                                z++;
                            }
                        }
                    }
                    if(cache_on){
                        long long int a=hexToDecimal(memory[g].first);
                        int t=a%block_sizes;
                        int djwalababa=a-a%block_sizes;
                        string fu=hex_binary(decimalToHex(djwalababa));
                        int start=32-indexi-biteoffset; 
                        string index1=fu.substr(start+32, indexi); // Extract the index from the input
                        string tag=fu.substr(32, start); // Extract the tag from the input
                        if(index1.size()%4!=0){
                            while(index1.size()%4!=0){
                                index1="0"+index1;
                            }
                        }
                        while(tag.size()%4!=0){
                            tag="0"+tag;
                        }
                        bool miss=true;
                        if(write_policy=="WT"){
                            auto it=direct_mapped_cache.find(index1);
                            if (it != direct_mapped_cache.end()) {
                                for (int i=0; i<associate; i++) {
                                    if (it->second.valid[i] && it->second.tags[i]==tag){
                                        hits++;
                                        otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;
                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        if(replacement_policy=="LRU"){
                                            for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                if (it->first == index1) { // Check if the entry matches the index
                                                    // Search within the vector of tags for a match
                                                    auto& tags = it->second;
                                                    auto tagIt = find(tags.begin(), tags.end(), tag);

                                                    if (tagIt != tags.end()) { // If the tag is found
                                                        // Move the matched tag to the front of the vector
                                                        tags.erase(tagIt);           // Remove the tag from its current position
                                                        tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                        // No need to move the whole pair in the list now, only update the vector
                                                        break; // Exit after updating the vector
                                                    }
                                                }
                                            }
                                        }
                                        miss=false;
                                        break;
                                    }
                                }  
                                if(miss){
                                    misses++;
                                    otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  
                                }                                           
                            }
                            else{
                                misses++;
                                otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Clean" << endl;  

                            }
                            g-=1; int start=14;
                            if(regvals[rd_1].size()<16){
                            while(regvals[rd_1].size()!=16){
                                regvals[rd_1]="0"+regvals[rd_1];
                                }
                            }
                            if(g>=(int)memory.size()-1) {k=0;break;}
                            while(g<(int)memory.size()-1 && l!=0){
                                g=g+1;
                                memory[g].second=regvals[rd_1].substr(start,2);
                                l--; start-=2;
                            }
                            string mem_write1=memory[g].first;
                            while(l!=0){
                                mem_write1=addHex(mem_write1,"1");
                                memory.emplace_back(addHex(mem_write1,"0"),regvals[rd_1].substr(start,2));
                                l--; start-=2;
                            }
                            k=1;  
                        }
                        if(write_policy=="WB"){
                            int mine=l;
                            auto it=direct_mapped_cache.find(index1);
                            if(it != direct_mapped_cache.end()){
                                for (int i=0; i<associate; i++) {
                                    if (it->second.valid[i] && it->second.tags[i]==tag){
                                        hits++;
                                        otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Hit, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;  
                                        int arnav=14;string residence="";
                                        while(arnav>=0){
                                            residence=residence+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                            arnav-=2;
                                        }
                                        it->second.data[i].replace(2*t, 2*mine, residence.substr(16 - 2*mine, 2*mine));
                                        if(it->second.dirty_bit[i]){
                                            string b=decimalToHex(a+t);
                                            int tempeg=a-65536;
                                            int arnav=0;
                                            tempeg-=1;
                                            int hihi=block_sizes;
                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                            while(tempeg<(int)memory.size()-1 && hihi!=0){
                                                tempeg=tempeg+1;
                                                memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                hihi--; arnav+=2;
                                            }
                                        }
                                        k=1;
                                        
                                        it->second.dirty_bit[i]=true;
                                        int replaceIndex=0;
                                        for (int i=0; i<associate; i++) {
                                            if (!it->second.valid[i]) {
                                                replaceIndex=i;
                                                break;
                                            }
                                        }
                                        if(replacement_policy=="LRU"){
                                            for (auto it = lru.begin(); it != lru.end(); ++it) {
                                                if (it->first == index1) { // Check if the entry matches the index
                                                    // Search within the vector of tags for a match
                                                    auto& tags = it->second;
                                                    auto tagIt = find(tags.begin(), tags.end(), tag);

                                                    if (tagIt != tags.end()) { // If the tag is found
                                                        // Move the matched tag to the front of the vector
                                                        tags.erase(tagIt);           // Remove the tag from its current position
                                                        tags.insert(tags.begin(), tag); // Insert it at the front of the vector

                                                        // No need to move the whole pair in the list now, only update the vector
                                                        break; // Exit after updating the vector
                                                    }
                                                }
                                            }
                                        }
                                        miss=false;
                                        break;
                                        
                                    }
                                }
                                if(miss){
                                    otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;
                                    misses++;
                                    g-=1; start=14;
                                    if(regvals[rd_1].size()<16){
                                        while(regvals[rd_1].size()!=16){
                                            regvals[rd_1]="0"+regvals[rd_1];
                                        }
                                    }
                                    if(g>=(int)memory.size()-1) {k=0;break;}
                                    while(g<(int)memory.size()-1 && l!=0){
                                        g=g+1;
                                        l--; start-=2;
                                    }
                                    k=1;
                                    //Tag mismatch
                                    int replaceIndex=0;
                                    for (int i=0; i<associate; i++) {
                                        if (!it->second.valid[i]) {
                                            replaceIndex=i;
                                            break;
                                        }
                                    }
                                    int arnav=14;string residence="";
                                    while(arnav>=0){
                                        residence=residence+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                        arnav-=2;
                                    }
                                    miss=false;
                                    int don=g;
                                    don=don-a%block_sizes;
                                    g-=1;start=14;
                                    if(g>=(int)memory.size()-1) {k=0;break;}
                                    while(g<(int)memory.size()-1 && l!=0){
                                        g=g+1;
                                        memory[g].second=regvals[rd_1].substr(start,2);
                                        l--; start-=2;
                                    }
                                    string res="";
                                    for(int i=0;i<block_sizes;i++){
                                        res=res+memory[don].second;
                                        don++;
                                    }
                                    if (replacement_policy == "FIFO") {
                                        if(replaceIndex!=0){
                                            for (auto& entry : fifo) {
                                                if (entry.first == index1) {
                                                    entry.second.push(make_pair(tag,replaceIndex));  // Push tag to the existing queue
                                                    it->second.tags[replaceIndex]=tag;
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    it->second.dirty_bit[replaceIndex]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            for (auto& entry : fifo) {
                                                if (entry.first == index1) {
                                                    int replacei=0;
                                                    if (!entry.second.empty()) {
                                                        replacei = entry.second.front().second;
                                                        if(it->second.dirty_bit[replacei]){
                                                            string b=decimalToHex(a+t);
                                                            int tempeg=a+t-65536;
                                                            int arnav=0;
                                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                            int djman=mine;
                                                            while(tempeg<(int)memory.size()-1 && djman!=0){
                                                                tempeg=tempeg+1;
                                                                memory[tempeg].second=it->second.data[replacei].substr(arnav,2);
                                                                djman--; arnav+=2;
                                                            }
                                                        }
                                                        entry.second.pop();  
                                                    }
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    entry.second.push(make_pair(tag,replacei));
                                                    it->second.tags[replacei]=tag;
                                                    it->second.data[replacei]=res; 
                                                    it->second.valid[replacei]=true;
                                                    it->second.dirty_bit[replacei]=true;
                                                    break;
                                                }
                                            }

                                        }
                                    }
                                    else if(replacement_policy=="LRU"){
                                        if(replaceIndex!=0){
                                            bool indexFound = false;
                                            for (auto &entry : lru) {
                                                if (entry.first == index1 || entry.first=="") { 
                                                    indexFound = true;
                                                    entry.second.insert(entry.second.begin(), tag);
                                                    it->second.tags[replaceIndex]=tag;
                                                    string result = "";
                                                    int arnav=14;
                                                    while(arnav>=0){
                                                        result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                        arnav-=2;
                                                    }
                                                    res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    it->second.dirty_bit[replaceIndex]=true;
                                                    it->second.dirty_bit[i]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            for (auto &entry : lru) {
                                                if (entry.first == index1 || entry.first.size()==0) { 
                                                    auto lastelem=entry.second.back();
                                                    entry.second.pop_back();
                                                    entry.second.insert(entry.second.begin(), tag);
                                                    auto tagIt = find(it->second.tags.begin(), it->second.tags.end(), lastelem);
            
                                                    if (tagIt != it->second.tags.end()) {
                                                        size_t index = distance(it->second.tags.begin(), tagIt); // Get the index of lastelem
                                                        if(it->second.dirty_bit[index]){
                                                            string b=decimalToHex(a+t);
                                                            int tempeg=a+t-65536;
                                                            int arnav=0;
                                                            tempeg-=1;
                                                            if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                            int djman=mine;
                                                            while(tempeg<(int)memory.size()-1 && djman!=0){
                                                                tempeg=tempeg+1;
                                                                memory[tempeg].second=it->second.data[index].substr(arnav,2);
                                                                djman--; arnav+=2;
                                                            }
                                                        }
                                                        string result = "";
                                                        int arnav=14;
                                                        while(arnav>=0){
                                                            result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                            arnav-=2;
                                                        }
                                                        res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                                        it->second.tags[index] = tag;
                                                        it->second.data[index] = res;
                                                        it->second.dirty_bit[index]=true;
                                                        it->second.valid[index] = true;
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    else if (replacement_policy == "LFU") {
                                        if(replaceIndex!=0){
                                            bool indexFound = false;
                                            for (auto &entry : lfu) {
                                                if (entry.first == index1) { 
                                                    indexFound = true;
                                                    entry.second.emplace_back(1, tag);
                                                    sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                        return a.first > b.first; 
                                                    });
                                                    it->second.tags[replaceIndex]=tag;
                                                    it->second.data[replaceIndex]=res; 
                                                    it->second.valid[replaceIndex]=true;
                                                    break;
                                                }
                                            }
                                        }
                                        else{
                                            bool indexFound = false;
                                            for (auto &entry : lfu) {
                                                if (entry.first == index1) { 
                                                    auto &tagi = entry.second;
                                                    auto &lastent=tagi.back();
                                                    string replaci = lastent.second;
                                                    tagi.pop_back();
                                                    
                                                    indexFound = true;
                                                    entry.second.emplace_back(1, tag);
                                                    sort(entry.second.begin(), entry.second.end(), [](const auto &a, const auto &b) {
                                                        return a.first > b.first; 
                                                    });
                                                    auto iti = find(it->second.tags.begin(), it->second.tags.end(), replaci);
                                                    *iti=tag;
                                                    int index = distance(it->second.tags.begin(), iti);
                                                    it->second.data[index] = res; 
                                                    it->second.valid[index] = true; 
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    if(replacement_policy=="RANDOM"){
                                        random_device rd;  // Initialize random device for seeding
                                        mt19937 gen(rd()); // Mersenne Twister engine
                                        uniform_int_distribution<> distrib(0, associate - 1); // Uniform distribution in range [0, associate-1]
                                        if(replaceIndex!=0){
                                            it->second.tags[replaceIndex]=tag;
                                            string result = "";
                                            int arnav=14;
                                            while(arnav>=0){
                                                result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                arnav-=2;
                                            }
                                            res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                            it->second.data[replaceIndex]=res; 
                                            it->second.valid[replaceIndex]=true;
                                            it->second.dirty_bit[replaceIndex]=true;
                                            break;
                                        }
                                        else{
                                            // Generate a random index between 0 and associate-1
                                            int replaceIndex = distrib(gen);
                                            if(it->second.dirty_bit[replaceIndex]){
                                                string b=decimalToHex(a+t);
                                                int tempeg=a+t-65536;
                                                int arnav=0;
                                                int djman=mine;
                                                if(tempeg>=(int)memory.size()-1) {k=0;break;}
                                                while(tempeg<(int)memory.size()-1 && djman!=0){
                                                    tempeg=tempeg+1;
                                                    memory[tempeg].second=it->second.data[i].substr(arnav,2);
                                                    djman--; arnav+=2;
                                                }
                                            }    
                                            string result = "";
                                            int arnav=14;
                                            while(arnav>=0){
                                                result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                                arnav-=2;
                                            }
                                            res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                            // Now use the replaceIndex to update tags, data, and valid arrays
                                            it->second.tags[replaceIndex] = tag;
                                            it->second.data[replaceIndex] = res;
                                            it->second.valid[replaceIndex] = true;
                                        }
                                    }
                                }
                            }
                            else{
                                misses++;
                                otpfile << "W: Address: 0x" << removeLeadingZeros(memory[g].first) << ", Set: 0x" << removeLeadingZeros(binary_hex(index1)) << ", Miss, Tag: 0x" << removeLeadingZeros(binary_hex(tag)) << ", Dirty" << endl;
                                int don=g;
                                don=don-a%block_sizes;
                                g-=1;
                                start=14;
                                if(regvals[rd_1].size()<16){
                                    while(regvals[rd_1].size()!=16){
                                        regvals[rd_1]="0"+regvals[rd_1];
                                    }
                                }
                                string res="";
                                for(int i=0;i<block_sizes;i++){
                                    res=res+memory[don].second;
                                    don++;
                                }
                                if (replacement_policy == "FIFO") {
                                    // Check if the queue exists for the given index
                                    bool found1 = false;
                                    for (auto& entry : fifo) {
                                        if (entry.first == index1) {
                                            entry.second.push(make_pair(tag,0));  // Push tag to the existing queue
                                            found1 = true;
                                            break;
                                        }
                                    }
                                    
                                    // If the queue for index1 doesn't exist, create a new pair
                                    if (!found1) {
                                        queue<pair<string,int>> newQueue;
                                        newQueue.push(make_pair(tag,0));
                                        fifo.emplace_back(index1, newQueue);  // Add new pair to the vector
                                    }
                                }
                                if(replacement_policy=="LRU"){
                                    vector<string> hi={tag};
                                    lru.emplace_back(index1,hi);
                                }
                                if (replacement_policy == "LFU") {
                                    lfu.emplace_back(index1, vector<pair<int, string>>{{1, tag}});
                                }
                                // Create a new entry in the cache with the specified associativity
                                
                                CacheEntry newEntry;
                                newEntry.tags[0]=tag;
                                newEntry.data[0]=res; 
                                newEntry.valid[0]=true;
                                string result = "";
                                int arnav=14;
                                while(arnav>=0){
                                    result=result+regvals[rd_1].substr(16 - 2 * mine, 2 * mine).substr(arnav,2);
                                    arnav-=2;
                                }
                                res.replace(2*(binaryToDecimal(imm)%block_sizes), 2 * mine, result);
                                direct_mapped_cache[index1]=newEntry;
                                newEntry.dirty_bit[0]=true;
                            }
                        }
                    }
                    else{
                          
                        g-=1; int start=14;
                        if(regvals[rd_1].size()<16){
                            while(regvals[rd_1].size()!=16){
                                regvals[rd_1]="0"+regvals[rd_1];
                            }
                        }
                        if(g>=(int)memory.size()-1) {k=0;break;}
                        while(g<(int)memory.size()-1 && l!=0){
                            g=g+1;
                            memory[g].second=regvals[rd_1].substr(start,2);
                            l--; start-=2;
                        }
                        string mem_write1=memory[g].first;
                        while(l!=0){
                            mem_write1=addHex(mem_write1,"1");
                            memory.emplace_back(addHex(mem_write1,"0"),regvals[rd_1].substr(start,2));
                            l--; start-=2;
                        }
                    }
                    k=1;
                    break;
                }
            }
            if(k==0){
                string mem_write=memory[g].first;
                while(memory[g].first!=addHex(val1,binary_hex(imm))){
                    g=g+1;
                    memory.emplace_back(addHex(mem_write,"0"),"00");
                    mem_write=addHex(mem_write,"1");
                }
                l=8;
                int start=14;
                while(l!=0){
                    memory[g].second=regvals[rd_1].substr(start,2);
                    g+=1; l--; start-=2;
                }  
            }
        }
    }
    cout<<"Executed "<<trimTrailing(actual_lines[curr_line])<<"; pc=0x"<<pc.substr(8)<<endl;
    pc=addHex(pc,"4");
}

int jumpandlinkreg(string hello,string &pc){
    string imm="",operation="jalr";
    string rs1=hello.substr(12,5);
    string rs1_1="x"+to_string(binaryToDecimalpos(rs1));
    imm+=hello.substr(0,11);
    string rd=hello.substr(20,5);
    string rd_1="x"+to_string(binaryToDecimalpos(rd));   
    if(rd_1!="x0"){
        rd_1=decimalToHex(curr_line+1);
    }              
    cout<<"Executed "<<trimTrailing(actual_lines[curr_line])<< "; ";
    string val1=regvals[rs1_1];
    long long line_no=subtractHex(val1,"0000000000000000");
    string key=rs1_1;
    return stoi(imm)/4+line_no/4;
}

void processFile(const string& inputFilename, const string& outputFilename);

int main() {
    string pc="0000000000000000";
    string command;
    string input="";
    string cacheinput="",cache_size="",block_size="",associativity="";
    string hexcode,ct;
    ifstream file,file1;
    vector<string> instruction_mem;
    vector<int> break_pt_line_no;
    int no_of_instructions=0,line_no;
    bool after_breakpt=false;
    int offset;
    bool is_jump=false,is_jalr=false;
    int curr_line_temp;
    vector<pair<string,int>> stackframe;
    int curr_call=0;
    int bs=0;
    while(command!="exit"){
        cin>>command;
        if(command=="load"){
            for (auto& element :regvals) {
                element.second="0";
            }
            no_of_instructions=0;
            instruction_mem.clear();
            stackframe.clear();
            return_adresses.clear();
            curr_ret.clear();
            break_pt_line_no.clear();
            labels.clear();
            actual_lines.clear();
            bs=0;
            curr_call=0;
            curr_line=0;
            hits=0;
            misses=0;
            cache_on=false;
            is_jump=false;
            is_jalr=false;
            after_breakpt=false;
            cin>>filename;
            file1.open(filename);
            while(getline(file1,input)){
                if(input[0]=='.'){
                    bs++;
                    continue;
                }
                actual_lines.push_back(input);
            }
            stackframe.emplace_back("main",0);
            for (size_t i=0; i<actual_lines.size(); i++) {
                size_t colon_pos=actual_lines[i].find(':'); 
                if (colon_pos != string::npos) {
                    size_t start_pos=actual_lines[i].find_first_not_of(" ", colon_pos + 1);
                    actual_lines[i]=(start_pos != string::npos) ? actual_lines[i].substr(start_pos) : ""; 
                }
            }
            file1.close();
            processFile(filename);
            file.open("output.hex");
            instruction_mem.clear();
            break_pt_line_no.clear(); 
            while(getline(file,input)){
                instruction_mem.push_back(input);
            }
            file.close();
            no_of_instructions=instruction_mem.size();
            curr_line=0;
            cout<<endl;
        }
        else if(command=="break"){
            if(curr_line==no_of_instructions){
                cout<<"Load a file.."<<endl<<endl;
                pc="0000000000000000";
            }
            else{
                if(break_pt_line_no.size()>=5){
                    cout<<"You can enter only 5 breakpoints "<<endl;
                    continue;
                }
                cin >> line_no;
                break_pt_line_no.push_back(line_no-1-counter);
                cout<<"Breakpoint set at line "<<line_no<<endl;
                cout<<endl;
            }
        }
        else if(command=="del"){
            if(curr_line==no_of_instructions){
                cout<<"Load a file.."<<endl<<endl;
                pc="0000000000000000";
            }
            else{
                string temps;
                cin >> temps;
                cin >> line_no;
                auto iterator=find(break_pt_line_no.begin(),break_pt_line_no.end(),line_no-1-counter);
                if(iterator==break_pt_line_no.end()){
                }
                else{
                    cout<<"breakpoint removed at line "<<line_no<<endl;
                    break_pt_line_no.erase(iterator);
                }
                cout<<endl;
            }
        }
        else if(command=="step"){
            size_t lastDotPos = filename.find_last_of('.');
            if (lastDotPos != string::npos && lastDotPos != 0) {
                filename = filename.substr(0, lastDotPos);  // Remove the extension
            }
            string outpfile = filename + ".output";

            ofstream otpfile(outpfile,ios::app);
            is_jalr=false;
            is_jump=false;
             if(curr_line==no_of_instructions){
                cout<<"Nothing to step"<<endl;
                pc="0000000000000000";
            }
            else{
                if(after_breakpt){
                after_breakpt=false;
                }
                else{
                    auto iterator=find(break_pt_line_no.begin(),break_pt_line_no.end(),curr_line);
                    if(iterator!=break_pt_line_no.end()){
                        cout<<"Execution stopped at breakpoint"<<endl;
                        cout<<endl;
                        after_breakpt=true;
                    }
                }
                if(after_breakpt==false){
                    string curr_inst=instruction_mem[curr_line];
                    string hello=hex_binary(curr_inst);
                    string opcode=hello.substr(25);
                    if(opcode=="0110011") R(hello,pc);
                    else if(opcode=="0010011") I(hello,pc);
                    else if(opcode=="0000011") Iload(hello,pc,otpfile);
                    else if(opcode=="0100011") S(hello,pc,otpfile);
                    else if(opcode=="1100011"){
                        string imm=B(hello,pc);
                        if(imm=="4"){
                            imm=hex_binary(imm);
                        }
                        cout<<"pc=0x"<<pc.substr(8)<<endl;
                        pc=addHex(pc,decimalToHex(binaryToDecimal(imm)));
                        curr_line+=binaryToDecimal(imm)/4-1;
                    }
                    else if(opcode=="1101111"){
                        is_jump=true;
                        offset=J(hello,pc);
                        cout<<"pc=0x"<<pc.substr(8)<<endl;
                        pc=addHex(pc,decimalToHex(offset));
                        string curr_label=labeloutput(curr_line+offset/4+1+counter);
                        // cout<<curr_label<<endl;
                        //stackframe[curr_call].second++;
                        if(curr_label!=stackframe[curr_call].first){
                            // stackframe[curr_call].second++;
                            stackframe.emplace_back(curr_label,stackframe[curr_call].second++);
                            curr_call++;
                            // stackframe[curr_call].second--;
                        }
                        else{
                            // stackframe[curr_call].second++;
                            stackframe.emplace_back(curr_label,stackframe[curr_call].second++);
                            curr_call++;
                            // stackframe[curr_call].second--;
                        }
                    // curr_line=curr_line-1+offset/4;                      
                    }
                    else if(opcode=="1100111"){
                        is_jalr=true;
                        curr_line_temp=jumpandlinkreg(hello,pc);
                        stackframe.pop_back();
                        curr_call--;
                        string rdd=get_rd(hello);
                        regvals[rdd]=decimalToHex((stackframe[curr_call].second)*4);
                        // stackframe[curr_call].second--;
                        cout<<"pc=0x"<<pc.substr(8)<<endl;
                    }
                    else if(opcode=="0110111") U(hello,pc);
                    curr_line++;
                    stackframe[curr_call].second=curr_line;
                    if(is_jump){
                        curr_line=curr_line+offset/4-1;
                    }
                    if(is_jalr){
                        curr_line=curr_line_temp;
                        stackframe[curr_call].second=curr_line;
                        pc=decimalToHex(curr_line*4);
                    }
                    if(curr_line==no_of_instructions){
                        pc="0000000000000000";
                        stackframe[curr_call].second=0;
                    }   
                }    
            }   
            cout<<endl;  
        }
        else if(command=="show-stack"){
            if(curr_line==no_of_instructions){
                cout<<"Empty Call Stack: Execution complete"<<endl<<endl;
                pc="0000000000000000";
            }
            else{
                cout<<"Call Stack: "<<endl;
                for(auto i:stackframe){
                    cout<<i.first<<":"<<i.second+bs<<endl;
                }
                cout<<endl;
            }
            
        }
        else if(command=="run"){
            size_t lastDotPos = filename.find_last_of('.');
            if (lastDotPos != string::npos && lastDotPos != 0) {
                filename = filename.substr(0, lastDotPos);  // Remove the extension
            }
            string outpfile = filename + ".output";
            ofstream otpfile(outpfile,ios::app);
            if(curr_line==no_of_instructions){
                cout<<"Reached end of file, nothing to run"<<endl<<endl;
                pc="0000000000000000";
                continue;
            }
            else if(after_breakpt){
                cout<<"Cannot run just after a breakpoint, need to step first"<<endl;
                cout<<endl;
            }
            else{
                if(after_breakpt){
                after_breakpt=false;
                }
                while(curr_line!=no_of_instructions){
                    auto iterator=find(break_pt_line_no.begin(),break_pt_line_no.end(),curr_line);
                    if(iterator!=break_pt_line_no.end()){
                        cout<<"Execution stopped at breakpoint"<<endl;
                        after_breakpt=true;
                        break;
                    }
                    is_jalr=false;
                    is_jump=false;
                    string curr_inst=instruction_mem[curr_line];
                    string hello=hex_binary(curr_inst);
                    string opcode=hello.substr(25);
                    if(opcode=="0110011") R(hello,pc);
                    else if(opcode=="0010011") I(hello,pc);
                    else if(opcode=="0000011") Iload(hello,pc,otpfile);
                    else if(opcode=="0100011") S(hello,pc,otpfile);
                    else if(opcode=="1100011"){
                        string imm=B(hello,pc);
                        if(imm=="4"){
                            imm=hex_binary(imm);
                        }
                        cout<<"pc=0x"<<pc.substr(8)<<endl;
                        pc=addHex(pc,decimalToHex(binaryToDecimal(imm)));
                        curr_line+=binaryToDecimal(imm)/4-1;
                    }
                    else if(opcode=="1101111"){
                        is_jump=true;
                        offset=J(hello,pc);
                        cout<<"pc=0x"<<pc.substr(8)<<endl;
                        pc=addHex(pc,decimalToHex(offset));
                        string curr_label=labeloutput(curr_line+offset/4+1+counter);
                        // cout<<curr_label<<endl;
                        //stackframe[curr_call].second++;
                        if(curr_label!=stackframe[curr_call].first){
                            // stackframe[curr_call].second++;
                            stackframe.emplace_back(curr_label,stackframe[curr_call].second++);
                            curr_call++;
                            // stackframe[curr_call].second--;
                        }
                        else{
                            // stackframe[curr_call].second++;
                            stackframe.emplace_back(curr_label,stackframe[curr_call].second++);
                            curr_call++;
                            // stackframe[curr_call].second--;
                        }
                    // curr_line=curr_line-1+offset/4;                      
                    }
                    else if(opcode=="1100111"){
                        is_jalr=true;
                        curr_line_temp=jumpandlinkreg(hello,pc);
                        stackframe.pop_back();
                        curr_call--;
                        string rdd=get_rd(hello);
                        regvals[rdd]=decimalToHex((stackframe[curr_call].second)*4);
                        // stackframe[curr_call].second--;
                        cout<<"pc=0x"<<pc.substr(8)<<endl;
                    }
                    else if(opcode=="0110111") U(hello,pc);
                    curr_line++;
                    stackframe[curr_call].second=curr_line;
                    if(is_jump){
                        curr_line=curr_line+offset/4-1;
                    }
                    if(is_jalr){
                        curr_line=curr_line_temp;
                        stackframe[curr_call].second=curr_line;
                        pc=decimalToHex(curr_line*4);
                    }
                    if(curr_line==no_of_instructions){
                        pc="0000000000000000";
                        stackframe[curr_call].second=0;
                    }   
                }                              
            } 
            if(cache_on){
                if(hits+misses==0) cout<<endl;
                else {float percent = (float)hits/(float)(hits+misses);
                cout << "D-Cache statistics: Accesses=" <<hits+misses<<", Hit=" << hits << ", Miss=" << misses << ", Hit Rate=" << percent << endl; }  
            }
            else{
                cout<<"Cache simulator is disabled"<<endl;
            }
            cout<<endl;   
            // cout<<djwala<<endl;
        }
        else if(command=="regs"){
            cout<<"registers:"<<endl;
            for(string& reg : validRegisters){
                auto it=regvals.find(reg);
                if(it!=regvals.end()){
                    string m=removeLeadingZeros(it->second);
                    if(m==""){
                        m+="0";
                    }
                    if(stoll(reg.substr(1))<10){
                        cout<<reg<<" =0x"<<m<<endl;
                    }
                    else{
                        cout<<reg<<"=0x"<<m<<endl;
                    }
                }
            }
            cout<<endl;
        }
        else if(command=="mem"){
            cin>>hexcode>>ct;
            int ct1=stoi(ct);
            hexcode=hexcode.substr(2);
            if(hexcode.size()<5){
                cout<<"Memory starts from 0x10000"<<endl;
                cout<<endl;
                continue;
            }
            if(hexToDecimal(hexcode)>hexToDecimal("50000") || hexToDecimal(hexcode)<hexToDecimal("10000")){
                cout<<"Memory don't exists"<<endl;
                cout<<endl;
                continue;
            }
            else if(hexcode.size()>5){
                cout<<"Memory don't exists"<<endl;
                cout<<endl;
                continue;
            }
            for(int i=0;i<ct1;i++){
                for (const auto& pair : memory) {
                    if (pair.first.substr(11)==hexcode) {
                        string m=removeLeadingZeros(pair.second);
                        if(m==""){
                            m+="0";
                        }
                        cout<<"Memory[0x"<<pair.first.substr(11)<<"]=0x"<< m<<endl;
                        break; 
                    }
                }
                if(hexToDecimal(hexcode)>hexToDecimal("50000")){
                    break;
                }
                hexcode=addHex(hexcode,"1").substr(11);
            }
            cout<<endl;
        }
        else if(command=="exit"){
            cout<<"Exited the simulator"<<endl;
            break;
        }
        else if(command=="cache_sim"){
            string cmd1,cmd2;
            cin >> cmd1;
            if(cmd1=="enable"){
                cin >> cmd2;
                direct_mapped_cache.clear();
                fifo.clear();
                lru.clear();
                lfu.clear();
                cache_on=true;
                size_t lastDotPos = filename.find_last_of('.');
                if (lastDotPos != string::npos && lastDotPos != 0) {
                    filename = filename.substr(0, lastDotPos);  // Remove the extension
                }
                string outpfile = filename + ".output";
                ofstream otpfile(outpfile);
                ifstream cachefile(cmd2);
                getline(cachefile, cache_size);
                getline(cachefile, block_size);
                getline(cachefile, associativity);
                getline(cachefile, replacement_policy);
                getline(cachefile, write_policy);
                lines=stoll(cache_size)/stoll(block_size);
                associate=stoll(associativity);
                if(associate==0){
                    associate=lines;
                    lines=1;

                }
                else lines=lines/associate;
                block_sizes=stoll(block_size);
                long long int don=lines,don1=block_sizes;
                long long int countit=0,countit1=0;
                while(don!=1){
                    don/=2;
                    countit++;
                }
                while(don1!=1) {
                    don1/=2;
                    countit1++;
                }
                indexi=countit;
                biteoffset=countit1;
                cout<<endl;
            }
            else if(cmd1=="disable"){
                cache_on=false;
                
            }
            else if(cmd1=="status"){
                if(cache_on){
                cout << "Cache simulator enabled" << endl;
                cout << "Cache Size: " << cache_size << endl;
                cout << "Block Size: " << block_size << endl;
                cout << "Associativity: " << associativity << endl;
                cout << "Replacement Policy: " << replacement_policy << endl;
                cout << "Write Back Policy: " << write_policy << endl<<endl;
                }
                else{
                    cout << "Cache simulator disabled" << endl<<endl;
                }
            }
            else if(cmd1=="dump"){
                cin >> cmd2;
                CacheEntry curr_entry;
                string curr_index;
                ofstream outfile(cmd2);
                for(auto it=direct_mapped_cache.begin();it!=direct_mapped_cache.end();++it){
                    curr_index=it->first;
                    curr_entry=it->second;
                    string clean="Clean";
                    if(curr_index.size()%4!=0){
                        while(curr_index.size()%4!=0){
                            curr_index="0"+curr_index;
                        }
                    }
                    for(int j=0;j<associate;j++){
                        if(curr_entry.dirty_bit[j]) clean="Dirty";
                        if(curr_entry.valid[j]) outfile << "Set: 0x" << removeLeadingZeros(binary_hex(curr_index)) << ", Tag: 0x" << removeLeadingZeros(binary_hex(curr_entry.tags[j])) << ", " << clean << endl; 
                    }
                }
                cout<<endl;
                outfile.close();
            }
            else if(cmd1=="stats"){
                if(cache_on){
                    if(hits+misses==0) cout<<endl;
                    else {float percent = (float)hits/(float)(hits+misses);
                    cout << "D-Cache statistics: Accesses=" <<hits+misses<<", Hit=" << hits << ", Miss=" << misses << ", Hit Rate=" << percent << endl<<endl; }  
                }
                else{
                    cout<<"Cache simulator is disabled"<<endl<<endl;
                }
            }
            else if(cmd1=="invalidate"){
                CacheEntry hi;
                string hello;
                for(auto it=direct_mapped_cache.begin();it!=direct_mapped_cache.end();it++){
                    hi = it->second;
                    hello=it->first;
                    if(hello.size()) hello=hello.substr(4-indexi%4);
                    for(int j=0;j<associate;j++){
                        if(hello.size()) hi.tags[j] = hi.tags[j].substr(indexi); 
                        else{
                            hi.tags[j] = hi.tags[j].substr(1); 
                        }
                        if(hi.dirty_bit[j]){
                            string toaddit="";
                            for(int k=0;k<biteoffset;k++){
                                toaddit+="0";
                            }
                            string damn=hi.tags[j]+hello+toaddit;
                            damn=removeLeadingZeros(binary_hex(damn));
                            while(damn.size()<16) damn="0"+damn;
                            // cout<<damn<<endl;
                            for(int i=0;i<(int)memory.size();i++){
                                if(memory[i].first==damn){
                                    int arnav=0;
                                    int kaushal=block_sizes;
                                    int f=i;
                                    // cout<<hi.data[j]<<endl;
                                    if(arnav<(int)memory.size() && kaushal!=0){
                                        while(arnav<(int)memory.size()-1 && kaushal!=0  && arnav<=hi.data[j].size()-2){
                                            memory[f].second=hi.data[j].substr(arnav,2);
                                            // cout<<memory[f].first<<endl;
                                            kaushal-=2; 
                                            f++;
                                            arnav+=2;
                                        }
                                    }
                                    break;
                                }
                            }      
                        }
                    }
                }
                direct_mapped_cache.clear();
                fifo.clear();
                lru.clear();
                lfu.clear();
                cout<<endl;
            }
        }
        else{
            cout<<"Enter the valid command "<<endl;
            cout<<"Valid commands are: "<<endl;
            cout<<"     load <filename>"<<endl;
            cout<<"     run"<<endl;
            cout<<"     regs"<<endl;
            cout<<"     exit"<<endl;
            cout<<"     mem <addr> <count>"<<endl;
            cout<<"     show-stack"<<endl;
            cout<<"     break <line>"<<endl;
            cout<<"     cache_sim invalidate"<<endl;
            cout<<"     cache_sim enable [file containning cache info]"<<endl;
            cout<<"     cache_sim dump filename"<<endl;
            cout<<"     cache_sim stats"<<endl;
            cout<<"     cache_sim status"<<endl;
            cout<<"     cache_sim disable"<<endl;
            cout<<endl;
        }
        if(curr_line==no_of_instructions){
            break_pt_line_no.clear();
            pc="0000000000000000";
            labels.clear();
            continue;
        }
    }
    return 0;
}
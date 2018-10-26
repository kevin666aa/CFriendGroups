
#include "FriendGroups.h"
class CFriendGroups::CImplementation{
public:
    class value{
    public:
        std::string name;
        int next;
        int depth;
        value(std::string name, int next, int depth) : name(name), next(next), depth(depth){ }
    };
    class hash_table{
    public:
        int N;   // num of pairs
        int M;   //table size
        std::vector<int> hash_tb;
        std::vector<value> name_tb;
        
        hash_table(int size = 1000){
            M = size;
            N = 0;
            hash_tb.resize(size);
            name_tb.push_back(value("default", 0, 0));
        }
        
        std::string get_str(int ind){
            return name_tb.at(ind).name;
        }
        
        void resize(int n){
            std::vector<int> temp(n);
            for (auto i : hash_tb) {
                if (i != 0) {
                    int k = hash(get_str(i), n), h = 1;
                    while ( temp.at(k) != 0) {
                        h++;
                        k = (k + h * h) % n;
                    }
                    temp.at(k) = i;
                }
            }
            hash_tb = temp;
            M = n;
        }
        
        int hash(std::string key, int m)  {
            int intLength = (int)key.length() / 4;
            long sum = 0, mult = 1;
            for (int j = 0; j < intLength; j++) {
                std::string temp = key.substr(j * 4, (j * 4) + 4);
                for (int k = 0; k < (int)temp.size(); k++) {
                    sum += temp.at(k) * mult;
                    mult *= 253;
                }
            }
            std::string temp = key.substr(intLength * 4);
            mult = 1;
            for (int k = 0; k < (int)temp.size(); k++) {
                sum += temp.at(k) * mult;
                mult *= 253;
            }
            return (int)(abs(sum) % m);
        }

        void Union(int &index_0, int &index_1){
            if (name_tb.at(index_0).depth >= name_tb.at(index_1).depth) {
                name_tb.at(index_0).depth += name_tb.at(index_1).depth;
                name_tb.at(index_1).depth = 0;
                name_tb.at(index_1).next = index_0;
            } else{
                name_tb.at(index_1).depth += name_tb.at(index_0).depth;
                name_tb.at(index_0).depth = 0;
                name_tb.at(index_0).next = index_1;
            }
        }
        
        //para i is the index of name_tb, find root index in the name_tb
        int find_root(int i){
            int copy = i;
            while (i != name_tb.at(i).next) {
                i = name_tb.at(i).next;
            } //i now is the index of root in the nametable
            int new_ind;
            while (name_tb.at(copy).next != i) {
                new_ind = name_tb.at(copy).next;
                name_tb.at(copy).next = i;
                copy = new_ind;
            }
            return i;
        }
        
        // return the index of the key in the name_table
        void find(const std::string& key, int &index){
            // find the index, index shouldn't be used, just for return value
            if (N >= M / 2) {
                resize(M * 2);
            }
            int h = 1, i = hash(key, M);
            for (; hash_tb.at(i) != 0; i = (i + h * h) % M) {
                if (get_str(hash_tb.at(i)) == key) {
                    index = find_root(hash_tb.at(i));
                    return;
                }
                h++;
            }
            N++;
            name_tb.push_back(value(key, N, 1));
            hash_tb.at(i) = N;
            index = N;
            return;
        }
        
        //just for SameFriendGroup, shouldn't be used in other places
        int get_index(const std::string& key){
            int h = 1, i = hash(key, M);
            for (; hash_tb.at(i) != 0; i = (i + h * h) % M) {
                if (get_str(hash_tb.at(i)) == key) {
                    return hash_tb.at(i);
                }
                h++;
            }
            return -1;
        }
    };
    hash_table name_table;
    
};

CFriendGroups::CFriendGroups() : DData(std::make_unique<CImplementation>()){
}

CFriendGroups::~CFriendGroups(){
}

bool CFriendGroups::SameFriendGroup(const TConnection &connection) noexcept{
    int a = DData->name_table.get_index(std::get<0>(connection)), b =  DData->name_table.get_index(std::get<1>(connection));
    if( a == -1 || b == -1){ return false;  }
    else{  return DData->name_table.find_root(a) == DData->name_table.find_root(b);  }
}

void CFriendGroups::AddFriendship(const TConnection &friendship) noexcept{
    int index_0, index_1;
    DData->name_table.find(std::get<0>(friendship), index_0);
    DData->name_table.find(std::get<1>(friendship), index_1);
    if(index_0 == index_1){ return; }
    DData->name_table.Union(index_0, index_1);
}

std::vector<CFriendGroups::TConnection> CFriendGroups::FriendshipsForSingleGroup() noexcept{
    std::vector<std::string> temp;
    for(auto i:  DData->name_table.name_tb){
        if(i.depth > 0){
            temp.push_back(i.name);
        }
    }
    if((int)temp.size() <= 1){ return std::vector<CFriendGroups::TConnection>();}
    
    std::vector<CFriendGroups::TConnection> a;
    for(int i = 0; i < (int)temp.size() - 1; i++){
        a.push_back( std::make_tuple( temp.at(i), temp.at(i + 1) ));
    }
    return a;
}



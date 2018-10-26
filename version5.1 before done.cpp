//large duration 1508 Yeah!!!!!!!!!!!!!!!!!!!!!!!!!!
#include "FriendGroups.h"

class CFriendGroups::CImplementation{
public:
    struct value{
        int self;
        int next;
        int depth;
    };
    class hash_table{
    public:
        int N;   // num of pairs
        int M;   //table size
        std::vector<std::string> map;
        std::vector< value > vals;
        
        hash_table(int size = 113){
            M = size;
            N = 0;
            vals.resize(size);
        }
        ~hash_table(){
            map.clear();
            vals.clear();
        }
        
        void resize(int n){
            std::vector< value > temp(n);
            for (auto i : vals) {
                if (i.self != 0) {
                    int k = hash(get_str(i.self), n), h = 1;
                    while (temp.at(k).self != 0) {
                        h++;
                        k = (k + h * h) % n;
                    }
                    temp.at(k).self = i.self;
                    temp.at(k).next = i.next;
                    temp.at(k).depth = i.depth;
                }
            }
            vals = temp;
            M = n;
        }
        
        int hash(std::string key, int m)  {
            int intLength = (int)key.length() / 4;
            long sum = 0, mult = 1;
            for (int j = 0; j < intLength; j++) {
                std::string temp = key.substr(j * 4, (j * 4) + 4);
                for (int k = 0; k < (int)temp.size(); k++) {
                    sum += temp.at(k) * mult;
                    mult *= 256;
                }
            }
            std::string temp = key.substr(intLength * 4);
            mult = 1;
            for (int k = 0; k < (int)temp.size(); k++) {
                sum += temp.at(k) * mult;
                mult *= 256;
            }
            return (int)(abs(sum) % m);
        }
        
        value at(int i){
            return vals.at(i);
        }
        
        //1
        void add_to(int &index_0, int &index_1){
            vals.at(index_0).depth += vals.at(index_1).depth;
            vals.at(index_1).depth = 0;
            vals.at(index_1).next = vals.at(index_0).next;
        }
        
        std::string get_str(int ind){
            return map.at(ind - 1);
        }
        
        int get_index(int ind){
            int h = 1, i = hash(get_str(ind), M);
            for (; vals.at(i).self != 0; i = (i + h * h) % M) {
                if (get_str(vals.at(i).self) == get_str(ind)) {
                    return i;
                }
                h++;
            }
            
            return -1;
        } //return the real index of vals
        
        int find_root(int i){  //real i
            int copy = i;
            while (vals.at(i).self != vals.at(i).next) { // if keys.i not equal the parent in val.i
                i = get_index( vals.at(i).next );  // get the index of the parent, pass in i
            } //i now is the index of root
            int new_ind, root_ind = vals.at(i).self;
            while (vals.at(copy).next != root_ind) {
                new_ind = vals.at(copy).next;
                vals.at(copy).next = root_ind;
                copy = get_index(new_ind);
            }
            return i;
        }
        
        void find(std::string key, int &index){
            // 1
            // find the index, index shouldn't be used, just for return value
            if (N >= M / 2) {
                resize(M * 2);
            }
            int h = 1, i = hash(key, M);
            for (; vals.at(i).self != 0; i = (i + h * h) % M) {
                if (get_str(vals.at(i).self) == key) {
                    index = find_root(i);
                    return;
                }
                h++;
            }
            N++;
            map.push_back(key); //map get the string
            vals.at(i).self = N;
            vals.at(i).next = N;
            vals.at(i).depth = 1;
            index = i;
            return;
        }
        int get_index(std::string key){
            int h = 1, i = hash(key, M);
            for (; vals.at(i).self != 0; i = (i + h * h) % M) {
                if (get_str(vals.at(i).self) == key) {
                    return i;
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
    if( a == -1 || b == -1){
        return false;
    } else{
        return DData->name_table.find_root(a) == DData->name_table.find_root(b);
    }
}

void CFriendGroups::AddFriendship(const TConnection &friendship) noexcept{
    int index_0, index_1;
    DData->name_table.find(std::get<0>(friendship), index_0);
    DData->name_table.find(std::get<1>(friendship), index_1);
    if(index_0 == index_1){ return; }
    
    if(DData->name_table.at(index_0).depth >= DData->name_table.at(index_1).depth){
        DData->name_table.add_to(index_0, index_1);
    } else {
        DData->name_table.add_to(index_1, index_0);
    }
}

std::vector<CFriendGroups::TConnection> CFriendGroups::FriendshipsForSingleGroup() noexcept{
    std::vector<int> temp;
    int count = 0, i = 0;
    while(i < (int)DData->name_table.M && count < (int)DData->name_table.N){
        if(DData->name_table.at(i).depth > 0){
            temp.push_back(DData->name_table.at(i).self);
            count++;
        }
        i++;
    }

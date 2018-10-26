#include "FriendGroups.hpp"
#include <iostream>
class CFriendGroups::CImplementation{
public:
    class value{
    public:
        std::string key;
        std::string parent;
        //        value() : key(""), parent(""), depth(0){}
        int depth;
        bool empty(){ return key.empty(); }
        //        value(std::string key, std::string parent, int depth) : key(key), parent(parent), depth(depth){}
    };
    int N;   // num of pairs
    int M;   //table size
    std::vector<value> keys;
    std::vector<std::string> root;
    
    CImplementation(int size = 20) : M(size), N(0){
        keys.resize(size);
    }
    
    int hash(std::string key, int size)  {
        int intLength = (int)key.length() / 4;
        long sum = 0, mult = 1;
        for (int j = 0; j < intLength; j++) {
            std::string temp = key.substr(j * 4, (j * 4) + 4);
            for (int k = 0; k < temp.size(); k++) {
                sum += temp.at(k) * mult;
                mult *= 253;
            }
        }
        std::string temp = key.substr(intLength * 4);
        mult = 1;
        for (int k = 0; k < temp.size(); k++) {
            sum += temp.at(k) * mult;
            mult *= 253;
        }
        return (int)(abs(sum) % size);
    }
    
    void resize(int n){
        std::cout << "resize:" << n << std::endl;
        std::vector<value> temp(n);
        for (auto t : keys) {
            if (!t.empty()) {
                int a = hash(t.key, n);
                int h = 1;
                for (; !temp.at(a).empty(); a = (a + h * h) % n) { h++; }
                temp.at(a).key = t.key;
                temp.at(a).parent = t.parent;
                temp.at(a).depth = t.depth;
            }
        }
        keys = temp;
        M = n;
    }
    
    std::string get_parent(std::string key) {
        int h = 1, i = hash(key, M);
        for (; !keys.at(i).empty(); i = (i + h * h) % M) {
            if (keys.at(i).key == key) {
                return keys.at(i).parent;
            }
            h++;
        }printf("big problem1\n");
        return std::string();
    }
    
    int get_index(std::string key){
        int h = 1, i = hash(key, M);
        for (; !keys.at(i).empty(); i = (i + h * h) % M) {
            if (keys.at(i).key == key) {
                return i;
            }
            h++;
        }printf("big problem2\n");
        return -1;
    }
    
    //this function is the origin of put elements in the table
    int muti_fuc(std::string key, bool can_resize = true){
        if (N >= M / 2 && can_resize) {
            resize(M * 2);
        } //befor starting, double the size if needed
        
        int i = hash(key, M);
        int h = 1;
        for (; !keys.at(i).empty(); i = (i + h * h) % M) {
            if (keys.at(i).key == key) {
                //find the root, where key = parent
                return get_root_index(i);
            }
            h++;
        }// this means keys.at i is empty and key is a new one, so put it in the table
        keys.at(i).key = key;
        keys.at(i).parent = key;
        keys.at(i).depth = 1;
        binary_add_or_delete(key);  //its add here
        N++;
        return i;
    }
    
    int get_root_index(int i){
        std::string key = keys.at(i).key;
        while (keys.at(i).key != keys.at(i).parent) {
            i = get_index( keys.at(i).parent );
        } //i now is the index of root
        std::string new_key, root_string = keys.at(i).key;
        while (key != root_string) {
            new_key = get_parent(key);
            get_parent(key) = root_string;
            key = new_key;
        } // compress the pass
        return i;
    }
    
    void binary_add_or_delete(std::string name){
        int low = 0, high = (int)root.size() - 1, mid = 0;
        while(low <= high){
            mid = low + (high - low) / 2;
            if(name < root.at(mid)){
                high = mid - 1;
            } else if(name > root.at(mid)){
                low = mid + 1;
            } else{
                root.erase(root.cbegin() + mid);
                return;
            }
        }
        root.insert(root.cbegin() + low, name);
    }
    
    void check_root_vec(int fir, int sec){
        if (keys.at(fir).depth >= keys.at(sec).depth) {
            binary_add_or_delete(keys.at(sec).key);
            keys.at(fir).depth += keys.at(sec).depth;
            keys.at(sec).depth = 0;
            keys.at(sec).parent = keys.at(fir).key;
        } else{
            binary_add_or_delete(keys.at(fir).key);
            keys.at(sec).depth += keys.at(fir).depth;
            keys.at(fir).depth = 0;
            keys.at(fir).parent = keys.at(sec).key;
        }
    }
    
    
};

CFriendGroups::CFriendGroups() : DData(std::make_unique<CImplementation>()){
    
}

CFriendGroups::~CFriendGroups(){
    
}

bool CFriendGroups::SameFriendGroup(const TConnection &connection) noexcept{
    int a = DData->get_index(std::get<0>(connection)), b = DData->get_index(std::get<1>(connection));
    if(a < 0 || b < 0){ return false; }
    return DData->get_root_index(a) ==  DData->get_root_index(b);
}

//    {0   ,  1,     2}
//    {name, id, depth}
void CFriendGroups::AddFriendship(const TConnection &friendship) noexcept{
    int index_0 = DData->muti_fuc(std::get<0>(friendship)), index_1 = DData->muti_fuc(std::get<1>(friendship), false);
    if(index_0 == index_1){ return; }
    std::cout <<"begin" << std::endl;
    for(auto i: DData->root){
        std::cout << i << std::endl;
    }
    DData->check_root_vec(index_0, index_1);
    
    
    
    std::cout  << std::endl;
    for(auto i: DData->root){
        std::cout << i << std::endl;
    } std::cout << "end" << std::endl;
    }
    
    std::vector<CFriendGroups::TConnection> CFriendGroups::FriendshipsForSingleGroup() noexcept{
        if((int)DData->root.size() <=1 ){
            return std::vector<CFriendGroups::TConnection>();
        }
        std::vector<CFriendGroups::TConnection> a;
        
        for(int i = 0; i < (int)DData->root.size() - 1; i++){
            a.push_back( std::make_tuple( DData->root.at(i), DData->root.at(i + 1) ) );
        }
        return a;
    }

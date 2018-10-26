//fourth addition
//complete new way of put names
//takes forever for mid size
//failure

#include "FriendGroups.hpp"
#include <iostream>
class CFriendGroups::CImplementation{
public:
    std::vector< std::vector< std::vector<  std::tuple < unsigned int, unsigned int, unsigned int, std::string> > > > name_table;
    //first three is id like 5-7-16, second one is name
    
    std::vector< std::tuple<std::string, unsigned int>> root_table;
    // first string is name, second one is depth
    CImplementation(){
        for (auto i = 0; i < 26; i++) {
            name_table.push_back( std::vector< std::vector< std::tuple<unsigned int, unsigned int, unsigned int, std::string> > >());
            for (auto j = 0; j < 26; j++) {
                name_table.at(i).push_back( std::vector< std::tuple<unsigned int, unsigned int, unsigned int, std::string> >() );
            }
        }
    }
    
    auto parent(std::tuple<unsigned int, unsigned int, unsigned int>& orgin){
        auto temp = name_table.at(std::get<0>(orgin)).at(std::get<1>(orgin)).at(std::get<2>(orgin));
        return std::make_tuple(std::get<0>(temp), std::get<1>(temp), std::get<2>(temp));
    }
    
    auto findRoot_nametable(std::tuple<unsigned int, unsigned int, unsigned int> orgin){
        auto root = orgin;
        while (root != parent(root)) {
            root = parent(root);
        }
        std::tuple<unsigned int, unsigned int, unsigned int> temp;
        while (orgin != root) {
            temp = parent(orgin);
            parent(orgin) = root;
            orgin = temp;
        }
        return root;
    }
    
    void muti_func(std::string &name, int &index_root, std::tuple<unsigned int, unsigned int, unsigned int> &root_id){
        // every name in, need to find its location in name_table, and then find its root, if not in name_table, add it and its root is its self
        // also, after find the root(or add it), we need to find the location of root in the root_table(or add it),
        // these two params will be used in further comparison, change, union
        unsigned int a = name.at(0) - 65, b;
        if (name.size() == 1){
            b = 0;
        } else{
            b = name.at(1) - 97;
        }
        auto &temp = name_table.at(a).at(b);
        
        if (temp.size() == 0) {
            root_id = std::make_tuple(a, b, (unsigned int)1) ;
            temp.push_back(std::make_tuple(a, b, (unsigned int)0, name) );
            temp.push_back(std::make_tuple(a, b, (unsigned int)1, name) );
            index_root = locate_index_rootable(name);
            root_table.insert(root_table.cbegin() + index_root, std::make_tuple(name, 1));
            return;
        }
        unsigned int i = 1;
        while (i < (unsigned int)temp.size()) {
            if (name == std::get<3>(temp.at(i))   ) {
                root_id = findRoot_nametable(std::make_tuple(std::get<0>(temp.at(i)), std::get<1>(temp.at(i)),  std::get<2>(temp.at(i))) );
                index_root = locate_index_rootable(std::get<3>(name_table.at(std::get<0>(root_id)).at(std::get<1>(root_id)).at(std::get<2>(root_id)) ) );
                name = std::get<0>(root_table.at(index_root));
                return;
            } else if(name > std::get<3>(temp.at(i) ) ){
                i = 1 + (i << 1);
            } else {
                i <<= 1;
            }
        }
        
        //trying to find the id of the name, the root of the name, and the index in root_table
        
        //if not, need insert in both name_table and root_table, and the id and index will both be clear
        temp.resize(i + 1);
        temp.at(i) = std::make_tuple(a, b, i , name);
        
        root_id = std::make_tuple(a, b, i);
        
        index_root =  locate_index_rootable(name);
        root_table.insert(root_table.cbegin() + index_root, std::make_tuple(name, 1));
        
    }
    
    
    // pass in id, find the id of its root
    
    int locate_index_rootable(std::string name){
        int low = 0, high = (int)root_table.size() - 1, mid = 0;
        while (low <= high) {
            mid = low + (high - low) / 2;
            if ( name < std::get<0>(root_table.at(mid)) ) {
                high = mid - 1;
            } else if( name > std::get<0>(root_table.at(mid)) ){
                low = mid + 1;
            } else {
                return mid;
            }
        }
        return low;
    }
    
    bool compare_find_root(std::string name1, std::tuple<unsigned int, unsigned int, unsigned int> &get0){
        unsigned int a = name1.at(0) - 65, b;
        if (name1.size() == 1){
            b = 0;
        } else{
            b = name1.at(1) - 97;
        }
        if (name_table.at(a).at(b).size() == 0) {
            return false;
        }
        auto temp = name_table.at(a).at(b);
        unsigned int i = 1;
        while (i < (unsigned int)temp.size()) {
            if ( name1 < std::get<3>(temp.at(i) ) ) {
                i <<= 1;
            } else if(name1 > std::get<3>(temp.at(i) ) ){
                i = 1 + (i << 1);
            } else {
                get0 = findRoot_nametable(std::make_tuple(std::get<0>(temp.at(i)), std::get<1>(temp.at(i)),  std::get<2>(temp.at(i))) );
                return true;
            }
        }
        return false;
    }
    //https: //algs4.cs.princeton.edu/15uf/WeightedQuickUnionPathCompressionUF.java.html
    
};

CFriendGroups::CFriendGroups() : DData(std::make_unique<CImplementation>()){
    
}

CFriendGroups::~CFriendGroups(){
    
}

bool CFriendGroups::SameFriendGroup(const TConnection &connection) noexcept{
    std::tuple<unsigned int, unsigned int, unsigned int> get0, get1;
    if(DData->compare_find_root(std::get<0>(connection), get0) &&  DData->compare_find_root(std::get<1>(connection), get1)){
        return get0 == get1;
    }
    return false;
}


void CFriendGroups::AddFriendship(const TConnection &friendship) noexcept{
    std::tuple<unsigned int, unsigned int, unsigned int> fir_id, sec_id;
    int fir_index, sec_index;
    std::string name1 = std::get<0>(friendship), name2 = std::get<1>(friendship);
    DData->muti_func(name1, fir_index, fir_id);
    DData->muti_func(name2, sec_index, sec_id);
    
    for(auto i: DData->root_table){
        std::cout << "{" << std::get<0>(i) << ", " << std::get<1>(i) << "}" <<std::endl;
        }
        if(fir_id == sec_id){return;}
        auto& a = DData->name_table.at(std::get<0>(fir_id)).at(std::get<1>(fir_id)).at(std::get<2>(fir_id));
        auto& b = DData->name_table.at(std::get<0>(sec_id)).at(std::get<1>(sec_id)).at(std::get<2>(sec_id));
        if(std::get<0>(DData->root_table.at(fir_index)) != name1) {
            std::cout << std::get<0>(DData->root_table.at(fir_index)) << " " << std::get<0>(friendship);
            fir_index ++;}
        
        if(std::get<1>(DData->root_table.at(fir_index)) >= std::get<1>(DData->root_table.at(sec_index))){
            std::get<1>(DData->root_table.at(fir_index)) += std::get<1>(DData->root_table.at(sec_index));
            DData->root_table.erase(DData->root_table.cbegin() + sec_index);
            b = std::make_tuple(std::get<0>(a),std::get<1>(a),std::get<2>(a), std::get<3>(b));
        } else {
            std::get<1>(DData->root_table.at(sec_index)) += std::get<1>(DData->root_table.at(fir_index));
            DData->root_table.erase(DData->root_table.cbegin() + fir_index);
            a = std::make_tuple(std::get<0>(b),std::get<1>(b),std::get<2>(b), std::get<3>(a));
        }
        std::cout <<   "after"<< std::endl;
        for(auto i: DData->root_table){
            std::cout << "{" << std::get<0>(i) << ", " << std::get<1>(i) << "}" <<std::endl;
        }
        std::cout <<    std::endl;
        
        }
        
        std::vector<CFriendGroups::TConnection> CFriendGroups::FriendshipsForSingleGroup() noexcept{
            if((unsigned int)DData->root_table.size() <= 1 ){
                return std::vector<CFriendGroups::TConnection>();
            }
            
            std::vector<CFriendGroups::TConnection> a;
            for(unsigned int i = 0; i < (unsigned int)DData->root_table.size() - 1; i++){
                a.push_back( std::make_tuple(std::get<0>(   DData->root_table.at(i)) , std::get<0>(DData->root_table.at(i + 1)) ));
                
            }
            return a;
        }


//third version
//Duration: 139

#include "FriendGroups.h"

class CFriendGroups::CImplementation{
public:
    std::vector< std::tuple<std::string, unsigned int, unsigned int> > id;
    std::vector< std::string > root;
    
    void pop_root(std::string &name){
        for (unsigned int i = 0; i < root.size(); i++) {
            if (root.at(i) == name) {
                root.erase(root.begin() + i);
                break;
            }
        }
    }
    // https: //algs4.cs.princeton.edu/15uf/WeightedQuickUnionPathCompressionUF.java.html
    bool connected(const TConnection &connection) noexcept{
        unsigned int f_index = 0, s_index = 0;
        bool a = false, b = false;
        if(id.size() == 0){ return false;}
        
        for(unsigned int i = 0; (unsigned int)i < id.size(); i++){
            if (std::get<0>(id.at(i)) == std::get<0>(connection)) {
                f_index = i;
                a = true;
                if (b) {break;}
            } else if (std::get<0>(id.at(i)) == std::get<1>(connection)){
                s_index = i;
                b = true;
                if (a) {break;}
            }
        }
        while (f_index != std::get<1>(id.at(f_index))) {
            f_index = std::get<1>(id.at(f_index));
        }
        while (s_index != std::get<1>(id.at(s_index))) {
            s_index = std::get<1>(id.at(s_index));
        }
        return s_index == f_index;
    }
};

CFriendGroups::CFriendGroups() : DData(std::make_unique<CImplementation>()){
    
}

CFriendGroups::~CFriendGroups(){
    
}

bool CFriendGroups::SameFriendGroup(const TConnection &connection) noexcept{
    return DData->connected(connection);
}
//    {0   ,  1,     2}
//    {name, id, depth}
void CFriendGroups::AddFriendship(const TConnection &friendship) noexcept{
    unsigned int fir_root = 0, sec_root = 0, pass, tempi;
    bool fir_done = false, sec_done = false;
    for(unsigned int i = 0; i < DData->id.size(); i++){
        if (std::get<0>(DData->id.at(i)) == std::get<0>(friendship) ) {
            fir_root = i;
            
            while (fir_root != std::get<1>(DData->id.at(fir_root))) {
                fir_root = std::get<1>(DData->id.at(fir_root));
            }
            tempi = i;
            while (tempi != fir_root) {
                pass = std::get<1>(DData->id.at(tempi));
                std::get<1>(DData->id.at(i)) = fir_root;
                tempi = pass;
            }
            
            if(sec_done) { fir_done = true; break;}
            fir_done = true;
        } else if (std::get<0>(DData->id.at(i)) == std::get<1>(friendship) ){
            sec_root = i;
            while (sec_root != std::get<1>(DData->id.at(sec_root))) {
                sec_root = std::get<1>(DData->id.at(sec_root));
            }
            tempi = i;
            while (tempi != sec_root) {
                pass = std::get<1>(DData->id.at(tempi));
                std::get<1>(DData->id.at(tempi)) = sec_root;
                tempi = pass;
            }
            if(fir_done) {sec_done = true; break;}
            sec_done = true;
        }
    } //if in id, find its root and return
    
    if(!fir_done){
        DData->id.push_back(std::make_tuple( std::get<0>(friendship), (unsigned int)DData->id.size(), 1) );
        DData->root.push_back(std::get<0>(friendship));
        fir_root = (unsigned int)DData->id.size() - 1;
    }
    if(!sec_done){
        DData->id.push_back(std::make_tuple( std::get<1>(friendship), (unsigned int)DData->id.size(), 1) );
        DData->root.push_back(std::get<1>(friendship));
        sec_root = (unsigned int)DData->id.size() - 1;
    }
    
    if(fir_root == sec_root){ return; }
    if(std::get<2>(DData->id.at(fir_root)) >= std::get<2>(DData->id.at(sec_root)) ){
        DData->pop_root(std::get<0>(DData->id.at(sec_root)));
        std::get<1>(DData->id.at(sec_root)) = fir_root;
        std::get<2>(DData->id.at(fir_root)) += std::get<2>(DData->id.at(sec_root));
        std::get<2>(DData->id.at(sec_root)) = 0;
    } else{
        DData->pop_root(std::get<0>(DData->id.at(fir_root)));
        std::get<1>(DData->id.at(fir_root)) = sec_root;
        std::get<2>(DData->id.at(sec_root)) += std::get<2>(DData->id.at(fir_root));
        std::get<2>(DData->id.at(fir_root)) = 0;
    }
}

std::vector<CFriendGroups::TConnection> CFriendGroups::FriendshipsForSingleGroup() noexcept{
    if((unsigned int)DData->root.size() <= 1 ){
        return std::vector<CFriendGroups::TConnection>();
    }
    std::vector<CFriendGroups::TConnection> a;
    
    for(unsigned int i = 0; i < (unsigned int)DData->root.size() - 1; i++){
        a.push_back( std::make_tuple( DData->root.at(i), DData->root.at(i + 1) ) );
    }
    return a;
}

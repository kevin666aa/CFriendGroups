//second edition, can pass all, slow
//friendsmed.csv
//Duration: 94


#include "FriendGroups.h"
class CFriendGroups::CImplementation{
public:
    std::vector< std::tuple<std::string, int, int> > id;
    std::vector< std::string > root;
    int add_find_root(std::string name){
        for(auto i = 0; i < (int)id.size(); i++){
            if (std::get<0>(id.at(i)) == name) {
                int root = i;
                while (root != std::get<1>(id.at(root))) {
                    root = std::get<1>(id.at(root));
                }
                int newi;
                while (i != root) {
                    newi = std::get<1>(id.at(i));
                    std::get<1>(id.at(i)) = root;
                    i = newi;
                }
                return root;
            }
        } //if in id, find its root and return
        
        //if not, add a new one and return itself
        id.push_back(std::make_tuple(name, (int)id.size(), 1) );
        root.push_back(name);
        return (int)id.size() - 1;
    }
    void pop_root(std::string name){
        for (int i = 0; i < (int)root.size(); i++) {
            if (root.at(i) == name) {
                root.erase(root.begin() + i);
                break;
            }
        }
    }
    //https: //algs4.cs.princeton.edu/15uf/WeightedQuickUnionPathCompressionUF.java.html
    bool connected(const TConnection &connection) noexcept{
        int f_index = 0, s_index = 0;
        bool a = false, b = false;
        if((int)id.size() == 0){ return false;}
        
        for(int i = 0; i < (int)id.size(); i++){
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
    int fir = DData->add_find_root(std::get<0>(friendship));
    int sec = DData->add_find_root(std::get<1>(friendship));
    if(fir == sec){ return; }
    if(std::get<2>(DData->id.at(fir)) >= std::get<2>(DData->id.at(sec)) ){
        DData->pop_root(std::get<0>(DData->id.at(sec)));
        std::get<1>(DData->id.at(sec)) = fir;
        std::get<2>(DData->id.at(fir)) += std::get<2>(DData->id.at(sec));
        std::get<2>(DData->id.at(sec)) = 0;
    } else{
        DData->pop_root(std::get<0>(DData->id.at(fir)));
        std::get<1>(DData->id.at(fir)) = sec;
        std::get<2>(DData->id.at(sec)) += std::get<2>(DData->id.at(fir));
        std::get<2>(DData->id.at(fir)) = 0;
    }
    
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

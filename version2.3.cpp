//version 5
//update of version 2, but a little bit faster
#include "FriendGroups.h"

class CFriendGroups::CImplementation{
public:
    std::vector< std::tuple<std::string, int> > id;
    std::vector< std::tuple<std::string, int> > root;
    CImplementation(){
        id.resize(1);
    }
    auto findRoot_id(int i){
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
    
    int add_find_root(std::string name,int &root_index){
        int temp = 1;
        while (temp < (int)id.size()) {
            if (std::get<0>(id.at(temp)) == "") {
                id.at(temp) = std::make_tuple(name, temp);
                root.push_back(std::make_tuple(name, 1) );
                root_index = (int)root.size() - 1;
                return temp;
            } else if (name < std::get<0>(id.at(temp))) {
                temp <<= 1;
            } else if(name > std::get<0>(id.at(temp))){
                temp = 1 + (temp << 1);
            } else {
                temp = findRoot_id(temp);
                name = std::get<0>(id.at(temp));
                for (int i = 0; i < (int)root.size(); i++) {
                    if (name == std::get<0>(root.at(i))) {
                        root_index = i;
                        return temp;
                    }
                }
            }
        }
        
        //if not, add a new one and return itself
        root.push_back(std::make_tuple(name, 1) );
        root_index = (int)root.size() - 1;
        id.resize(temp + 1);
        id.at(temp) = std::make_tuple(name, temp);
        return temp;
    }
    
    //https: //algs4.cs.princeton.edu/15uf/WeightedQuickUnionPathCompressionUF.java.html
    bool check(std::string name, int& mid) noexcept{
        int temp = 1;
        while (temp < (int)id.size()) {
            if (name < std::get<0>(id.at(temp))) {
                temp <<= 1;
                } else if(name > std::get<0>(id.at(temp))){
                    temp = 1 + (temp << 1);
                } else {
                    mid = findRoot_id(temp);
                    return true;
                }
                }
                return false;
                }
                
                };
                
                CFriendGroups::CFriendGroups() : DData(std::make_unique<CImplementation>()){
                    
                }
                
                CFriendGroups::~CFriendGroups(){
                    
                }
                
                bool CFriendGroups::SameFriendGroup(const TConnection &connection) noexcept{
                    int a = 0, b = 0;
                    if(DData->check(std::get<0>(connection), a) && DData->check(std::get<1>(connection), b)){
                        return a == b;
                    }
                    return false;
                }
                //    {0   ,  1,     2}
                //    {name, id, depth}
                void CFriendGroups::AddFriendship(const TConnection &friendship) noexcept{
                    int r1, r2;
                    int fir = DData->add_find_root(std::get<0>(friendship), r1);
                    int sec = DData->add_find_root(std::get<1>(friendship), r2);
                    if(fir == sec){ return; }
                    if(std::get<1>(DData->root.at(r1)) >= std::get<1>(DData->root.at(r2)) ){
                        std::get<1>(DData->root.at(r1)) += std::get<1>(DData->root.at(r2));
                        DData->root.erase(DData->root.cbegin() + r2);
                        std::get<1>(DData->id.at(sec)) = fir;
                    } else{
                        std::get<1>(DData->root.at(r2)) += std::get<1>(DData->root.at(r1));
                        DData->root.erase(DData->root.cbegin() + r1);
                        std::get<1>(DData->id.at(fir)) = sec;
                    }
                }
                
                std::vector<CFriendGroups::TConnection> CFriendGroups::FriendshipsForSingleGroup() noexcept{
                    if((int)DData->root.size() <=1 ){
                        return std::vector<CFriendGroups::TConnection>();
                    }
                    std::vector<CFriendGroups::TConnection> a;
                    for(int i = 0; i < (int)DData->root.size() - 1; i++){
                        a.push_back( std::make_tuple( std::get<0>(DData->root.at(i)), std::get<0>(DData->root.at(i + 1)) ) );
                    }
                    return a;
                    
                }

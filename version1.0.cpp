//first editon, medium not pass

#include "FriendGroups.h"

class CFriendGroups::CImplementation{
public:
    std::vector< std::tuple<std::string, int> > id;
    std::vector<int> Sz;
    
    int root_finder(std::string name){
        int index = -1;
        Sz.push_back(1); //assume it is a new group with depth 1
        //check if the name in id
        for(auto i = 0; i < (int)id.size(); i++){
            if (std::get<0>(id.at(i)) == name) {
                //                std::cout << "check " << i << " ";
                Sz.pop_back();  //pop the size if it already existed
                index = i;
                break;
            }
        }
        if (index == -1) {
            id.push_back(std::make_tuple(name, (int)id.size()));   // if not in id, push new one in id
            index = (int)id.size() - 1; //index is the last one
        }
        
        while (std::get<1>(id.at(index)) != index) {
            index = std::get<1>(id.at(index));
        } //find its root
        return index;
    }
    
};

CFriendGroups::CFriendGroups() : DData(std::make_unique<CImplementation>()){
    
}

CFriendGroups::~CFriendGroups(){
    
}

bool CFriendGroups::SameFriendGroup(const TConnection &connection) noexcept{
    return DData->root_finder(std::get<0>(connection)) == DData->root_finder(std::get<1>(connection));
}

void CFriendGroups::AddFriendship(const TConnection &friendship) noexcept{
    int first_root = DData->root_finder(std::get<0>(friendship));
    int second_root = DData->root_finder(std::get<1>(friendship));
    
    if( DData->Sz.at(first_root) >= DData->Sz.at(second_root) ){
        std::get<1>(DData->id.at(second_root) )  = first_root;
        DData->Sz.at(first_root) +=  DData->Sz.at(second_root);
        DData->Sz.at(second_root) = 0;
    } else{
        std::get<1>(DData->id.at(first_root) )  = second_root;
        DData->Sz.at(second_root) +=  DData->Sz.at(first_root);
        DData->Sz.at(first_root) = 0;
    }
    // Your code here
}

std::vector<CFriendGroups::TConnection> CFriendGroups::FriendshipsForSingleGroup() noexcept{
    std::vector<std::string> a;
    for(auto i = 0; i < (int)DData->Sz.size(); i++){
        if(DData->Sz.at(i) != 0){
            a.push_back( std::get<0>(DData->id.at(i)) );
        }
    }
    std::vector<CFriendGroups::TConnection> temp;
    for(auto i = 0; i < (int)a.size() - 1; i++){
        temp.push_back( std::make_tuple(a.at(i), a.at(i+1)) );
    }
    return std::vector<CFriendGroups::TConnection>(temp);
}



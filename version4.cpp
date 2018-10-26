// can finish the lg.csv very slow

#include "FriendGroups.h"
class CFriendGroups::CImplementation{
public:
    std::vector< std::tuple<std::string, int> > ordered;
    std::vector< std::tuple<std::string, int, int> > id;
    std::vector< std::string > pointer_to_root;
    
    int findRoot_id(int i){
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
    int check_ordered(std::string name, int &ptr){
        //check if the name already passed in, always O(logn)
        
        int low = 0, high = (int)ordered.size() - 1, mid = 0;
        while (low <= high) {
            mid = low + (high - low) / 2;
            if ( name < std::get<0>(ordered.at(mid)) ) {
                high = mid - 1;
            } else if( name > std::get<0>(ordered.at(mid)) ){
                low = mid + 1;
            } else {
                mid = findRoot_id(std::get<1>(ordered.at(mid)) );
                ptr = find_ptr(mid);
                return mid;
            }
        }
        ordered.insert(ordered.cbegin() + low, std::make_tuple(name, (int)id.size()) );
        id.push_back( std::make_tuple(name, (int)id.size(), 1 ) );
        pointer_to_root.push_back( std::get<0>(id.at((int)id.size() - 1)) );
        ptr = (int)pointer_to_root.size() - 1;
        return (int)id.size() - 1;
    }
    
    int find_ptr(int index){
        int i = 0;
        for (; i < (int)pointer_to_root.size(); i++) {
            if (pointer_to_root.at(i) == std::get<0>(id.at(index)) ) {
                return i;
            }
        }
        return i;
    }
    
    //https: //algs4.cs.princeton.edu/15uf/WeightedQuickUnionPathCompressionUF.java.html
    bool check(std::string name, int& mid) noexcept{
        int low = 0, high = (int)ordered.size() - 1;
        while (low <= high) {
            mid = low + (high - low) / 2;
            if ( name < std::get<0>(ordered.at(mid)) ) {
                high = mid - 1;
            } else if( name > std::get<0>(ordered.at(mid)) ){
                low = mid + 1;
            } else {
                mid = findRoot_id(std::get<1>(ordered.at(mid)) );
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
    int ptr_1 = 0, ptr_2 = 0;
    int fir = DData->check_ordered(std::get<0>(friendship), ptr_1);
    int sec = DData->check_ordered(std::get<1>(friendship), ptr_2);
    if(fir == sec){ return; }
    if(std::get<2>(DData->id.at(fir)) >= std::get<2>(DData->id.at(sec))){
        std::get<1>(DData->id.at(sec)) = fir;
        std::get<2>(DData->id.at(fir)) += std::get<2>(DData->id.at(sec));
        std::get<2>(DData->id.at(sec)) = 0;
        DData->pointer_to_root.erase(DData->pointer_to_root.cbegin() + ptr_2);
    } else{
        DData->pointer_to_root.erase(DData->pointer_to_root.cbegin() + ptr_1);
        std::get<1>(DData->id.at(fir)) = sec;
        std::get<2>(DData->id.at(sec)) += std::get<2>(DData->id.at(fir));
        std::get<2>(DData->id.at(fir)) = 0;
    }
}

std::vector<CFriendGroups::TConnection> CFriendGroups::FriendshipsForSingleGroup() noexcept{
    if((int)DData->pointer_to_root.size() <=1 ){
        return std::vector<CFriendGroups::TConnection>();
    }
    std::vector<CFriendGroups::TConnection> a;
    
    for(int i = 0; i < (int)DData->pointer_to_root.size() - 1; i++){
        a.push_back( std::make_tuple( DData->pointer_to_root.at(i), DData->pointer_to_root.at(i + 1) ) );
    }
    return a;
}

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <exception>
#include "FriendGroups.h"


int main(int argc, char *argv[]){
    auto Start = std::chrono::steady_clock::now();
    CFriendGroups Groups;
    
    if(2 > argc){
        std::cerr<<"Syntax Error: proj2 file"<<std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream InFile(argv[1]);
    while(!InFile.eof()){
        std::string NewLine;
        std::getline(InFile, NewLine);
        auto CommaLocation = NewLine.find(",");
        
        if(std::string::npos != CommaLocation){
            Groups.AddFriendship(CFriendGroups::TConnection{NewLine.substr(0,CommaLocation), NewLine.substr(CommaLocation+1)});
        }
    }
    auto NewConnections = Groups.FriendshipsForSingleGroup();
    
    auto Duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-Start);
    std::cout<<"Duration: "<<Duration.count()<<std::endl;
    for(auto Connection : NewConnections){
        std::cout<<std::get<0>(Connection)<<" <-> "<<std::get<1>(Connection)<<std::endl;   
    }
        
    
    return EXIT_SUCCESS;
}

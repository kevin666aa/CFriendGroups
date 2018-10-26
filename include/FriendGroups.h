#ifndef FRIENDGROUPS_H
#define FRIENDGROUPS_H

#include <string>
#include <tuple>
#include <vector>
#include <memory>

class CFriendGroups{
    protected:
        class CImplementation;
        std::unique_ptr< CImplementation > DData;
        
    public:
        CFriendGroups();
        ~CFriendGroups();
        
        using TConnection = std::tuple<std::string, std::string>;
        
        bool SameFriendGroup(const TConnection &) noexcept;
        void AddFriendship(const TConnection &) noexcept;
        std::vector<TConnection> FriendshipsForSingleGroup() noexcept;
        
};

#endif

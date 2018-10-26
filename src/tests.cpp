#include <gtest/gtest.h>
#include <algorithm>
#include "FriendGroups.h"


TEST(FriendGroups, BasicFriendTest){
    CFriendGroups Groups;
    
    EXPECT_FALSE(Groups.SameFriendGroup(CFriendGroups::TConnection{"A", "B"}));
    Groups.AddFriendship(CFriendGroups::TConnection{"A", "B"});
    EXPECT_TRUE(Groups.SameFriendGroup(CFriendGroups::TConnection{"A", "B"}));
    Groups.AddFriendship(CFriendGroups::TConnection{"C", "D"});
    EXPECT_FALSE(Groups.SameFriendGroup(CFriendGroups::TConnection{"A", "C"}));
    auto FriendshipsNeeded = Groups.FriendshipsForSingleGroup();
    
    EXPECT_EQ(FriendshipsNeeded.size(), 1);
    if(FriendshipsNeeded.size()){
        auto MinFriend = std::min(std::get<0>(FriendshipsNeeded[0]), std::get<1>(FriendshipsNeeded[0]));
        auto MaxFriend = std::max(std::get<0>(FriendshipsNeeded[0]), std::get<1>(FriendshipsNeeded[0]));
        EXPECT_TRUE(MinFriend == "A" || MinFriend == "B");
        EXPECT_TRUE(MaxFriend == "C" || MaxFriend == "D");
    }
}

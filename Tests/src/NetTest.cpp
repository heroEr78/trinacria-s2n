#include <gtest/gtest.h>
#include <TrinacriaS2N/Net.h>

class NetTest : public testing::Test
{
public:
    NetTest()
    {
        _net.QueueNode("./resources/NetClassTest/Player.lua");
        _net.QueueNode("./resources/NetClassTest/Enemy.lua");
        _net.QueueNetNode("./resources/NetClassTest/Net.lua");
        _net.Expose("TestExpose",[this](const sol::state_view& state) -> sol::object
        {
            return sol::make_object(state, [this](const std::string& msg)
            {
                _messages.push_back(msg);
            });
        });
        _net.RunAll();
    }
protected:
    Trinacria::S2N::Net _net{ sol::lib::base };
    std::vector<std::string> _messages;
};

TEST_F(NetTest, GetNodeTest)
{
    Trinacria::S2N::Node* playerNode = nullptr;
    ASSERT_NO_THROW(
        playerNode = &_net.GetNode("Player");
    );
    
    EXPECT_EQ(playerNode->Id(), "Player");
    
    Trinacria::S2N::Node* enemyNode = nullptr;
    ASSERT_NO_THROW(
        enemyNode = &_net.GetNode("Enemy");
    );
    EXPECT_EQ(enemyNode->Id(), "Enemy");
}

TEST_F(NetTest, ConstGetNodeTest)
{
    const Trinacria::S2N::Node* playerNode = nullptr;
    ASSERT_NO_THROW(
        playerNode = &_net.GetNode("Player");
    );
    
    EXPECT_EQ(playerNode->GetId(), "Player");
    
    const Trinacria::S2N::Node* enemyNode = nullptr;
    ASSERT_NO_THROW(
        enemyNode = &_net.GetNode("Enemy");
    );
    EXPECT_EQ(enemyNode->GetId(), "Enemy");
}

TEST_F(NetTest, PublicTableTest)
{
    Trinacria::S2N::Node& playerNode = _net.GetNode("Player");
    Trinacria::S2N::Node& enemyNode = _net.GetNode("Enemy");
    
    sol::object playerHealth = playerNode.Public()["Health"];
    
    EXPECT_EQ(playerHealth.get_type(), sol::type::number);
    EXPECT_EQ(playerHealth.as<int>(), 100);
    
    sol::object playerDamageInflated = playerNode.Public()["DamageInflated"];
    
    EXPECT_EQ(playerDamageInflated.get_type(), sol::type::number);
    EXPECT_EQ(playerDamageInflated.as<int>(), 0);
    
    sol::object enemyHealth = enemyNode.Public()["Health"];
    
    EXPECT_EQ(enemyHealth.get_type(), sol::type::number);
    EXPECT_EQ(enemyHealth.as<int>(), 100);
    
    sol::object enemyDamageInflated = enemyNode.Public()["DamageInflated"];
    
    EXPECT_EQ(enemyDamageInflated.get_type(), sol::type::number);
    EXPECT_EQ(enemyDamageInflated.as<int>(), 0);
}

TEST_F(NetTest, EventTableTest)
{
    Trinacria::S2N::Node& playerNode = _net.GetNode("Player");
    Trinacria::S2N::Node& enemyNode = _net.GetNode("Enemy");
    
    sol::object playerOnDamaged = playerNode.Events()["OnDamaged"];
    EXPECT_EQ(playerOnDamaged.get_type(), sol::type::function);
    
    sol::object playerOnDamageEnemy = playerNode.Events()["OnDamageEnemy"];
    EXPECT_EQ(playerOnDamageEnemy.get_type(), sol::type::function);
    
    sol::object enemyOnDamaged = enemyNode.Events()["OnDamaged"];
    EXPECT_EQ(enemyOnDamaged.get_type(), sol::type::function);
    
    sol::object enemyOnDamagePlayer = enemyNode.Events()["OnDamagePlayer"];
    EXPECT_EQ(enemyOnDamagePlayer.get_type(), sol::type::function);
}

TEST_F(NetTest, CommunicationTest)
{
    Trinacria::S2N::Node& playerNode = _net.GetNode("Player");
    Trinacria::S2N::Node& enemyNode = _net.GetNode("Enemy");
    playerNode.Events()["OnDamageEnemy"]();
    
    EXPECT_EQ(((sol::object)playerNode.Public()["DamageInflated"]).as<int>(), 10);
    EXPECT_EQ(((sol::object)enemyNode.Public()["Health"]).as<int>(), 90);
    
    enemyNode.Events()["OnDamagePlayer"]();
    
    EXPECT_EQ(((sol::object)enemyNode.Public()["DamageInflated"]).as<int>(), 10);
    EXPECT_EQ(((sol::object)playerNode.Public()["Health"]).as<int>(), 90);
}


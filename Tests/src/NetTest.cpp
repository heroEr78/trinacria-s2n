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
        _net.Expose("TestExpose",[this](sol::state_view state) -> sol::object
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

TEST_F(NetTest, QueryEventsFromAnotherScriptTest)
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

TEST_F(NetTest, QueryPublicFromAnotherScriptTest)
{
    Trinacria::S2N::Node& playerNode = _net.GetNode("Player");
    Trinacria::S2N::Node& enemyNode = _net.GetNode("Enemy");
 
    playerNode.Events()["RefreshPose"]();
    EXPECT_EQ(((sol::object)playerNode.Public()["Pose"]).as<std::string>(), "EnemyHealthyPose");
    
    enemyNode.Events()["OnDamaged"](50);
    playerNode.Events()["RefreshPose"]();
    EXPECT_EQ(((sol::object)playerNode.Public()["Pose"]).as<std::string>(), "EnemyStrugglingPose");
    
    enemyNode.Events()["OnDamaged"](100);
    playerNode.Events()["RefreshPose"]();
    EXPECT_EQ(((sol::object)playerNode.Public()["Pose"]).as<std::string>(), "EnemyDeadPose");
}

// NOTE: this is a normal test without fixture
TEST(NetNodeWithMoreThan1Argument, NetNodeWithMoreThan1Argument)
{
    std::vector<std::string> messages;
    
    Trinacria::S2N::Net net(sol::lib::base);
    net.QueueNode("resources/NetNodeWithMultipleParametersTest/Script1.lua");
    net.QueueNode("resources/NetNodeWithMultipleParametersTest/Script2.lua");
    net.QueueNetNode("resources/NetNodeWithMultipleParametersTest/Net.lua");
    net.Expose("Log",[&messages](sol::state_view state) -> sol::object
    {
        return sol::make_object(state, [&messages](const std::string& msg)
        {
            messages.push_back(msg);
        });
    });
    net.RunAll();
    
    Trinacria::S2N::Node& script1 = net["Script1"];
    Trinacria::S2N::Node& script2 = net["Script2"];
    
    script1.Events()["RequestForScript2"]();
    
    EXPECT_EQ(((sol::object)script1.Public()["Pose"]).as<int>(), -2);
    EXPECT_EQ(((sol::object)script2.Public()["Pose"]).as<int>(), -1);
    
    sol::function_result script2RequireScript1Result = script2.Events()["RequireScript1"]();
    sol::function_result script2RequireEvents1Result = script2.Events()["RequireEvents1"]();
 
    // Compare in EXPECT_EQ strings so GTest say something human-readable
    auto script2RequireScript1ResultTypeName = sol::type_name(script2.GetEnvironment().lua_state(),
        script2RequireScript1Result.get_type());
    
    auto script2RequireEvents1ResultTypeName = sol::type_name(script2.GetEnvironment().lua_state(),
        script2RequireEvents1Result.get_type());
    
    if (!script2RequireScript1Result.valid()) {
        sol::error err = script2RequireScript1Result;
        FAIL() << "Test failed because of error while executing script: " << err.what();
    }
    
    if (!script2RequireEvents1Result.valid()) {
        sol::error err = script2RequireEvents1Result;
        FAIL() << "Test failed because of error while executing script: " << err.what();
    }
    
    EXPECT_EQ(script2RequireScript1ResultTypeName, "nil");
    EXPECT_EQ(script2RequireEvents1ResultTypeName, "nil");
}

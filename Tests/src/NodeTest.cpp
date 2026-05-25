#include <gtest/gtest.h>
#include "TrinacriaS2N/Node.h"

class NodeTest : public testing::Test
{
public:
    NodeTest(): _nodeEnvironment(_nodeState, sol::create, _nodeState.globals())
    {
        _nodeState.open_libraries(sol::lib::base);
        _nodeState.do_file("./resources/NodeClassTestScript.lua", _nodeEnvironment);
        _node.LinkEnvironment(_nodeEnvironment);
    }
private:
    sol::state _nodeState;
    sol::environment _nodeEnvironment;
protected:
    Trinacria::S2N::Node _node;
};

TEST_F(NodeTest, IdTest)
{
    std::string expectedId = "NodeClassTestScript";
    std::string actualId;
    
    ASSERT_NO_THROW({
        actualId = _node.Id();
    });
    
    EXPECT_EQ(expectedId, actualId);
}

TEST_F(NodeTest, GetIdTest)
{
    std::string expectedId = "NodeClassTestScript";
    _node.Id(); // To set the id field
    std::string actualId;
    
    ASSERT_NO_THROW({
        actualId = _node.GetId();
    });
    EXPECT_EQ(expectedId, actualId);
}

TEST_F(NodeTest, PublicTest)
{
    sol::table publicTable = _node.Public();
    std::map<std::string, sol::object> publicMap;
    
    for (const auto& [key, value] : publicTable)
    {
        ASSERT_EQ(key.get_type(), sol::type::string);
        auto tableKey = key.as<std::string>();
        publicMap[tableKey] = value;
    }
    
    ASSERT_NE(publicMap.size(), 0);
    
    EXPECT_EQ(publicMap["firstProp"].as<int>(), 6);
    EXPECT_EQ(publicMap["secondProp"].as<int>(), 7);
    
    ASSERT_EQ(publicMap["getHp"].get_type(), sol::type::function);
    auto getHp = publicMap["getHp"].as<sol::function>();
    
    sol::object gotHp = getHp();
    ASSERT_EQ(gotHp.get_type(), sol::type::number);
    int hp = gotHp.as<int>();
    EXPECT_EQ(hp, 100);
}

TEST_F(NodeTest, EventsTest)
{
    sol::table eventsTable = _node.Events();
    sol::table publicTable = _node.Public();
    std::map<std::string, sol::function> eventsMap;
    
    for (const auto& [key, value] : eventsTable)
    {
        ASSERT_EQ(key.get_type(), sol::type::string);
        ASSERT_EQ(value.get_type(), sol::type::function);
   
        eventsMap[key.as<std::string>()] = value.as<sol::function>();
    }
    
    sol::object getHpObj = publicTable["getHp"];
    ASSERT_EQ(getHpObj.get_type(), sol::type::function);
    
    sol::function getHp = getHpObj.as<sol::function>();
    sol::object hpObj = getHp();
    ASSERT_EQ(hpObj.get_type(), sol::type::number);
    
    auto hp = hpObj.as<int>();
    EXPECT_EQ(hp, 100);
    
    eventsMap["OnHit"](10);
    
    hp = ((sol::object) getHp()).as<int>();
    EXPECT_EQ(hp, 90);
    
    eventsMap["OnHit"](6);

    hp = ((sol::object) getHp()).as<int>();
    EXPECT_EQ(hp, 84);
}
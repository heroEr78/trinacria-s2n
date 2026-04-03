#include <gtest/gtest.h>
#include <TrinacriaS2N/NetNode.h>
#include <src/gtest-internal-inl.h>


class NetNodeTest : public testing::Test
{
public:
    NetNodeTest()
    {
        _netNodeState.open_libraries(sol::lib::base);
        _netNodeState.do_file("./resources/NetNodeClassTestScript.lua");
        _netNode.LinkState(_netNodeState);
    }
protected:
    Trinacria::S2N::NetNode _netNode;
private:
    sol::state _netNodeState;
};

TEST_F(NetNodeTest, IdTest)
{
    std::string expectedId = "NetNodeClassTestScript";
    std::string actualId;
    
    ASSERT_NO_THROW({
        actualId = _netNode.Id();
    });
    
    EXPECT_EQ(expectedId, actualId);
}

TEST_F(NetNodeTest, GetIdTest)
{
    std::string expectedId = "NetNodeClassTestScript";
    _netNode.Id(); // To set the id field
    std::string actualId;
    
    ASSERT_NO_THROW({
        actualId = _netNode.GetId();
    });
    EXPECT_EQ(expectedId, actualId);
}

TEST_F(NetNodeTest, GetScriptTest)
{
    sol::table getScriptTable = _netNode.GetScript("IdTest", "Add");
    std::map<std::string, sol::object> getScriptMap;
    
    for (const auto& [key, value]: getScriptTable)
    {
        ASSERT_EQ(key.get_type(), sol::type::string);
        getScriptMap[key.as<std::string>()] = value;
    }
    
    EXPECT_EQ(getScriptMap["prop1"].get_type(), sol::type::number);
    EXPECT_EQ(getScriptMap["prop1"].as<int>(), 6);
    
    EXPECT_EQ(getScriptMap["getHp"].get_type(), sol::type::function);
    auto getHp = getScriptMap["getHp"].as<sol::function>();
    sol::object hp = getHp();
    EXPECT_EQ(hp.get_type(), sol::type::number);
    EXPECT_EQ(hp.as<int>(), 67);
    
    EXPECT_EQ(getScriptMap["getId"].get_type(), sol::type::function);
    auto getId = getScriptMap["getId"].as<sol::function>();
    sol::object id = getId();
    EXPECT_EQ(id.get_type(), sol::type::string);
    EXPECT_EQ(id.as<std::string>(), "IdTest");
    
    EXPECT_EQ(getScriptMap["getAdd"].get_type(), sol::type::function);
    auto getAdd = getScriptMap["getAdd"].as<sol::function>();
    sol::object add = getAdd();
    EXPECT_EQ(add.get_type(), sol::type::string);
    EXPECT_EQ(add.as<std::string>(), "Add");
}

TEST_F(NetNodeTest, GetEventsTest)
{
    sol::table getEventsTable = _netNode.GetEvents("IdTest", "Add");
    
    sol::object onDamagedFuncObj = getEventsTable["OnDamage"];
    EXPECT_EQ(onDamagedFuncObj.get_type(), sol::type::function);
    
    sol::function onDamagedFunc = onDamagedFuncObj.as<sol::function>();
    sol::object damagedResult = onDamagedFunc(20);
    EXPECT_EQ(damagedResult.get_type(), sol::type::string);
    EXPECT_EQ(damagedResult.as<std::string>(), "Damaged id: IdTest by damage 20 with add Add");
}
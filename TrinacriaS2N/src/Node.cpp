#include "TrinacriaS2N/Node.h"
#include "TrinacriaS2N/asserts.h"
#include "TrinacriaS2N/log.h"

namespace Trinacria::S2N
{

Node::Node(sol::state& state): BaseNode(state)
{}

Node::Node(sol::state* state) : BaseNode(state)
{}

sol::table Node::Public() const
{
    TRCN_DEPEND_START("Trinacria::S2N::Node::Public");
    TRCN_ASSERTEXP_MSG(_state != nullptr, "Before doing anything with Node class you must link a state.");
    
    sol::object publicObj = (*_state)[PUBLIC_NODE_NAME];
    TRCN_ASSERTEXP_MSG(publicObj.is<sol::function>(), "The Public property should be function");
    
    auto publicFunc = publicObj.as<sol::function>();
    sol::object publicTableObj = publicFunc();
    
    TRCN_ASSERTEXP_MSG(publicTableObj.is<sol::table>(), "The Public property should be function returning a table");
    
    return publicTableObj.as<sol::table>();
}

sol::table Node::Events() const
{
    TRCN_DEPEND_START("Trinacria::S2N::Node::Events");
    TRCN_ASSERTEXP_MSG(_state != nullptr, "Before doing anything with Node class you must link a state.");
    
    sol::object eventsObj = (*_state)[EVENTS_NODE_NAME];
    if (!eventsObj.valid())
    {
        return _state->create_table();
    }
    
    TRCN_ASSERTEXP_MSG(eventsObj.is<sol::function>(), "The Events property should be function");
    
    auto eventsFunc = eventsObj.as<sol::function>();
    sol::object eventsTableObj = eventsFunc();
    
    TRCN_ASSERTEXP_MSG(eventsTableObj.is<sol::table>(), "The Events property should be a function returning table");
    
    return eventsTableObj.as<sol::table>();
}
}

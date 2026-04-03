#include "TrinacriaS2N/BaseNode.h"

#include <stdexcept>
#include <sol/sol.hpp>

#include "TrinacriaS2N/asserts.h"

namespace Trinacria::S2N
{

BaseNode::BaseNode(sol::state& state)
{
    LinkState(state);
}

BaseNode::BaseNode(sol::state* state)
{
    LinkState(state);
}

void BaseNode::LinkState(sol::state& state)
{
    LinkState(&state);
}

void BaseNode::LinkState(sol::state* state)
{
    _state = state;
}

const std::string& BaseNode::Id()
{
    TRCN_DEPEND_START("Trinacria::S2N::Node::Id");
    
    if (!_id.empty())
    {
        return _id;
    }
    
    TRCN_ASSERTEXP_MSG(_state != nullptr, "Before doing anything with Node class you must link a state.");
    
    sol::object idObj = (*_state)[ID_NODE_NAME];
    TRCN_ASSERTEXP_MSG(idObj.is<std::string>(), "The Id property should be a string.");
    
    _id = idObj.as<std::string>();
    TRCN_ASSERTEXP_MSG(!_id.empty(), "The Id property should be a not-empty string.");
    
    return _id;
}

const std::string& BaseNode::GetId() const
{
    return _id;
}

const sol::state& BaseNode::GetState() const
{
    return *_state;
}

sol::state& BaseNode::GetState()
{
    return *_state;
}

}


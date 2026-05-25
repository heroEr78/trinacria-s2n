#include "TrinacriaS2N/BaseNode.h"

#include <stdexcept>
#include <sol/sol.hpp>

#include "TrinacriaS2N/asserts.h"

namespace Trinacria::S2N
{

BaseNode::BaseNode(sol::environment& environment)
{
    LinkEnvironment(environment);
}

BaseNode::BaseNode(sol::environment* environment)
{
    LinkEnvironment(environment);
}

void BaseNode::LinkEnvironment(sol::environment& environment)
{
    LinkEnvironment(&environment);
}

void BaseNode::LinkEnvironment(sol::environment* environment)
{
    _environment = environment;
}

const std::string& BaseNode::Id()
{
    TRCN_DEPEND_START("Trinacria::S2N::Node::Id");
    
    if (!_id.empty())
    {
        return _id;
    }
    
    TRCN_ASSERTEXP_MSG(_environment != nullptr, "Before doing anything with Node class you must link a state.");
    
    sol::object idObj = (*_environment)[ID_NODE_NAME];
    TRCN_ASSERTEXP_MSG(idObj.is<std::string>(), "The Id property should be a string.");
    
    _id = idObj.as<std::string>();
    TRCN_ASSERTEXP_MSG(!_id.empty(), "The Id property should be a not-empty string.");
    
    return _id;
}

const std::string& BaseNode::GetId() const
{
    return _id;
}

const sol::environment& BaseNode::GetState() const
{
    return *_environment;
}

sol::environment& BaseNode::GetState()
{
    return *_environment;
}

}


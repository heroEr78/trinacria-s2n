#include <TrinacriaS2N/NetNode.h>

#include <sol/as_args.hpp>
#include <sol/variadic_args.hpp>

#include "TrinacriaS2N/asserts.h"

namespace Trinacria::S2N
{
NetNode::NetNode(sol::environment& environment) : BaseNode(environment)
{}

NetNode::NetNode(sol::environment* environment) : BaseNode(environment)
{}

sol::table NetNode::GetScript(const std::string& id, sol::variadic_args args) const
{
    TRCN_DEPEND_START("Trinacria::S2N::NetNode::GetScript");
    TRCN_ASSERTEXP_MSG(_environment != nullptr, "Before doing anything with NetNode class you must link a state.");
    
    sol::object getScriptFuncObj = (*_environment)[GET_SCRIPT_NET_NODE_NAME];
    TRCN_DEPEND_ASSERTEXP_MSG(getScriptFuncObj.is<sol::function>(), "The GetScript property should be a function.");
    
    sol::function getScriptFunc = getScriptFuncObj.as<sol::function>();
    
    sol::object getScriptFuncRes = getScriptFunc(id, sol::as_args(args));
    TRCN_DEPEND_ASSERTEXP_MSG(getScriptFuncRes.is<sol::table>(), "The GetScript property should be a function.");
    
    return getScriptFuncRes.as<sol::table>();
}


sol::table NetNode::GetEvents(const std::string& id, sol::variadic_args args) const
{
    TRCN_DEPEND_START("Trinacria::S2N::NetNode::GetEvents");
    TRCN_ASSERTEXP_MSG(_environment != nullptr, "Before doing anything with NetNode class you must link a state.");
    
    sol::object getEventsFunctionObject = (*_environment)[GET_EVENTS_NET_NODE_NAME];
    TRCN_DEPEND_ASSERTEXP_MSG(getEventsFunctionObject.is<sol::function>(), "The GetEvents property should be a function.");
    
    auto getEventsFunc = getEventsFunctionObject.as<sol::function>();
    
    sol::object getEventsFuncRes = getEventsFunc(id, sol::as_args(args));
    TRCN_DEPEND_ASSERTEXP_MSG(getEventsFuncRes.is<sol::table>(), "The GetEvents property should be a function.");
    
    return getEventsFuncRes.as<sol::table>();
}

}


#pragma once
#include <sol/state.hpp>

#include "TrinacriaS2N/BaseNode.h"
#include "TrinacriaS2N/asserts.h"

namespace Trinacria::S2N
{
constexpr auto GET_SCRIPT_NET_NODE_NAME = "GetScript";
constexpr auto GET_EVENTS_NET_NODE_NAME = "GetEvents";


/**
 * @brief NetNode is the class representing the net node script.
 *   The net node script is the messenger every script calls to 
 *   request the public table or the events of another script.
 *   This is not managed by the Net to give the user space to build 
 *   something on top of the simple "can I have this script" thing,
 *   for example a visibility system for the scripts.
 */
class NetNode : public BaseNode
{
public:
    NetNode() = default;

    /**
     * @brief A constructor that links the given state
     * @param state the state to link, a reference
     */
    explicit NetNode(sol::state& state);

    /**
     * @brief A constructor that links to a given state
     * @param state the pointer to the state to link
     */
    explicit NetNode(sol::state* state);

    /**
     * @brief The wrapper to the lua function GetScript.
     *   This calls the lua function under the hood
     * @tparam Args the type of the additional parameters (other than id),
     *   for example the id of the node that's requesting for another node.
     * @param id the id of the node requested
     * @param args the additional parameter built for the user to 
     *   add extra functionalities without modifying the library (only the script),
     *   for example a visibility system.
     * @return the public table of the node requested
     */
    template<typename ...Args>
    sol::table GetScript(const std::string& id, Args&&... args) const;
    
    /**
     * @brief The wrapper to the lua function GetEvents.
     *   This calls the lua function under the hood
     * @tparam Args the type of the additional parameters (other than id),
     *   for example the id of the node that's requesting for another node.
     * @param id the id of the node requested
     * @param args the additional parameter built for the user to 
     *   add extra functionalities without modifying the library (only the script),
     *   for example a visibility system.
     * @return the events table of the node requested
     */
    template<typename ...Args>
    sol::table GetEvents(const std::string& id, Args&&... args) const;
};


template<typename ... Args>
sol::table NetNode::GetScript(const std::string& id, Args&&... args) const
{
    TRCN_DEPEND_START("Trinacria::S2N::NetNode::GetScript");
    TRCN_ASSERTEXP_MSG(_state != nullptr, "Before doing anything with NetNode class you must link a state.");
    
    sol::object getScriptFuncObj = (*_state)[GET_SCRIPT_NET_NODE_NAME];
    TRCN_DEPEND_ASSERT_MSG(getScriptFuncObj.is<sol::function>(), "The GetScript property should be a function.");
    
    sol::function getScriptFunc = getScriptFuncObj.as<sol::function>();
    
    sol::object getScriptFuncRes = getScriptFunc(id, std::forward<Args>(args)...);
    TRCN_DEPEND_ASSERT_MSG(getScriptFuncRes.is<sol::table>(), "The GetScript property should be a function.");
    
    return getScriptFuncRes.as<sol::table>();
}

template<typename ... Args>
sol::table NetNode::GetEvents(const std::string& id, Args&&... args) const
{
    TRCN_DEPEND_START("Trinacria::S2N::NetNode::GetEvents");
    TRCN_ASSERTEXP_MSG(_state != nullptr, "Before doing anything with NetNode class you must link a state.");
    
    sol::object getEventsFunctionObject = (*_state)[GET_EVENTS_NET_NODE_NAME];
    TRCN_DEPEND_ASSERT_MSG(getEventsFunctionObject.is<sol::function>(), "The GetEvents property should be a function.");
    
    sol::function getEventsFunc = getEventsFunctionObject.as<sol::function>();
    
    sol::object getEventsFuncRes = getEventsFunc(id, std::forward<Args>(args)...);
    TRCN_DEPEND_ASSERT_MSG(getEventsFuncRes.is<sol::table>(), "The GetEvents property should be a function.");
    
    return getEventsFuncRes.as<sol::table>();
}

}

#pragma once
#include <sol/state.hpp>

namespace Trinacria::S2N
{
// Constants, modify them if you need
constexpr auto ID_NODE_NAME = "Id";
constexpr auto PUBLIC_NODE_NAME = "Public";
constexpr auto EVENTS_NODE_NAME = "Events";

/**
 * @brief The class that represents a lua node.
 *   A Node represents a script part of a Net. 
 *   Every script to be a node has to have: 
 *    *a Public() function that returns a table containing the properties of a script
 *    *an optional Events() function that returns a table of functions: 
 *      the functions are events that every other node can advise the script of
 *    *an Id string field, a unique-identifier for the script.
 */
class Node
{
public:
    Node() = default;
    
    /**
     * @brief Constructor that links the given state
     * @param state the given state, a reference
     */
    explicit Node(sol::state& state);

    /**
     * @brief Constructor that link the given state
     * @param state the given state, a pointer.
     *   This parameter is any different from the one of
     *   the other constructor
     */
    explicit Node(sol::state* state);

    /**
     * @brief Calls the other method to link a state
     * @param state the given state.
     */
    inline void LinkState(sol::state& state);

    /**
     * @brief Sets the state of a node
     * @param state the given state
     */
    void LinkState(sol::state* state);

    /**
     * @brief Gets the id from the object, if the id is empty, the function will grab it from the script
     * @return the script id
     */
    const std::string& Id();

    /**
     * @brief The const versions of Id() function
     * @return the object id
     */
    const std::string& GetId() const;

    /**
     * @brief Gets the public table of a script
     * @return the public table
     */
    sol::table Public() const;

    /**
     * @brief Gets the events table of a script
     * @return The events table, empty if none provided.
     */
    sol::table Events() const;

    /**
     * @brief Gets the state
     * @return the state
     */
    const sol::state& GetState() const;
    
    /**
     * @brief Gets the state
     * @return the state
     */
    sol::state& GetState();
private:
    sol::state* _state = nullptr;
    std::string _id;
};
}

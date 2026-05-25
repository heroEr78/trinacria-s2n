#pragma once
#include <sol/state.hpp>
#include <TrinacriaS2N/BaseNode.h>

namespace Trinacria::S2N
{

/**
 * @brief The class that represents a lua node.
 *   A Node represents a script part of a Net. 
 *   Every script to be a node has to have: 
 *    *a Public() function that returns a table containing the properties of a script
 *    *an optional Events() function that returns a table of functions: 
 *      the functions are events that every other node can advise the script of
 *    *an Id string field, a unique-identifier for the script.
 */
class Node : public BaseNode
{
public:
    Node() = default;
    
    /**
     * @brief Constructor that links the given state
     * @param state the given state, a reference
     */
    explicit Node(sol::environment& environment);

    /**
     * @brief Constructor that link the given state
     * @param state the given state, a pointer.
     *   This parameter is any different from the one of
     *   the other constructor
     */
    explicit Node(sol::environment* environment);

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
};
}

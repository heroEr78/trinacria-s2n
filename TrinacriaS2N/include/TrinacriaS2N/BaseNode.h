#pragma once
#include <string>

namespace sol
{
class state;
}

namespace Trinacria::S2N
{
// Constants, modify them if you need
constexpr auto ID_NODE_NAME = "Id";
constexpr auto PUBLIC_NODE_NAME = "Public";
constexpr auto EVENTS_NODE_NAME = "Events";

/**
 * @brief The base class of Node and NetNode
 */
class BaseNode
{
public:
    
    BaseNode() = default;
    virtual ~BaseNode() = default;
    
    /**
     * @brief Constructor that links the given state
     * @param state the given state, a reference
     */
    explicit BaseNode(sol::state& state);
    
    /**
     * @brief Constructor that link the given state
     * @param state the given state, a pointer.
     *   This parameter is any different from the one of
     *   the other constructor
     */
    explicit BaseNode(sol::state* state);
    
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
    virtual const std::string& Id();

    /**
     * @brief The const versions of Id() function
     * @return the object id
     */
    const std::string& GetId() const;
    
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
protected:
    sol::state* _state = nullptr;
    std::string _id;
};
}
#pragma once
#include <sol/variadic_args.hpp>
#include <string>
#include <unordered_map>
#include "TrinacriaS2N/NetNode.h"
#include "TrinacriaS2N/Node.h"

namespace Trinacria::S2N
{
constexpr auto PUBLIC_NET_NAME = "_NetScript";
constexpr auto PUBLIC_NODE_GETTER_NAME = "GetNode";
constexpr auto PUBLIC_EVENTS_GETTER_NAME = "GetEvents";
constexpr auto NET_NODE_GETTER_NODE = "_GetScript";
constexpr auto NET_NODE_GETTER_EVENTS = "_GetEvents";

/**
 * @brief The class representing a net, that is the system where
 *  all nodes live in and communicate. The net exposes the net node.
 */
class Net
{
public:
    /**
     * @brief The only constructor. Exposes the NetNode to all
     *   the nodes
     *   @tparam Args the vararg template of libs
     *   @param libs the libs the global state should opend
     */
    template<typename... Args>
    Net(Args&&... libs)
    {
        _globalState.open_libraries(std::forward<Args>(libs)...);
        Expose(
            PUBLIC_NET_NAME,
            [this](sol::state_view state) -> sol::table
            {
                sol::table table = state.create_table();

                table.set_function(
                    PUBLIC_NODE_GETTER_NAME,
                    [this](
                    const std::string& id,
                    const sol::variadic_args& opts) -> sol::table
                    {
                        return _netNode.GetScript(id, opts);
                    }
                    );

                table.set_function(
                    PUBLIC_EVENTS_GETTER_NAME,
                    [this](
                    const std::string& id,
                    const sol::variadic_args& opts) -> sol::table
                    {
                        return _netNode.GetEvents(id, opts);
                    }
                    );

                return table;
            }
            );
    }

    // Type aliases
    using Allocator = std::unordered_map<std::string, Node>;
    using Iterator = Allocator::iterator;
    using ConstIterator = Allocator::const_iterator;
    using SolObjectExposer = std::function<sol::object(sol::state_view)>;

    // Allocator functions

    /**
     * @brief Gets a Node that lives in the Net.
     * @param id the id of the Node, assigned by the script
     * @return The got node
     */
    [[nodiscard]] Node& GetNode(const std::string& id);

    /**
     * @brief Gets a Node that lives in the Net.
     * @param id the id of the Node, assigned by the script
     * @return The got node
     */
    [[nodiscard]] Node& operator[](const std::string& id) { return GetNode(id); }

    /**
     * @brief Gets a Node that lives in the Net.
     *   The const version of the previous functions.
     * @param id the id of the Node, assigned by the script
     * @return The got node
     */
    [[nodiscard]] const Node& GetNode(const std::string& id) const;
    /**
     * @brief Gets a Node that lives in the Net.
     *   The const version of the previous functions.
     * @param id the id of the Node, assigned by the script
     * @return The got node
     */
    [[nodiscard]] const Node& operator[](const std::string& id) const { return GetNode(id); }

    // Net functions
    /**
     * @brief Adds a node to the net, and queues its execution
     *   to when the RunAll function called.
     * @param path the path (absolute or relative) to the script
     */
    void QueueNode(const std::string& path);

    /**
     * @brief Adds a net node to the net, and queues its execution
     *   to when the RunAll function called.
     * @param path the path (absolute or relative) to the script
     */
    void QueueNetNode(const std::string& path);

    /**
     * @brief Queues the exposition of a sol::object returned 
     *   by the exposer to all the nodes added to the net.
     * @param name the name the scripts should be able to access exposed things
     * @param exposer the function that takes a sol::state (the node is exposed to) 
     *   and returns the object to be exposed
     */
    void Expose(const std::string& name, const SolObjectExposer& exposer);

    /**
     * @brief Runs all the nodes, should be executed once
     *   after the configuration done with queues and exposes methods.
     */
    void RunAll();

private:
    sol::state _globalState;


    struct BaseNodeData
    {
        std::string Path;
        sol::environment Environment;
    };


    Allocator _nodes;
    std::list<BaseNodeData> _nodeQueue;

    BaseNodeData _netNodeData;
    NetNode _netNode;


    struct ExposeObject
    {
        std::string Name;
        SolObjectExposer Exposer;
    };


    std::vector<ExposeObject> _exposeObjects;

    void exposeAllToState(sol::state& state) const;
    void runNetNode();
};
}

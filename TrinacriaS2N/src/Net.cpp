#include "TrinacriaS2N/Net.h"
#include "TrinacriaS2N/asserts.h"

namespace Trinacria::S2N
{

Node& Net::GetNode(const std::string& id)
{
    TRCN_DEPEND_START("Trinacria::S2N::GetNode");
    auto it = _nodes.find(id);
    TRCN_DEPEND_ASSERTEXP_MSG(it != _nodes.end(), "The id given is not associated to any node in the Net.");
    return it->second;
}

const Node& Net::GetNode(const std::string& id) const
{
    TRCN_DEPEND_START("Trinacria::S2N::GetNode");
    auto it = _nodes.find(id);
    TRCN_DEPEND_ASSERTEXP_MSG(it != _nodes.end(), "The id given is not associated to any node in the Net.");
    return it->second;
}

void Net::QueueNode(const std::string& path)
{
    _nodeQueue.emplace_back(
        path,
        sol::environment{ _globalState, sol::create, _globalState.globals() }
    );
}

void Net::QueueNetNode(const std::string& path)
{
    _netNodeData.Path = path;
    _netNodeData.Environment = { _globalState, sol::create, _globalState.globals() };
}

void Net::RunAll()
{
    exposeAllToState(_globalState);
    
    runNetNode();
    
    for (BaseNodeData& nodeData : _nodeQueue)
    {
        _globalState.do_file(
            nodeData.Path,
            nodeData.Environment
        );
       
        Node node(nodeData.Environment);
        
        std::string id = node.Id();
        _nodes.emplace(id, std::move(node));
    }
}

void Net::exposeAllToState(sol::state& state) const
{
    for (const ExposeObject& exposeObject : _exposeObjects)
    {
        state.set(exposeObject.Name, exposeObject.Exposer(sol::state_view{state} ));
    }
}

void Net::runNetNode()
{
    _netNodeData.Environment.set_function(NET_NODE_GETTER_NODE, [this](const std::string& id) -> sol::table
    {
        return GetNode(id).Public();
    });
    _netNodeData.Environment.set_function(NET_NODE_GETTER_EVENTS, [this](const std::string& id) -> sol::table
    {
        return GetNode(id).Events();
    });
    _globalState.do_file(_netNodeData.Path, _netNodeData.Environment);
    
    _netNode.LinkEnvironment(_netNodeData.Environment);
}

void Net::Expose(const std::string& name, const SolObjectExposer& exposer)
{
    _exposeObjects.emplace_back(name, exposer);
}

}

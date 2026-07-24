# TrinacriaS2N

TrinacriaS2N is a light scripting engine made to be used as a base for more complicated engines, or when a simple app needs something simple.

## Stack

- **Language**: C++

- **Generation of build files**: [CMake](https://cmake.org/)

- **Testing**: [GoogleTest](https://github.com/google/googletest) with CTest integration

- **Scripting Language**: [lua](https://www.lua.org/)

- **Scripting Library**: [sol](https://github.com/ThePhD/sol2)

## Theory

Trinacria S2N is composed of three parts that work together:

- **Node**: The script in a Net (see below). It is composed of: 
  
  * a `Public` function that returns a lua table containing the state. For example in a gaming engine, this will hold things like Health, DamageInflicted, PE if the script represents a player.
  
  * an `Events` function that returns a lua table containing all events (functions) the node might receive to perform actions. In a gaming engine, this can hold things like `OnDamaged()`.
  
  * an `Id`, a public string containing an identifier used by other script when they want to call an event or retrieving the state.

- **NetNode**: This is a special node (one for every net), that will be queried by nodes that want to trigger an event or get the state of another node. We created this additional layer to support adding more features to communicating, like a visibility layer. In this node should be specified the functions `GetScript` and `GetEvents` that can access `_GetScript(id)` and `_GetEvents(id)` exposed by the library. These function will do the actual work of respectively getting the public table and the events table. 

- **Net**: This is the network that the nodes and the net node live in. This is only c++ side, and you can do things common to all nodes like exposing a function. This holds the `sol::state`.

## Build the project

For building the project you need at least CMake 4.0 and a compile version of C++ 20, the recommended generator is Visual Studio 18 (2026).

### How to

- First, git clone using: `git clone https://github.com/heroEr78/trinacria-s2n.git --recurse-submodules`

- Then, move into the directory you cloned to: `cd trinacria-s2n`

- Last, generate project using CMake: `cmake -G Visual Studio 18 -B build`

- Finally, you can open the .sln file using Visual Studio

## Usage

### Lua nodes

Every script to be a `Trinacria::S2N::Node` should have an `Id`, a `Public` function and an `Events` function. Below is an example of a node. By default every node can access the `_NetScript` global, a table containing the function `GetEvents` and `GetScript` specified inside the `NetNode`.

```lua
Id = "Player"

local obj = {
    ["DamageInflated"] = 0,
    ["Health"] = 100
}

local events = {
    ["OnDamaged"] = function(damage)
        obj["Health"] = obj["Health"] - damage
    end,
    ["OnDamageEnemy"] = function()
        _NetScript.GetEvents("Enemy")["OnDamaged"](10)
        obj["DamageInflated"] = obj["DamageInflated"] + 10
    end,
    ["IncrementDamageInflated"] = function()
        obj["DamageInflated"] = obj["DamageInflated"] + 10
    end
}

function Public()
    return obj
end

function Events()
    return events
end 
```

### NetNode

This is a light `NetNode`, that doesn't add any layer on top of what the library has actually built.

```lua
Id = "Net"

function GetScript(id)
    return _GetScript(id)
end 

function GetEvents(id)
    return _GetEvents(id)
end
```

But you can add also something like this to create an access system (behavior not tested):

```lua
Id = "Net"

local accessTable = {
    ["Player"] = { ["Enemy"] = true },
    ["Enemy"] = { ["Player"] = true }
}

function GetScript(id, requestId)
    if(accessTable[requestId] and accessTable[requestId][id]) then
        return _GetScript(id)
    else 
        return nil
    end
end 

function GetEvents(id)
    return _GetEvents(id)
end
```

### Inside C++

Inside c++ you need to create a `Trinacria::S2N::Net`, add all of your nodes and net node,  and execute them like this:

```cpp
Trinacria::S2N::Net net(sol::lib::base);
net.QueueNode("path/to/node");
net.QueueNode("path/to/secondnode");
net.QueueNetNode("path/to/netnode");
net.RunAll();
```

If you want to expose something from C++ to Lua you can do this before running al the nodes:

```cpp
net.Expose("Something", [](sol::state_view state) -> sol::object
{
    return sol::make_object(state, "Else");
});
```

For more information about the library you can use the in-code documentation.

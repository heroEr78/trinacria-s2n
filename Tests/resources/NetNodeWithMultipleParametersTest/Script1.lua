Id = "Script1"

local state = {
    ["Field1"] = 1,
    ["Field2"] = "2",
    ["Pose"] = 0
}
local events = {
    ["Event1"] = function() print("Event1") end,
    ["Event2"] = function() print("Event2") end,
    ["RequestForScript2"] = function() 
        local script2PublicTable = _NetScript.GetScript("Script2", Id)
        local script2EventsTable = _NetScript.GetEvents("Script2", Id)
     
        state["Pose"] = script2PublicTable["PoseFor1"]
        script2EventsTable["SetPose"](-1)
    end
}

function Public()
    return state
end 

function Events()
    return events
end 
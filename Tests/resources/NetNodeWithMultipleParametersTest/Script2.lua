Id = "Script2"

local state = {
    ["Pose"] = 1,
    ["PoseFor1"] = -2
}

local events = {
    ["SetPose"] = function(poseGiven) 
        state["Pose"] = poseGiven
    end,
    ["RequireScript1"] = function()
        return _NetScript.GetScript("Script1", Id)
    end,
    ["RequireEvents1"] = function() 
        return _NetScript.GetEvents("Script1", Id)
    end
}

function Public()
    return state
end 

function Events()
    return events
end 
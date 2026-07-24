Id = "Net"

local accessTable = { 
    ["Script1"] = { ["Script2"] = true },
    ["Script2"] = {  }
}

function GetScript(id, requestId)
    local requesterPermissions = accessTable[requestId]
    if(requesterPermissions and requesterPermissions[id]) then
        return _GetScript(id)
    end
    return nil
end

function GetEvents(id, requestId)
    local requesterPermissions = accessTable[requestId]
    if(requesterPermissions and requesterPermissions[id]) then
        return _GetEvents(id)
    end
    return nil
end 
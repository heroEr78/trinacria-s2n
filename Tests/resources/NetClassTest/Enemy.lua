Id = "Enemy"

local obj = {
    ["Health"] = 100,
    ["DamageInflated"] = 0,
}

local events = {
    ["OnDamaged"] = function(damage)
        TestExpose("Enemy OnDamaged called with " .. damage)
        obj["Health"] = obj["Health"] - damage
    end,
    ["OnDamagePlayer"] = function()
        TestExpose("Enemy OnDamagePlayer called")
        _NetScript.GetEvents("Player")["OnDamaged"](10)
        obj["DamageInflated"] = obj["DamageInflated"] + 10
    end
}

function Public()
    return obj
end

function Events()
    return events
end

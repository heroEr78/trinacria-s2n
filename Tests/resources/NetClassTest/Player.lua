Id = "Player"

local obj = {
    ["DamageInflated"] = 0,
    ["Health"] = 100
}

local events = {
    ["OnDamaged"] = function(damage)
        TestExpose("Player OnDamaged called with " .. damage)
        obj["Health"] = obj["Health"] - damage
    end,
    ["OnDamageEnemy"] = function()
        TestExpose("Player OnDamageEnemy called")
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
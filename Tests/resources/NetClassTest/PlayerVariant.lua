Id = "player"

local netScript = _GetNetScript();
local getEvents = netScript["GetEvents"]
local getScript = netScript["GetScript"]

local object = {
    ["Health"] = 100,
    ["InflictedDamage"] = 0,
}

local function damage()
    local enemyEvents = getEvents("enemy")
    enemyEvents["OnDamage"](damage)
    object["InflictedDamage"] = object["InflictedDamage"] + damage

    local enemyScript = getScript("enemy")
    --print("The enemy health is now: " .. tostring(enemyScript["Health"]) .. "")
    --print("Its total inflicted damage: " .. tostring(enemyScript["InflictedDamage"]) .. "")

end

local function onDamage(damage)
    object["Health"] = object["Health"] - damage
end

object["Damage"] = damage

function Public()
    for id, value in pairs(object) do
        print("Id: " .. id .. ", value: " .. value)
    end
    return object
end

function Events()
    return {
        ["OnDamage"] = onDamage
    }
end


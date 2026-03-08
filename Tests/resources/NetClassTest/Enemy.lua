Id = "enemy"

local inflictedDamage = 0;
local health = 100;

function Public()
    local netScript = _GetNetScript();
    local getEvents = netScript["GetEvents"]
    local getScript = netScript["GetScript"]
    
    local result = {
        ["GetHealth"] = function () return health end,
        ["GetInflictedDamage"] = function() return inflictedDamage end,
        ["Damage"] = function (damage)
            local playerEvents = getEvents("player")
            playerEvents["OnDamage"](damage)
            inflictedDamage = inflictedDamage + damage

            local playerScript = getScript("player")
            --print("The player health is now: " .. tostring(playerScript["Health"]) .. "")
            --print("Its total inflicted damage: " .. tostring(playerScript["InflictedDamage"]) .. "")
        end
    }
    
    -- Optional: iterate for debug (kept commented out)
    for key, val in pairs(result) do
        local printValue = ""
        if(type(val) ~= "function") then
            printValue = val
        else
            printValue = tostring(val())
        end
        print("Key: " .. key .. ", val: " .. printValue)
    end

    return result
end

function Events()
    return {
        ["OnDamage"] = function (damage)
            health = health - damage;
        end
    }
end
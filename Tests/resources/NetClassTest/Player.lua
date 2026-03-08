Id = "player"

local health = 100;
local inflictedDamage = 0;

function Public()
    local netScript = _GetNetScript();
    local getEvents = netScript["GetEvents"]
    local getScript = netScript["GetScript"]
    
    local result =  {
        ["GetHealth"] = function () return health end,
        ["GetInflictedDamage"] = function() 
            print("GetInflictedDamage called")
            return inflictedDamage 
            end,
        ["Damage"] = function (damage)
            local enemyEvents = getEvents("enemy")
            enemyEvents["OnDamage"](damage)
            inflictedDamage = inflictedDamage + damage

            local enemyScript = getScript("enemy")
            --print("The enemy health is now: " .. tostring(enemyScript["Health"]) .. "")
            --print("Its total inflicted damage: " .. tostring(enemyScript["InflictedDamage"]) .. "")
        end
    }

    for key, val in pairs(result) do
        local printValue = ""
        if(type(val) ~= "function") then
            printValue = val
        end
        print("Key: " .. key .. ", val: " .. printValue)
    end
    return result
end

function Events()
    return {
        ["OnDamage"] = function (damage)
            health = health - damage
        end
    }
end
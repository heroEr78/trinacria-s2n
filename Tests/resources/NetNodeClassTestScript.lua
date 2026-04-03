-- Net node --
Id = "NetNodeClassTestScript"

function GetScript(id, add)
    -- Simulate real getting
    return {
        ["prop1"] = 6,
        ["getHp"] = function ()
            return 67
        end,
        ["getId"] = function ()
            return id
        end,
        ["getAdd"] = function() 
            return add
        end
    }
end

function GetEvents(id, add)
    -- Simulate real getting
    return {
        ["OnDamage"] = function (damage)
            return "Damaged id: " .. id .. " by damage " .. damage .. " with add " .. add .. ""
        end
    }
end

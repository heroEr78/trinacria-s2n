-- Net node --

function GetScript(id)
    -- Simulate real getting
    return {
        ["prop1"] = 6,
        ["prop2"] = 7,
        ["getHp"] = function ()
            return 67
        end,
        ["getId"] = function ()
            return id
        end
    }
end

function GetEvents(id)
    -- Simulate real getting
    return {
        ["OnDamage"] = function (damage)
            print("Damaged id: " .. id .. " by damage " .. damage .. "")
        end
    }
end

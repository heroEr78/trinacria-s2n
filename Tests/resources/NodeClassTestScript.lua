Id = "NodeClassTestScript"

local hp = 100;
function Public()
    return {
        ["firstProp"] = 6,
        ["secondProp"] = 7,
        ["getHp"] = function ()
            return hp;
        end
    }
end

function Events()
    return {
        ["OnHit"] = function (damage)
            hp = hp - damage;
       end
    }
end
Id = "Player"

local poses = {
    "EnemyHealthyPose",
    "EnemyStrugglingPose",
    "EnemyDeadPose",
}
local obj = {
    ["DamageInflated"] = 0,
    ["Health"] = 100,
    ["Pose"] = poses[1]
}

-- Source - https://stackoverflow.com/a/27028488
-- Posted by hookenz, modified by community. See post 'Timeline' for change history
-- Retrieved 2026-07-23, License - CC BY-SA 4.0

function dump(o)
    if type(o) == 'table' then
        local s = '{ '
        for k,v in pairs(o) do
            if type(k) ~= 'number' then k = '"'..k..'"' end
            s = s .. '['..k..'] = ' .. dump(v) .. ','
        end
        return s .. '} '
    else
        return tostring(o)
    end
end


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
    end,
    ["RefreshPose"] = function()
        local enemyObject = _NetScript.GetScript("Enemy")
        local enemyHealth = enemyObject["Health"]
        
        if(enemyHealth <= 100 and enemyHealth > 50) then
            obj["Pose"] = poses[1]
        elseif(enemyHealth <= 50 and enemyHealth > 0 ) then
            obj["Pose"] = poses[2]
        elseif(enemyHealth <= 0) then
            obj["Pose"] = poses[3]
        end
    end
}

function Public()
    return obj
end

function Events()
    return events
end 
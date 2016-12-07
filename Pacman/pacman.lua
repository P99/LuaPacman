
-- Pacman game in Lua OOP
Game = {}

-- Load map file and draw the whole thing
function Game:loadMap(filename)
  local actions = {
    ["#"] = function () print("Create wall") end,
    ["@"] = function () print("Create coin") end,
  }
  local f = assert(io.open(filename, "r"))
  while true do
    local line = f:read("*line")
    if line == nil then break end
    for i = 1, #line do
      local type = line:sub(i, i)
      if actions[type] ~= nill then
        actions[type]()
      end
    end
  end
  f:close()
end

--drawRect{top=5, left=24, width=53, height=79}
--drawRect{top=15, left=54, width=5, height=79}
--drawRect{top=45, left=24, width=3, height=75}


function Game:eventHandler()
  while true do
    local events = getKey()
    if events ~= nil then
      print("Event: " .. events)
    end
  end
end

function Game:init()
  print("Pacman game in Lua")
  Game:loadMap("map.txt")
  Game:eventHandler()
end

Game:init()



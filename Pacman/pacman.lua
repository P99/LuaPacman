
local Scene = require ("scene")

-- Pacman game in Lua OOP
Game = {
  scene = Scene:init()
}

-- Load map file and draw the whole thing
function Game:loadMap(filename)
  local actions = {
    ["#"] = "wall",
    ["@"] = "coin",
    [" "] = "empty",
  }
  local x, y = 1, 1
  local f = assert(io.open(filename, "r"))
  while true do
    local line = f:read("*line")
    if line == nil then break end
    for x = 1, #line do
      local type = line:sub(x, x)
      if actions[type] ~= nill then
        self.scene:addCell(actions[type], x, y)
      end
    end
    y = y + 1
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

-- Constructor
function Game:init()
  print("Pacman game in Lua")
  Game:loadMap("map.txt")
  Game:eventHandler()
end

-- Launch the game
Game:init()



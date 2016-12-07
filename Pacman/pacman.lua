
local Scene = require ("scene")

-- Pacman game in Lua OOP
Game = {
  scene = Scene:init()
}

-- Load map file and draw the whole thing
function Game:loadMap(filename)
  local cellTypes = {
    ["#"] = "wall",
    ["@"] = "coin",
    [" "] = "empty",
    ["x"] = "pacman"
  }
  local x, y = 1, 1
  local f = assert(io.open(filename, "r"))
  while true do
    local line = f:read("*line")
    if line == nil then break end
    for x = 1, #line do
      local type = line:sub(x, x)
      type = cellTypes[type]
      if type ~= nill then
        if type == "pacman" then
          self.scene:addCharacter(type, x, y)
        else
          self.scene:addCell(type, x, y)
        end
      end
    end
    y = y + 1
  end
  f:close()
end

function Game:eventHandler()
  local event = getKey()
  if event ~= nil then
    --print("Event: " .. event)
    self.scene:eventHandler(event)
  end
end

function Game:draw()
  if self.scene then
    --print("Repainting area")
    self.scene:draw()
  end
end

function Game:run()
  while true do
    self:eventHandler()
    self:draw()
    usleep(200)
  end
end

-- Constructor
function Game:init()
  print("Pacman game in Lua")
  self:loadMap("map.txt")
  self:run()
end

-- Launch the game
Game:init()



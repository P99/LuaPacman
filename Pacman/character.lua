
local Character = {}

function Character:init()
  local o = {}
  print("Adding new character")
  setmetatable(o, self)
  self.__index = self
  o.position = { x = 0, y = 0 }
  o.direction = { x = 0, y = 0 }
  o.rect = {}
  return o;
end

function Character:setPosition(x, y)
  self.position.x = x
  self.position.y = y
  --print("SetPosition " .. x .. " " .. y)
end

function Character:setRect(rect)
  self.rect = rect
end

function Character:draw()
  drawRect(self.rect)
end

return Character;


local Cell = {}

function Cell:init(type)
  local o = {}
  setmetatable(o, self)
  self.__index = self
  o.type = type
  o.dirty = true
  return o
end

function Cell:setRect(rect)
  self.rect = rect
end

function Cell:draw()
  local cellsColors = {
    wall = "grey",
    coin = "yellow",
    empty = "black",
  }
  if self.dirty then
    --print("Drawing " .. self.type .. " in " .. cellsColors[self.type])
    self.rect.color = cellsColors[self.type];
    drawRect(self.rect)
  end
end

return Cell;


local Cell = {}

function Cell:init(type)
  local o = {}
  setmetatable(o, self)
  self.__index = self
  o.type = type
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
  print("Drawing " .. self.type .. " in " .. cellsColors[self.type])
  drawRect(rect, cellsColors[self.type])
end

return Cell;

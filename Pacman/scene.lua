
local Cell = require ("cell")
local CELL_SIZE_PIXELS = 20

local Scene = {
cells = {{}}
}

function Scene:init()
local o = {}
  print ("Creating scene")
  setmetatable(o, self)
  self.__index = self
  return o
end

function Scene:addCell(type, x, y)
  --print("Adding cell")
  local cell = Cell:init(type, x, y)
  local rect = {
    left = (x * CELL_SIZE_PIXELS),
    top = (y * CELL_SIZE_PIXELS),
    width = CELL_SIZE_PIXELS,
    height = CELL_SIZE_PIXELS,
  }
  cell:setRect(rect)
  if self.cells[x] == nil then self.cells[x] = {} end
  self.cells[x][y] = cell
end

function Scene:draw()
  for i = 1, #self.cells do
    for j = 1, #self.cells[i] do
      self.cells[i][j]:draw()
    end
  end
end

return Scene

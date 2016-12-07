
local Cell = require ("cell")
local Character = require ("character")
local CELL_SIZE_PIXELS = 20

local Scene = {
  cells = {{}},
  characters = {}
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

function Scene:addCharacter(type, x, y)
  if type == "pacman" then
    local pacman = Character:init()

    pacman:setPosition(x, y)
    pacman:setRect{
      left = (x * CELL_SIZE_PIXELS),
      top = (y * CELL_SIZE_PIXELS),
      width = CELL_SIZE_PIXELS,
      height = CELL_SIZE_PIXELS,
      color = "yellow",
    }

    -- Need to create an empty cell as well
    -- in the same position
    self:addCell("empty", x, y)
    self.characters[#self.characters+1] = pacman
  end
end

function Scene:eventHandler(event)
  local pacman = self.characters[1]
  local pos = { x = pacman.position.x, y = pacman.position.y }
  local next = { x = pos.x, y = pos.y }

  -- Check wall collisions
  if event == "Up" then
    if self.cells[pos.x][pos.y -1].type ~= "wall" then
      next.y = pos.y -1
    end
  elseif event == "Down" then
    if self.cells[pos.x][pos.y +1].type ~= "wall" then
      next.y = pos.y +1
    end
  elseif event == "Left" then
    if self.cells[pos.x -1][pos.y].type ~= "wall" then
      next.x = pos.x -1
    end
  elseif event == "Right" then
    if self.cells[pos.x +1][pos.y].type ~= "wall" then
      next.x = pos.x +1
    end
  else
    print("Unknown event " .. event)
  end

  -- Check pacman grabbing coins
  if self.cells[next.x][next.y].type == "coin" then
    self.cells[next.x][next.y].type = "empty"
    -- Increment score
  end

  -- Only if move can be done, refresh the scene
  if next ~= pos then
    self.cells[pos.x][pos.y].dirty = true
    pacman:setPosition(next.x, next.y)
    pacman:setRect{
      left = (next.x * CELL_SIZE_PIXELS),
      top = (next.y * CELL_SIZE_PIXELS),
      width = CELL_SIZE_PIXELS,
      height = CELL_SIZE_PIXELS,
      color = "yellow",
    }
  self:draw()
  end
end

function Scene:draw()
  for i = 1, #self.cells do
    for j = 1, #self.cells[i] do
      self.cells[i][j]:draw()
    end
  end

  -- For now we have only one pacman
  -- and no gosts, but this is for later
  for i = 1, #self.characters do
    self.characters[i]:draw()
  end
end

return Scene

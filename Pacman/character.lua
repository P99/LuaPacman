
local Character = {}

function Character:init()
  local o = {}
  print("Adding new character")
  setmetatable(o, self)
  self.__index = self
  o.position = { x = 0, y = 0 }
  o.direction = ""
  o.rect = {}
  o.animation_stage = 1
  return o;
end

function Character:setPosition(x, y)
  if self.position.x < x then
    self.direction = "Right"
  elseif self.position.x > x then
    self.direction = "Left"
  end

  if self.position.y < y then
    self.direction = "Down"
  elseif self.position.y > y then
    self.direction = "Up"
  end

  self.animation_stage = ((self.animation_stage + 1) % 3) +1

  --print("Character direction: " .. self.direction);

  self.position.x = x
  self.position.y = y
  --print("SetPosition " .. x .. " " .. y)
end

function Character:setRect(rect)
  self.rect = rect
end

function Character:draw()
  local sprite_metadata = {
    Left = {
      {0, 0, 22, 22}, { 22, 0, 22, 22 }, {44, 0, 22, 22}
    },
    Right = {
      {0, 22, 22, 22}, { 22, 22, 22, 22 }, {44, 0, 22, 22}
    },
    Up = {
      {0, 44, 22, 22}, { 22, 44, 22, 22 }, {44, 0, 22, 22}
    },
    Down = {
      {0, 66, 22, 22}, { 22, 66, 22, 22 }, {44, 0, 22, 22}
    }
  }
  sprite = {}
  sprite.src_left = sprite_metadata[self.direction][self.animation_stage][1]
  sprite.src_top = sprite_metadata[self.direction][self.animation_stage][2]
  sprite.src_width = sprite_metadata[self.direction][self.animation_stage][3]
  sprite.src_height = sprite_metadata[self.direction][self.animation_stage][4]
  sprite.dst_top = self.rect.top
  sprite.dst_left = self.rect.left
  sprite.dst_width = self.rect.width
  sprite.dst_height = self.rect.height

  drawSprite(sprite)
end

return Character;

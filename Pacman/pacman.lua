print("Pacman game in Lua")

-- Load map file and draw the whole thing
function load_map(filename)
  local f = assert(io.open(filename, "r"))
  while true do
    local line = f:read("*line")
    if line == nil then break end
    --print("line: " .. line)
  end
  f:close()
end

load_map("map.txt")

drawRect{top=5, left=24, width=53, height=79}
drawRect{top=15, left=54, width=5, height=79}
drawRect{top=45, left=24, width=3, height=75}

while true do
  local events = getKey()
  if events ~= nil then
    print("Event: " .. events)
  end
end

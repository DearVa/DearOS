fillScreen(0)
local i = 0
while (true) do
    i++
    if (i == 128) then
        i = 0
    end
    drawPixel(20, i, 65535)
end
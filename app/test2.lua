local i = 0
while (true) do
    fillCircle(50, i, 5, 0)
    i = i + 1
    if (i == 128) then
        i = 0
    end
    fillCircle(50, i, 5, 65535)
end
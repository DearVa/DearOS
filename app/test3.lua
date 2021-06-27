local i = 0
while (true) do
    fillCircle(90, i, 5, 0)
    i = i + 1
    if (i == 64) then
        break
    end
    fillCircle(90, i, 5, 65535)
end
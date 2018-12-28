print('main.lua', MyTest)

local a = 1;
print('a = ', a)
error("string expected")

MyTest = nil
collectgarbage() 
collectgarbage() 
print("after collectgarbage",MyTest)
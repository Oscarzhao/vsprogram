nGlobal = 10 -- 一个全局变量
strGlobal = "hello, I am in lua" -- 一个全局的字符串变量
-- 一个返回值为 int型的函数
function add(a, b)
	return a + b
end
-- 一个返回值为 string类型的函数
function strEcho(a)
	print(a)
	return "haha, i have print your input param"
end
testFunc()  -- 调用 C++ 暴露的一个测试函数
a = Apple()
print(a:getWeight())
a:eatApple(12)
t = {name="ettan", age=23, desc="花样年华"}
nGlobal = 10 -- һ��ȫ�ֱ���
strGlobal = "hello, I am in lua" -- һ��ȫ�ֵ��ַ�������
-- һ������ֵΪ int�͵ĺ���
function add(a, b)
	return a + b
end
-- һ������ֵΪ string���͵ĺ���
function strEcho(a)
	print(a)
	return "haha, i have print your input param"
end
testFunc()  -- ���� C++ ��¶��һ�����Ժ���
a = Apple()
print(a:getWeight())
a:eatApple(12)
t = {name="ettan", age=23, desc="�����껪"}
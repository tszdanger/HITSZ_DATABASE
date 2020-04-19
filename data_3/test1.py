





a = "select employee.essn from employee where employee.superssn in (select essn from employee where ename=%s)"


print(a%"'baba'")
message = 'baba'
print("'"+ message + "'")

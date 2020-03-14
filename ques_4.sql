# 4) 查询由翟蕾领导的员工的姓名，员工编号和居住地址；
SELECT ename,essn,address
FROM employee
WHERE employee.superssn in (SELECT essn FROM employee WHERE ename = '翟蕾');



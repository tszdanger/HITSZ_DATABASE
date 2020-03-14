# 2) 查询在“Produce Department”工作且姓“冯”的员工信息；

# SELECT *
# FROM employee,
#      department
# WHERE employee.ename like '冯%'
#   and employee.dno = department.dno
#   and department.dname = 'Produce Department';

SELECT *
FROM employee
WHERE employee.ename like '冯%'
  and employee.dno in (SELECT dno FROM department WHERE department.dname = 'Produce Department' );


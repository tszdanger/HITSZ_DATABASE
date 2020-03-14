# 6) 查询部门 Economy Department 的员工人数；

SELECT COUNT(*)
FROM employee
WHERE employee.dno in (SELECT dno FROM department WHERE department.dname='Economy Department')

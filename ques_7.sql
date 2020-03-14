# 7) 查询员工平均工资在 2000 元到 3000 元（包含 2000，不包含 3000）的部 门名称；


SELECT department.dname
FROM department,
     (SELECT employee.dno, AVG(salary) AS avg_salary
      FROM employee
      GROUP BY employee.dno
      ORDER BY avg_salary DESC) AS temple1
WHERE (temple1.avg_salary>=2000 and temple1.avg_salary<3000 and temple1.dno=department.dno);



# 5) 查询至少参加了项目编号为 P3 和 P4 的项目的员工编号
SELECT essn
FROM employee
WHERE essn in (SELECT e1.essn
       FROM works_on as e1,
            works_on as e2
       WHERE e1.essn = e2.essn
         and e1.pno = 'P3'
         and e2.pno = 'P4');

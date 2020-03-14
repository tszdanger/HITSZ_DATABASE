# 3) 查询没有参加项目所在地在黑龙江的项目的员工姓名；
SELECT employee.ename
FROM employee
WHERE employee.essn not in (SELECT works_on.essn
                            FROM project,
                                 works_on
                            WHERE project.plocation = '黑龙江'
                              and project.pno = works_on.pno
);


# 8) 查询总工时（人·时间）在 50 小时之上的工程项目名


SELECT project.pname
FROM project,
     (SELECT pno,
     SUM(hours) AS sumhour FROM works_on
GROUP BY pno) AS t1
WHERE project.pno=t1.pno and t1.sumhour>50;




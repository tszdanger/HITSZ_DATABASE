# 查询员工直接领导居住地在广州市的员工编号
SELECT e1.essn
FROM employee AS e1,
     employee AS e2
WHERE e1.superssn = e2.essn
  and e2.address LIKE '%广州%';
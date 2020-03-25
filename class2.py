import pymysql

class database():
    def __init__(self):
        self.db = pymysql.connect(host="localhost",user="root",password="123456",db="company",port=3306)
        self.cur= self.db.cursor()




def ask_need(num):
    if num==1:
        ENAME = input("请输入领导名字")
        return ENAME
    elif num==2:
        PLOCATION = input("请输入项目所在地")
        return PLOCATION
    elif num==3:
        PNAME = input("请输入项目名称")
        return PNAME
    elif num==4:
        ADDRESS = input("请输入领导地址")
        SALARY = input("请输入工资阈值")
        return ['%'+ADDRESS+'%',SALARY]
    elif num==5:
        PNO = input("请输入项目编号")
        return PNO
    elif num==6:
        MGRSTARTDATE = input("请输入时间xxxx-mm-dd")
        return MGRSTARTDATE
    elif num==7:
        HOURS = input("请输入小时数")
        return HOURS
    elif num==8:
        HOURS = input("请输入小时数")
        return HOURS
    elif num==9:
        HOURS = input("请输入小时数")
        N = input("请输入项目数")
        return [N,HOURS]
    elif num==10:
        data1  = input("请输入新的employee1:ename,essn,address,salary,superssn,dno")
        data1 = data1.split(",")
        data2  = input("请输入新的employee2:ename,essn,address,salary,superssn,dno")
        data2 = data2.split(",")
        data =[tuple(data1),tuple(data2)]
        return data
    elif num==11:
        return
    elif num==12:
        return
    else:
        print("别输那些乱七八糟的")


def sql_sen(num):
    ss = ["select employee.essn from employee where employee.superssn in (select essn from employee where ename=%s)",\
        "select department.dname from department where department.dno in (select project.dno from project where project.plocation=%s)",\
        "select employee.ename, employee.address from employee where employee.essn in (select works_on.essn from works_on where works_on.pno in (select project.pno from project where project.pname =%s))",\
        "select employee.ename, employee.address from employee where employee.superssn in (select employee.essn from employee where employee.address like %s) and employee.salary >= %s",\
        "select  employee.ename from employee where employee.essn not in (select works_on.essn from works_on where works_on.pno =%s)",\
        "select department.dname from department where department.mgrstartdate>%s",\
        "select project.pname from project where project.pno in (select works_on.pno from works_on group by pno having sum(works_on.hours)>%s)",\
         "select project.pname from project where project.pno in (select works_on.pno from works_on group by works_on.pno having sum(works_on.hours)/count(*)<%s)",\
        "select employee.ename from employee where (employee.essn in (select works_on.essn from works_on group by works_on.essn having count(*)>%s and sum(works_on.hours)>%s) )",\
        "insert into employee(ename,essn,address,salary,superssn,dno) values (%s,%s,%s,%s,%s,%s);",\
         "update employee set address='深圳市南山区西丽大学城哈工大（深圳）' where employee.essn='251'",\
          "delete from employee where employee.essn='250'"

                     ]
    return ss[num-1]

db = pymysql.connect(host="localhost", user="root", password="123456", db="company", port=3306)
cur = db.cursor()

def main():
    while (1):
        print("1. 查询直接领导为%ENAME%的员工编号")
        print("2. 查询项目所在地为%PLOCATION%的部门名称；")
        print("3. 查询参与%PNAME%项目的所有工作人员的名字和居住地址")
        print("4. 查询部门领导居住地在%ADDRESS%且工资不低于%SALARY%元的员工姓名和居住地")
        print("5. 查询没有参加项目编号为%PNO%的项目的员工姓名")
        print("6. 查询部门领导工作日期在%MGRSTARTDATE%之后的部门名")
        print("7. 查询总工作量大于%HOURS%小时的项目名称")
        print("8. 查询员工平均工作时间低于%HOURS%的项目名称")
        print("9. 查询至少参与了%N%个项目并且工作总时间超过%HOURS%小时的员工名字")
        print("10.在employee表新增记录2条记录")
        print("11.将第10步新增的其中1条记录的地址改成“深圳市南山区西丽大学城哈工大（深圳）”")
        print("12.将第10步新增的2条记录中没有修改的那条记录删除")
        choice = int(input("输入你的选择(输入数字)"))

        try:
            if choice==10:
                cur.executemany(sql_sen(choice),ask_need(choice))
                db.commit()
            elif choice==11 or 12:
                cur.execute(sql_sen(choice))
                db.commit()

            else:
                cur.execute(sql_sen(choice),ask_need(choice))
            result = cur.fetchall()
            for i in result:
                print(i)



        except Exception as e:
            print("failed:",e)






if __name__ == '__main__':
    main()
    cur.close()
    db.close()










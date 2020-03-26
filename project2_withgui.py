import pymysql
import wx


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


def OnCloseMe(self):
    dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", u"默认内容")
    if dlg.ShowModal() == wx.ID_OK:
        message = dlg.GetValue()  # 获取文本框中输入的值
        dlg_tip = wx.MessageDialog(None, message, u"标题信息", wx.OK | wx.ICON_INFORMATION)
        if dlg_tip.ShowModal() == wx.ID_OK:
            self.Close(True)
        dlg_tip.Destroy()
    dlg.Destroy()

db = pymysql.connect(host="localhost", user="root", password="123456", db="company", port=3306)
cur = db.cursor()

app = wx.App()
frame = wx.Frame(None, title="Gui Test Editor", pos=(1000, 200), size=(500, 400))
text = wx.StaticText(frame, wx.ID_ANY, "查询直接领导为%ENAME%的员工编号", (0,7 ), (250,250), wx.ALIGN_LEFT)
# 1. 查询直接领导为%ENAME%的员工编号
basicText = wx.TextCtrl(frame, -1, "请输入员工编号",  pos=(230,5) , size=(125, -1))

button1 = wx.Button(frame, label="查询", pos=(370, 5), size=(50, 24))
button1.Bind(wx.EVT_BUTTON,ask_need)

button = wx.Button(frame, label=u'关闭', pos=(250, 20), size=(100, 60))
button.Bind(wx.EVT_BUTTON,OnCloseMe)
# frame.Bind(wx.EVT_BUTTON, OnCloseMe, button)



# path_text = wx.TextCtrl(frame, pos=(5, 5), size=(350, 24))
# open_button = wx.Button(frame, label="打开", pos=(370, 5), size=(50, 24))
# open_button.Bind(wx.EVT_BUTTON, openfile)  # 绑定打开文件事件到open_button按钮上
#
# save_button = wx.Button(frame, label="保存", pos=(430, 5), size=(50, 24))
#
# content_text = wx.TextCtrl(frame, pos=(5, 39), size=(475, 300), style=wx.TE_MULTILINE)
#  wx.TE_MULTILINE可以实现以滚动条方式多行显示文本,若不加此功能文本文档显示为一行

frame.Show()
app.MainLoop()

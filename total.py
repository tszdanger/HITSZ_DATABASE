import pymysql
import wx

sentences = ["1. 查询直接领导为%ENAME%的员工编号",\
"2. 查询项目所在地为%PLOCATION%的部门名称；",\
"3. 查询参与%PNAME%项目的所有工作人员的名字和居住地址",\
"4. 查询部门领导居住地在%ADDRESS%且工资不低于%SALARY%元的员工姓名和居住地",\
"5. 查询没有参加项目编号为%PNO%的项目的员工姓名",\
"6. 查询部门领导工作日期在%MGRSTARTDATE%之后的部门名",\
"7. 查询总工作量大于%HOURS%小时的项目名称",\
"8. 查询员工平均工作时间低于%HOURS%的项目名称",\
"9. 查询至少参与了%N%个项目并且工作总时间超过%HOURS%小时的员工名字",\
"10.在employee表新增记录2条记录",\
"11.将第10步新增的其中1条记录的地址改成“深圳市南山区西丽大学城哈工大（深圳）”",\
"12.将第10步新增的2条记录中没有修改的那条记录删除",]

db = pymysql.connect(host="localhost", user="root", password="123456", db="company", port=3306)
cur = db.cursor()

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




class MyFrame(wx.Frame):

    def __init__(self, parent, id):
        wx.Frame.__init__(self, parent, id, u'测试面板Panel', size=(600, 500))
        # 创建面板
        panel = wx.Panel(self)
        text1 = wx.StaticText(panel, wx.ID_ANY, sentences[0], (0, 5), (400, 25), wx.ALIGN_LEFT)
        # basicText = wx.TextCtrl(panel, -1, "请输入员工编号", pos=(230, 5), size=(125, -1))
        button1 = wx.Button(panel, label="查询", pos=(530, 5), size=(50, 24))
        button1.Bind(wx.EVT_BUTTON, self.OnCloseMe1)

        text2 = wx.StaticText(panel, wx.ID_ANY, sentences[1], (0, 30), (400, 25), wx.ALIGN_LEFT)
        button2 = wx.Button(panel, label="查询", pos=(530, 30), size=(50, 24))
        button2.Bind(wx.EVT_BUTTON, self.OnCloseMe2)

        text3 = wx.StaticText(panel, wx.ID_ANY, sentences[2], (0, 55), (400, 25), wx.ALIGN_LEFT)
        button3 = wx.Button(panel, label="查询", pos=(530, 55), size=(50, 24))
        button3.Bind(wx.EVT_BUTTON, self.OnCloseMe3)

        text4 = wx.StaticText(panel, wx.ID_ANY, sentences[3], (0, 80), (500, 25), wx.ALIGN_LEFT)
        button4 = wx.Button(panel, label="查询", pos=(530, 80), size=(50, 24))
        button4.Bind(wx.EVT_BUTTON, self.OnCloseMe4)

        text5 = wx.StaticText(panel, wx.ID_ANY, sentences[4], (0, 105), (400, 25), wx.ALIGN_LEFT)
        button5 = wx.Button(panel, label="查询", pos=(530, 105), size=(50, 24))
        button5.Bind(wx.EVT_BUTTON, self.OnCloseMe5)

        text6 = wx.StaticText(panel, wx.ID_ANY, sentences[5], (0, 130), (400, 25), wx.ALIGN_LEFT)
        button6 = wx.Button(panel, label="查询", pos=(530, 130), size=(50, 24))
        button6.Bind(wx.EVT_BUTTON, self.OnCloseMe6)

        text7 = wx.StaticText(panel, wx.ID_ANY, sentences[6], (0, 155), (400, 25), wx.ALIGN_LEFT)
        button7 = wx.Button(panel, label="查询", pos=(530, 155), size=(50, 24))
        button7.Bind(wx.EVT_BUTTON, self.OnCloseMe7)

        text8 = wx.StaticText(panel, wx.ID_ANY, sentences[7], (0, 180), (400, 25), wx.ALIGN_LEFT)
        button8 = wx.Button(panel, label="查询", pos=(530, 180), size=(50, 24))
        button8.Bind(wx.EVT_BUTTON, self.OnCloseMe8)

        text9 = wx.StaticText(panel, wx.ID_ANY, sentences[8], (0, 205), (450, 25), wx.ALIGN_LEFT)
        button9 = wx.Button(panel, label="查询", pos=(530, 205), size=(50, 24))
        button9.Bind(wx.EVT_BUTTON, self.OnCloseMe9)

        text10 = wx.StaticText(panel, wx.ID_ANY, sentences[9], (0, 230), (400, 25), wx.ALIGN_LEFT)
        button10 = wx.Button(panel, label="增", pos=(530, 230), size=(50, 24))
        button10.Bind(wx.EVT_BUTTON, self.OnCloseMe10)

        text11 = wx.StaticText(panel, wx.ID_ANY, sentences[10], (0, 255), (500, 25), wx.ALIGN_LEFT)
        button11 = wx.Button(panel, label="改", pos=(530, 255), size=(50, 24))
        button11.Bind(wx.EVT_BUTTON, self.OnCloseMe11)

        text12 = wx.StaticText(panel, wx.ID_ANY, sentences[11], (0, 280), (400, 25), wx.ALIGN_LEFT)
        button12 = wx.Button(panel, label="删", pos=(530, 280), size=(50, 24))
        button12.Bind(wx.EVT_BUTTON, self.OnCloseMe12)



    # 在这里event是必要的
    def OnCloseMe1(self,event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", u"查询直接领导为%ENAME%的员工编号")
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            try:
                 cur.execute(sql_sen(1),message)
                 result = cur.fetchall()
                 for i in result:
                     print(i)

            except Exception as e:
                print("failed: ",e)
            message1 = "'"+ message + "'"
            temp= ['查询语句是'+sql_sen(1)%message1+'操作结果是']
            for i in range(len(result)):
                temp += result[i]
            dlg_tip = wx.MessageDialog(None, str(temp), u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()
# 在这里event是必要的
    def OnCloseMe2(self,event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", u"查询项目所在地为%PLOCATION%的部门名称")
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            try:
                 cur.execute(sql_sen(2),message)
                 result = cur.fetchall()
                 for i in result:
                     print(i)

            except Exception as e:
                print("failed: ",e)
            message1 = "'" + message + "'"
            temp = ['查询语句是' + sql_sen(2) % message1 + '操作结果是']
            for i in range(len(result)):
                temp += result[i]
            dlg_tip = wx.MessageDialog(None, str(temp), u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()

    def OnCloseMe3(self,event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", sentences[2])
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            try:
                 cur.execute(sql_sen(3),message)
                 result = cur.fetchall()
                 for i in result:
                     print(i)

            except Exception as e:
                print("failed: ",e)
            message1 = "'" + message + "'"
            temp = ['查询语句是' + sql_sen(3) % message1 + '操作结果是']
            for i in range(len(result)):
                temp += result[i]
            dlg_tip = wx.MessageDialog(None, str(temp), u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()

    def OnCloseMe4(self,event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", sentences[3])
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            a = message.split(',')
            ADDRESS = a[0]
            SALARY = a[1]
            message = ['%' + ADDRESS + '%', SALARY]
            try:
                 cur.execute(sql_sen(4),message)
                 result = cur.fetchall()
                 for i in result:
                     print(i)

            except Exception as e:
                print("failed: ",e)
            message1=[]
            message1.append( "'" + message[0] + "'")
            message1.append( "'" + message[1] + "'")
            temp = ['查询语句是' + sql_sen(4) % (message1[0],message1[1]) + '操作结果是']
            for i in range(len(result)):
                temp += result[i]
            dlg_tip = wx.MessageDialog(None, str(temp), u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()

    def OnCloseMe5(self,event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", sentences[4])
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            try:
                 cur.execute(sql_sen(5),message)
                 result = cur.fetchall()
                 for i in result:
                     print(i)

            except Exception as e:
                print("failed: ",e)
            message1 = "'" + message + "'"
            temp = ['查询语句是' + sql_sen(5) % message1 + '操作结果是']
            for i in range(len(result)):
                temp += result[i]
            dlg_tip = wx.MessageDialog(None, str(temp), u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()

    def OnCloseMe6(self, event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", sentences[5])
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            try:
                cur.execute(sql_sen(6), message)
                result = cur.fetchall()
                for i in result:
                    print(i)
            except Exception as e:
                print("failed: ", e)
            message1 = "'" + message + "'"
            temp = ['查询语句是' + sql_sen(6) % message1 + '操作结果是']
            for i in range(len(result)):
                temp += result[i]
            dlg_tip = wx.MessageDialog(None, str(temp), u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()
    def OnCloseMe7(self, event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", sentences[6])
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            try:
                cur.execute(sql_sen(7), message)
                result = cur.fetchall()
                for i in result:
                    print(i)
            except Exception as e:
                print("failed: ", e)
            message1 = "'" + message + "'"
            temp = ['查询语句是' + sql_sen(7) % message1 + '操作结果是']
            for i in range(len(result)):
                temp += result[i]
            dlg_tip = wx.MessageDialog(None, str(temp), u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()
    def OnCloseMe8(self,event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", sentences[7])
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            try:
                 cur.execute(sql_sen(8),message)
                 result = cur.fetchall()
                 for i in result:
                     print(i)

            except Exception as e:
                print("failed: ",e)
            message1 = "'" + message + "'"
            temp = ['查询语句是' + sql_sen(8) % message1 + '操作结果是']
            for i in range(len(result)):
                temp += result[i]
            dlg_tip = wx.MessageDialog(None, str(temp), u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()
    def OnCloseMe9(self,event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", sentences[8])
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            a = message.split(',')
            N = a[0]
            HOURS = a[1]
            message = [N, HOURS]
            try:
                 cur.execute(sql_sen(9),message)
                 result = cur.fetchall()
                 for i in result:
                     print(i)

            except Exception as e:
                print("failed: ",e)
            message1 = []
            message1.append("'" + message[0] + "'")
            message1.append("'" + message[1] + "'")
            temp = ['查询语句是' + sql_sen(9) % (message1[0], message1[1]) + '操作结果是']
            for i in range(len(result)):
                temp += result[i]
            dlg_tip = wx.MessageDialog(None, str(temp), u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()
    def OnCloseMe10(self,event):
        dlg = wx.TextEntryDialog(None, u"请输入新的employee1:ename,essn,address,salary,superssn,dno:", u"文本输入框标题", sentences[9])

        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            data = message.split(",")
            data1 = data[0:6]
            data2 = data[6:12]
            message2 = message
            message = [tuple(data1),tuple(data2)]

            try:
                 cur.execute(sql_sen(10),message)
                 # db.commit()
                 result = cur.fetchall()
                 for i in result:
                     print(i)

            except Exception as e:
                print("failed: ",e)
            message1 = []
            for i in range(6):
                message1.append("'" + data1[i] + "'")
            for i in range(6):
                message1.append("'" + data2[i] + "'")
            temp = ['语句是' + sql_sen(10) % (message1[0], message1[1],message1[2], message1[3],message1[4], message1[5]) ]
            temp += ['语句是' + sql_sen(10) % (message1[6], message1[7],message1[8], message1[9],message1[10], message1[11]) + '操作完成']

            # for i in range(len(result)):
            #     temp += result[i]
            dlg_tip = wx.MessageDialog(None, str(temp), u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()

    def OnCloseMe11(self,event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", sentences[10])
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            try:
                 cur.execute(sql_sen(11))
                 # db.commit()
                 result = cur.fetchall()
                 for i in result:
                     print(i)

            except Exception as e:
                print("failed: ",e)
            temp = "语句是"+sql_sen(11)+"操作完成了"
            dlg_tip = wx.MessageDialog(None, temp, u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()
    def OnCloseMe12(self,event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", sentences[11])
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            try:
                 cur.execute(sql_sen(12))
                 # db.commit()
                 result = cur.fetchall()
                 for i in result:
                     print(i)

            except Exception as e:
                print("failed: ",e)
            temp = "语句是" + sql_sen(11) + "操作完成了"
            dlg_tip = wx.MessageDialog(None, temp, u"操作结果", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                # self.Close(True)
                pass
            dlg_tip.Destroy()
        dlg.Destroy()


if __name__ == '__main__':
    app = wx.App()
    frame = MyFrame(parent=None, id=-1)




    frame.Show()
    app.MainLoop()
    cur.close()
    db.close()



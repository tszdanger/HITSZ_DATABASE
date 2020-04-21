import pymysql
import sys
from PyQt5.QtCore import *
from PyQt5.QtWidgets import QMainWindow, QWidget, QApplication, QGroupBox, QPushButton, QLabel, QHBoxLayout, \
    QVBoxLayout, QGridLayout, QFormLayout, QLineEdit, QTextEdit
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtWebEngineWidgets import *
from PyQt5.QtWidgets import QMessageBox
import datetime
import re
import requests
import json
import math
from pyecharts.charts import Map
from pyecharts import options as opts


def decimal_to_int(strtemp):
    return re.findall(r'\d+', strtemp)


# 首先完成数据库连接、用户登陆功能
# 1. 往用户信息表里面加数据
# 2.检查用户
# 3. 往用户密码表里加数据
# 4. 往用户钱表里加数据
# 5. 查询对应id的密码
# 6.输入日期动态查询
# 7.查询视图
# 8.查询指定日期的情况
# 9.10.11依次是汽车飞机和火车
# 12 增加一个患者
# 13.更新患者状态
# 14.增加患者交通工具记录/选择患者id
# 15/16/17增加汽车飞机火车的联系
# 18/19/20 增加汽车飞机火车的班次信息
# 21更新钱财
# 22显示全部
# 232425 删除某个用户
def sql_sen(num):
    ss = ["insert into user_info(user_name, reg_date, user_type)values (%s,%s,%s);", \
          "select user_info.user_name,user_info.user_id,user_info.user_type from user_info where user_name=%s;", \
          "insert into user_pass(user_id, password) VALUES (%s,%s);", \
          "insert into user_limit(user_id,user_type, money_left) VALUES(%s,%s,%s);", \
          "select user_pass.password  from user_pass where user_id=%s;", \
          "select sum(acc_confirm),sum(cursed),sum(dead),sum(exist_confirm),sum(no_outfact),sum(output) from(select b.country_id,a.acc_confirm,a.cursed,a.dead,a.exist_confirm,a.no_outfact,a.output from province_statement as a ,province as b where a.date=%s and a.province_id=b.province_id) as temp group by country_id;", \
          "select * from check_patloc;", \
          "select province_name,acc_confirm from province_statement as a,province as b where a.province_id=b.province_id and date=%s;", \
          "select bus_id from bus where bus_code=%s and bus_date_time=%s;", \
          "select flight_id from flight where flight_code=%s and flight_datenumber=%s;", \
          "select train_id from train where train_code=%s and train_date_time=%s;", \
          "insert into patient(patient_name, patient_state, patient_date, patient_loc, patient_postcode) VALUES (%s,%s,%s,%s,%s);", \
          "update patient set patient.patient_state=%s where patient.patient_name=%s and patient.patient_date=%s;", \
          "select patient_id from patient where patient_name=%s and patient_date=%s;",\
          "insert into bus_patient(bus_id, patient_id) VALUES (%s,%s);",\
          "insert into patient_flight(patient_id, flight_id) VALUES (%s,%s);",\
          "insert into patient_train(patient_id, train_id) VALUES (%s,%s);",\
          "insert into bus(bus_code, bus_date_time) VALUES (%s,%s);",\
          "insert into flight(flight_code, flight_datenumber) VALUES (%s,%s);",\
          "insert into train(train_code, train_date_time) VALUES (%s,%s);",\
          "update user_limit set user_limit.money_left=%s where user_id=%s;",\
          "select user_info.user_id,user_name,reg_date,password from user_info inner join user_pass up on user_info.user_id = up.user_id;",\
          "delete from user_pass where user_id=%s;",\
          "delete from user_limit where user_id=%s;",\
          "delete from user_info where  user_id=%s;",\
          ]
    return ss[num - 1]


class WAIBU(QMainWindow):
    def __init__(self):
        super(WAIBU, self).__init__()
        self.setWindowTitle('实时疫情地图')
        self.setGeometry(500, 500, 800, 800)
        self.browser = QWebEngineView()
        # 加载外部的web界面
        self.browser.load(QUrl('C:/Users/skywf/Documents/HITSZ_DATABASE/yiqing/yiqing.html'))
        self.setCentralWidget(self.browser)


class UPPAT(QWidget):
    def __init__(self):
        super(UPPAT, self).__init__()
        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('更新患者状态')
        self.setGeometry(600, 600, 600, 800)

        nameLabel = QLabel("患者名")
        self.nameLineEdit = QLineEdit("张三")

        self.rb11 = QRadioButton('疑似', self)
        self.rb12 = QRadioButton('确诊', self)
        self.rb13 = QRadioButton('死亡', self)
        self.rb21 = QRadioButton('治愈', self)

        dateLabel = QLabel("患者确诊日期")
        self.dateLineEdit = QLineEdit("2020-04-01")

        # layout.setSpacing(10)
        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)
        layout.addWidget(self.rb11, 2, 0)
        layout.addWidget(self.rb12, 2, 1)
        layout.addWidget(self.rb13, 3, 0)
        layout.addWidget(self.rb21, 3, 1)
        layout.addWidget(dateLabel, 4, 0)
        layout.addWidget(self.dateLineEdit, 4, 1)

        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('保存')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.uppatient)
        save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def uppatient(self):
        print(1)
        patient_state = -1
        flag = 0
        name = self.nameLineEdit.text()  # 获取文本框内容
        date = self.dateLineEdit.text()
        if self.rb11.isChecked():
            patient_state = 0
        elif self.rb12.isChecked():
            patient_state = 1
        elif self.rb13.isChecked():
            patient_state = 2
        elif self.rb21.isChecked():
            patient_state = 3
        # print('patient_state is {}'.format(patient_state))
        name = name.strip()
        date = date.strip()
        message = [str(patient_state), name, date]
        if patient_state not in [0, 1, 2, 3]:
            flag = 1
        if flag == 0:
            try:
                cur.execute(sql_sen(13), message)
                db.commit()
                result = cur.fetchall()

            except Exception as e:
                print("failed: ", e)
        if flag == 0:
            self.box = QMessageBox(QMessageBox.Warning, "恭喜", "您更新了一名患者！")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.show()
        elif flag == 1:
            self.box = QMessageBox(QMessageBox.Warning, "orz", "对不起您输入的信息有误")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.show()


class ADDPA(QWidget):
    def __init__(self):
        super(ADDPA, self).__init__()
        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('增加患者')
        self.setGeometry(600, 600, 600, 800)

        nameLabel = QLabel("患者名")
        self.nameLineEdit = QLineEdit(" ")

        self.rb11 = QRadioButton('疑似', self)
        self.rb12 = QRadioButton('确诊', self)
        self.rb13 = QRadioButton('死亡', self)
        self.rb21 = QRadioButton('治愈', self)

        dateLabel = QLabel("当前日期")
        self.dateLineEdit = QLineEdit("2020-04-01")

        provinceLabel = QLabel("当前所处省份")
        self.proLineEdit = QLineEdit("四川")
        postLabel = QLabel("邮编")
        self.postLineEdit = QLineEdit("610000")
        # layout.setSpacing(10)
        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)
        layout.addWidget(self.rb11, 2, 0)
        layout.addWidget(self.rb12, 2, 1)
        layout.addWidget(self.rb13, 3, 0)
        layout.addWidget(self.rb21, 3, 1)
        layout.addWidget(dateLabel, 4, 0)
        layout.addWidget(self.dateLineEdit, 4, 1)
        layout.addWidget(provinceLabel, 5, 0)
        layout.addWidget(self.proLineEdit, 5, 1)
        layout.addWidget(postLabel, 6, 0)
        layout.addWidget(self.postLineEdit, 6, 1)

        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('保存')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.addpatient)
        save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def name_to_id(self, name):
        prolist = ['四川',
                   '重庆',
                   '广东',
                   '山东',
                   '河南',
                   '江苏',
                   '河北',
                   '湖南',
                   '安徽',
                   '湖北',
                   '浙江',
                   '广西',
                   '云南',
                   '江西',
                   '辽宁',
                   '福建',
                   '黑龙',
                   '陕西',
                   '山西',
                   '贵州',
                   '吉林',
                   '甘肃',
                   '内蒙',
                   '上海',
                   '新疆',
                   '北京',
                   '天津',
                   '海南',
                   '宁夏',
                   '青海',
                   '西藏'
                   ]
        if name in prolist:
            return str(prolist.index(name) + 1)
        else:
            return '0'

    def addpatient(self):
        flag = 0  # 0是正常
        patient_state = -1
        print(1)
        name = self.nameLineEdit.text()  # 获取文本框内容
        date = self.dateLineEdit.text()
        if self.rb11.isChecked():
            patient_state = 0
        elif self.rb12.isChecked():
            patient_state = 1
        elif self.rb13.isChecked():
            patient_state = 2
        elif self.rb21.isChecked():
            patient_state = 3
        # print('patient_state is {}'.format(patient_state))
        pro = self.proLineEdit.text()
        post1 = self.postLineEdit.text()

        name = name.strip()
        date = date.strip()
        pro = pro.strip()
        # proid返回0表示不对劲
        proid = self.name_to_id(pro)
        if proid == '0':
            flag = 1
        post1 = post1.strip()
        message = [name, str(patient_state), date, proid, post1]
        if flag == 0:
            try:
                cur.execute(sql_sen(12), message)
                db.commit()
                result = cur.fetchall()

            except Exception as e:
                print("failed: ", e)

        if flag == 0:
            self.box = QMessageBox(QMessageBox.Warning, "恭喜", "您已经加入了一名患者！")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.show()
        elif flag == 1 or proid == '0':
            self.box = QMessageBox(QMessageBox.Warning, "orz", "对不起您输入的信息有误")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.show()


class First(QMainWindow):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.btn_register = QPushButton("用户注册", self)
        self.btn_register.move(30, 50)
        self.btn_signin = QPushButton("用户登陆", self)
        self.btn_signin.move(150, 50)

        self.setGeometry(300, 300, 500, 500)
        self.setWindowTitle('yiqing_system')
        self.show()

class REGISTER1(QWidget):
    def __init__(self):
        super(REGISTER1, self).__init__()
        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('增加用户')
        self.setGeometry(600, 600, 400, 400)

        nameLabel = QLabel("用户名")
        self.nameLineEdit = QLineEdit(" ")
        passLabel = QLabel("密码")
        self.passLineEdit = QLineEdit(" ")

        # layout.setSpacing(10)
        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)
        layout.addWidget(passLabel, 2, 0)
        layout.addWidget(self.passLineEdit, 2, 1)

        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('保存')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.addNum)
        save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def addNum(self):
        name = self.nameLineEdit.text()  # 获取文本框内容
        password = self.passLineEdit.text()
        name = name.strip()
        password = password.strip()
        flag = 0
        id_temp = -1
        print('账户名: %s 密码: %s  ' % (name, password))
        message = [name, datetime.datetime.now().strftime('%Y-%m-%d'), '0']

        # 先看看有没有已有用户名
        try:
            cur.execute(sql_sen(2), name)
            result = cur.fetchall()
            if result != ():
                if list(list(result)[0])[0] == name:
                    print('对不起这个用户名已经被使用')
                    flag = 1
        except Exception as e:
            print("failed: ", e)

        if flag == 0:

            try:
                cur.execute(sql_sen(1), message)
                # 先往里面加，加了以后查询id
                cur.execute(sql_sen(2), name)
                result = cur.fetchall()
                id_temp = list(list(result)[0])[1]
                message_pass = [id_temp, password]
                message_limit = [id_temp, '0', '0']
                # 默认没钱
                cur.execute(sql_sen(3), message_pass)
                cur.execute(sql_sen(4), message_limit)

                db.commit()
                result = cur.fetchall()
                for i in result:
                    print(i)

            except Exception as e:
                print("failed: ", e)

        if flag == 0:
            self.box = QMessageBox(QMessageBox.Warning, "恭喜", "您成功增加了该名用户！")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            self.box.show()
        elif flag == 1:
            self.box = QMessageBox(QMessageBox.Warning, "orz", "对不起这个用户名已经被使用")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            self.box.show()

class REGISTER(QWidget):
    def __init__(self):
        super(REGISTER, self).__init__()
        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('用户注册')
        self.setGeometry(600, 600, 400, 400)

        nameLabel = QLabel("用户名(空格自动去除)")
        self.nameLineEdit = QLineEdit(" ")
        passLabel = QLabel("密码")
        self.passLineEdit = QLineEdit(" ")

        # layout.setSpacing(10)
        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)
        layout.addWidget(passLabel, 2, 0)
        layout.addWidget(self.passLineEdit, 2, 1)

        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('保存')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.addNum)
        save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def addNum(self):
        name = self.nameLineEdit.text()  # 获取文本框内容
        password = self.passLineEdit.text()
        name = name.strip()
        password = password.strip()
        flag = 0
        id_temp = -1
        print('账户名: %s 密码: %s  ' % (name, password))
        message = [name, datetime.datetime.now().strftime('%Y-%m-%d'), '0']

        # 先看看有没有已有用户名
        try:
            cur.execute(sql_sen(2), name)
            result = cur.fetchall()
            if result != ():
                if list(list(result)[0])[0] == name:
                    print('对不起您的用户名已经被使用')
                    flag = 1
        except Exception as e:
            print("failed: ", e)

        if flag == 0:

            try:
                cur.execute(sql_sen(1), message)
                # 先往里面加，加了以后查询id
                cur.execute(sql_sen(2), name)
                result = cur.fetchall()
                id_temp = list(list(result)[0])[1]
                message_pass = [id_temp, password]
                message_limit = [id_temp, '0', '0']
                # 默认没钱
                cur.execute(sql_sen(3), message_pass)
                cur.execute(sql_sen(4), message_limit)

                db.commit()
                result = cur.fetchall()
                for i in result:
                    print(i)

            except Exception as e:
                print("failed: ", e)

        if flag == 0:
            self.box = QMessageBox(QMessageBox.Warning, "恭喜", "您已经注册成功！")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            self.box.show()
        elif flag == 1:
            self.box = QMessageBox(QMessageBox.Warning, "orz", "对不起您的用户名已经被使用")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            self.box.show()


class SIGNIN(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('用户登陆')
        self.setGeometry(600, 600, 400, 400)

        nameLabel = QLabel("用户名(空格自动去除)")
        self.nameLineEdit = QLineEdit(" ")
        passLabel = QLabel("密码")
        self.passLineEdit = QLineEdit(" ")

        # layout.setSpacing(10)
        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)
        layout.addWidget(passLabel, 2, 0)
        layout.addWidget(self.passLineEdit, 2, 1)

        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('确定')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.checkin)
        save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def checkin(self):
        flag = 0
        id_temp = -1
        name = self.nameLineEdit.text()  # 获取文本框内容
        password = self.passLineEdit.text()
        name = name.strip()
        password = password.strip()
        print('账户名: %s 密码: %s  ' % (name, password))
        # 先验证是否登陆成功

        try:
            cur.execute(sql_sen(2), name)
            result = cur.fetchall()
            if result == ():
                print('对不起没有找到您')
            else:
                id_temp = list(list(result)[0])[1]
                cur.execute(sql_sen(5), id_temp)
                result1 = cur.fetchall()
                passget = list(list(result1)[0])[0]
                usertype = list(list(result)[0])[2]
                if passget == password and usertype==1: # 1是管理员
                    flag = 1
                    print('管理员登陆成功')
                if passget == password and usertype==0: # 1是管理员
                    flag = 2
                    print('普通用户登陆成功')
        except Exception as e:
            print("failed: ", e)

        if flag == 1:
            self.box = QMessageBox(QMessageBox.Warning, "恭喜", "您已经登陆成功！您是管理员")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            # self.box.show()
            reply = self.box.exec()
            if reply == 0:
                print('收到了')
                menu_page.show()
        elif flag == 2:
            self.box = QMessageBox(QMessageBox.Warning, "恭喜", "您已经登陆成功！")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            # self.box.show()
            reply = self.box.exec()
            if reply == 0:
                print('收到了')
                menu_strict_page.show()

        elif flag == 0:
            self.box = QMessageBox(QMessageBox.Warning, "orz", "对不起由于账号密码不匹配请重试")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            self.box.show()


class SHOW_INFO(QWidget):
    def __init__(self, temp_str):
        super().__init__()

        self.initUI(temp_str)

    def initUI(self, temp_str):
        self.setGeometry(600, 600, 400, 400)
        self.textbox = QLineEdit(temp_str)
        self.textbox.move(600, 600)
        self.textbox.resize(280, 40)
        self.show()


class DONGTAI(QWidget):
    def __init__(self):
        super(DONGTAI, self).__init__()

        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('动态查询')
        self.setGeometry(600, 600, 400, 400)

        nameLabel = QLabel("请选择查询日期")
        self.nameLineEdit = QLineEdit(" ")
        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)
        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('确定')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.zhanshi)
        # save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def zhanshi(self):
        print('展示')
        flag = 1
        date = self.nameLineEdit.text()  # 获取文本框内容
        date = date.strip()
        state_list = []
        try:
            cur.execute(sql_sen(6), date)
            result = cur.fetchall()
            if result == ():
                print('数据更新高峰，暂有延时')
                flag = 0

            else:
                # 确诊，治愈，死亡，现存，无症状，外输
                for i in range(6):
                    state_list.append(list(list(result)[0])[i])

        except Exception as e:
            print("failed: ", e)
        if flag == 1:
            sentence = '当前确诊' + str(state_list[0]) + '人，治愈' + str(state_list[1]) + '人，死亡' + str(
                state_list[2]) + '人，现存' + str(state_list[3]) + '人，无症状' + str(state_list[4]) + '人，外输' + str(
                state_list[5]) + '人'
            self.box = QMessageBox(QMessageBox.Warning, "查询情况", sentence)
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.exec_()

        elif flag == 0:
            self.box = QMessageBox(QMessageBox.Warning, "这是标题", "对不起由于数据更新，您所指定的日期暂无数据")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.show()


class GUIJI(QWidget):
    # 轨迹查询，也就是查询附近的确诊病例
    # 根据输入地址动态调用高德api，返回经纬度和邮编来确定
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('GUIJI查询')
        self.setGeometry(600, 600, 400, 400)

        nameLabel = QLabel("请输入您当前的地址（省份＋城市＋区县＋城镇＋乡村＋街道＋门牌号码）")
        self.nameLineEdit = QLineEdit("北京市昌平区科技园区昌盛路18号B1座1-5层")
        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)
        # layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('确定')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.zhanshi)
        save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def zhanshi(self):
        addr = self.nameLineEdit.text()  # 获取文本框内容
        addr = addr.strip()
        host = 'restapi.amap.com'
        # = '北京市昌平区科技园区昌盛路18号B1座1-5层
        addr_key = 'b3542a766f9d7d760d7665c725a0f115'  # 申请的key
        s = requests.session()
        _url = 'https://{}:443/v3/geocode/geo?address={}&output=JSON&key={}'.format(host, addr, addr_key)
        s.headers = {"Conetent-Type": "application/json;charset=UTF-8"}
        response = json.loads(requests.get(_url).text)
        status = response["status"]

        if status == '0' or response["geocodes"] == []:
            print('对不起您的地址可能有点问题')
            self.box = QMessageBox(QMessageBox.Warning, "查询情况", "对不起您的地址可能有点问题")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.exec_()
        elif status == '1':
            data = response["geocodes"][0]
            adcode = data["adcode"]
            print(adcode)
            cur.execute(sql_sen(7))
            result = cur.fetchall()
            print(result)
            far_list = []
            for i in range(len(result)):
                temp = list(list(result)[i])[1]
                acc = 0
                for i in range(6):
                    k = abs((int(temp[i]) - int(adcode[i])))
                    acc += k * math.pow(3, 5 - i)
                far_list.append(acc)
            far_list.sort()
            nearest0 = str(far_list[0])
            nearest1 = str(far_list[1])
            nearest2 = str(far_list[2])
            self.box = QMessageBox(QMessageBox.Warning, "查询情况",
                                   "当前距离您最近的患者分别有" + nearest0 + "公里、" + nearest1 + "公里、" + nearest2 + "公里")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.exec_()


class SAME_TRIP(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('同行程查询')
        self.setGeometry(600, 600, 400, 400)
        self.cb1 = QCheckBox('客车', self)
        self.cb2 = QCheckBox('飞机', self)
        self.cb3 = QCheckBox('火车', self)
        nameLabel = QLabel("请输入查询的班次号")
        self.nameLineEdit = QLineEdit("CD343")
        passLabel = QLabel("请输入乘车日期")
        self.passLineEdit = QLineEdit("2020-04-01")

        # layout.setSpacing(10)
        layout.addWidget(self.cb1, 1, 0)
        layout.addWidget(self.cb2, 2, 0)
        layout.addWidget(self.cb3, 3, 0)
        layout.addWidget(nameLabel, 4, 0)
        layout.addWidget(self.nameLineEdit, 4, 1)
        layout.addWidget(passLabel, 5, 0)
        layout.addWidget(self.passLineEdit, 5, 1)
        # self.cb1.stateChanged.connect(self.changecb1)
        # self.cb2.stateChanged.connect(self.changecb2)
        # self.cb3.stateChanged.connect(self.changecb3)
        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('确定')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.zhanshi)
        save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def zhanshi(self):
        print('展示')
        flag = 0
        isfound = 1
        banci = self.nameLineEdit.text()  # 获取文本框内容
        banci = banci.strip()
        date = self.passLineEdit.text()
        date = date.strip()
        message = [banci, date]
        if self.cb1.isChecked():
            print('1被按了')
            flag = 1
        elif self.cb2.isChecked():
            print('2')
            flag = 2
        elif self.cb3.isChecked():
            print('3')
            flag = 3
        try:
            cur.execute(sql_sen(flag + 8), message)
            result = cur.fetchall()
            if result == ():
                print('当日没有')
                isfound = 0
            else:
                print('当日有')
        except Exception as e:
            print("failed: ", e)
        if isfound == 0:
            self.box = QMessageBox(QMessageBox.Warning, "查询情况", "您查询的班次没有病患")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.exec_()
        elif isfound == 1:
            self.box = QMessageBox(QMessageBox.Warning, "查询情况", "您查询的班次有病患，请谨慎就医")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.exec_()


class FENBU(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('动态查询')
        self.setGeometry(600, 600, 800, 800)

        nameLabel = QLabel("请选择查询日期")
        self.nameLineEdit = QLineEdit("2020-04-01")
        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)
        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('确定')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.zhanshi)
        save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def zhanshi(self):
        flag = 1
        print('展示')
        date = self.nameLineEdit.text()  # 获取文本框内容2020-04-01
        date = date.strip()
        state_list = []
        try:
            cur.execute(sql_sen(8), date)
            result = cur.fetchall()
            if result == ():
                print('数据更新高峰，暂有延时')
                flag = 0

            else:
                # 确诊，治愈，死亡，现存，无症状，外输
                map_data = list(result)
                c = (
                    # 声明一个map对象
                    Map()
                        # 添加数据
                        .add("确诊", map_data, "china")
                        # 设置标题和颜色
                        .set_global_opts(title_opts=opts.TitleOpts(title="全国疫情图"),
                                         visualmap_opts=opts.VisualMapOpts(split_number=6, is_piecewise=True,
                                                                           pieces=[{"min": 1, "max": 9, "label": "1-9人",
                                                                                    "color": "#ffefd7"},
                                                                                   {"min": 10, "max": 99,
                                                                                    "label": "10-99人",
                                                                                    "color": "#ffd2a0"},
                                                                                   {"min": 100, "max": 499,
                                                                                    "label": "100-499人",
                                                                                    "color": "#fe8664"},
                                                                                   {"min": 500, "max": 999,
                                                                                    "label": "500-999人",
                                                                                    "color": "#e64b47"},
                                                                                   {"min": 1000, "max": 9999,
                                                                                    "label": "1000-9999人",
                                                                                    "color": "#c91014"},
                                                                                   {"min": 10000, "label": "10000人及以上",
                                                                                    "color": "#9c0a0d"}
                                                                                   ]))
                )
                # 生成html文件
                c.render("yiqing.html")


        except Exception as e:
            print("failed: ", e)
        if flag == 0:
            self.box = QMessageBox(QMessageBox.Warning, "这是标题", "对不起由于数据更新，您所指定的日期暂时没图")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.show()
        elif flag == 1:
            self.tupian = WAIBU()
            self.tupian.show()


class ZHUANGTAI(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('动态查询')
        self.setGeometry(600, 600, 400, 400)

        addpa_Btn = QPushButton('新增患者')
        updapa_Btn = QPushButton('更新患者状态')
        addtran_Btn = QPushButton('增加患者交通工具记录')

        layout.addWidget(addpa_Btn)
        layout.addWidget(updapa_Btn)
        layout.addWidget(addtran_Btn)

        addpa_Btn.clicked.connect(self.addpa)
        addpa_Btn.clicked.connect(self.close)
        updapa_Btn.clicked.connect(self.updpa)
        updapa_Btn.clicked.connect(self.close)
        addtran_Btn.clicked.connect(self.addtr)
        addtran_Btn.clicked.connect(self.close)

        self.setLayout(layout)

    def addpa(self):
        self.addpa_page = ADDPA()
        self.addpa_page.show()

    def updpa(self):
        print(1)
        self.up_page = UPPAT()
        self.up_page.show()

    def addtr(self):
        self.addtr_page = ADDTR()
        self.addtr_page.show()

class ADDTR(QWidget):
    def __init__(self):
        super(ADDTR, self).__init__()
        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('更新患者出行信息')
        self.setGeometry(600, 600, 600, 800)

        nameLabel = QLabel("患者名")
        self.nameLineEdit = QLineEdit("张三")
        wayLabel = QLabel("交通方式")
        self.rb11 = QRadioButton('汽车', self)
        self.rb12 = QRadioButton('火车', self)
        self.rb13 = QRadioButton('飞机', self)

        dateLabel = QLabel("患者确诊日期")
        self.dateLineEdit = QLineEdit("2020-04-01")
        banciLabel = QLabel("患者班次号")
        self.banciLineEdit = QLineEdit("KT222")

        # layout.setSpacing(10)
        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)
        layout.addWidget(wayLabel,3,0)
        layout.addWidget(self.rb11, 2, 1)
        layout.addWidget(self.rb12, 3, 1)
        layout.addWidget(self.rb13, 4, 1)
        layout.addWidget(banciLabel, 5, 0)
        layout.addWidget(self.banciLineEdit, 5, 1)
        layout.addWidget(dateLabel, 6, 0)
        layout.addWidget(self.dateLineEdit, 6, 1)

        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('保存')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.addtrs)
        save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def addtrs(self):
        print(1)
        trans_way = -1
        flag = 0
        name = self.nameLineEdit.text()  # 获取文本框内容
        date = self.dateLineEdit.text()
        banci = self.banciLineEdit.text()
        if self.rb11.isChecked():
            trans_way = 0
        elif self.rb12.isChecked():
            trans_way = 1
        elif self.rb13.isChecked():
            trans_way = 2

        name = name.strip()
        date = date.strip()
        banci = banci.strip()
        # 首先拿到患者id，然后看有没有航班id，没有的话新建,再拿到班次id
        if trans_way not in [0, 1, 2]:
            flag = 1
        if flag == 0:
            try:
                message = [name,date]
                cur.execute(sql_sen(14), message)
                # db.commit()
                result = cur.fetchall()
                pat_id = list(list(result)[0])[0]
                if pat_id == ():
                    flag=2 #2代表没找到患者

            except Exception as e:
                print("failed: ", e)
        if flag==0:# 汽车火车航班
            if trans_way==0:
                message = [banci,date]
                cur.execute(sql_sen(9),message)
                result = cur.fetchall()
                if result==():
                    flag=3
                    cur.execute(sql_sen(18),message)
                    db.commit()
                else:
                    banciid = list(list(result)[0])[0]
                    message = [pat_id,banciid]
                    cur.execute(sql_sen(15),message)
                    db.commit()
                    print('qiche')
            elif trans_way==1:
                message = [banci, date]
                cur.execute(sql_sen(11), message)
                result = cur.fetchall()
                if result==():
                    flag=3
                    cur.execute(sql_sen(20), message)
                    db.commit()
                else:
                    banciid = list(list(result)[0])[0]
                    message = [pat_id, banciid]
                    cur.execute(sql_sen(17), message)
                    db.commit()
                    print('huoche')
            elif trans_way==2:
                message = [banci, date]
                cur.execute(sql_sen(10), message)
                result = cur.fetchall()
                if result==():
                    flag=3 #3表示没找到航班号
                    cur.execute(sql_sen(19), message)
                    db.commit()
                else:
                    banciid = list(list(result)[0])[0]
                    message = [pat_id, banciid]
                    cur.execute(sql_sen(16), message)
                    db.commit()
                    print('feiji')
        # 若flag=3 需要先加入班次
        if flag==3:
            if trans_way==0:
                message = [banci,date]
                cur.execute(sql_sen(9),message)
                result = cur.fetchall()
                if result==():
                    print('不可能啊')
                else:
                    banciid = list(list(result)[0])[0]
                    message = [pat_id,banciid]
                    cur.execute(sql_sen(15),message)
                    db.commit()
                    print('qiche')
            elif trans_way==1:
                message = [banci, date]
                cur.execute(sql_sen(11), message)
                result = cur.fetchall()
                if result==():
                    print('不可能啊')
                else:
                    banciid = list(list(result)[0])[0]
                    message = [pat_id, banciid]
                    cur.execute(sql_sen(17), message)
                    db.commit()
                    print('huoche')
            elif trans_way==2:
                message = [banci, date]
                cur.execute(sql_sen(10), message)
                result = cur.fetchall()
                if result==():
                    print('不可能啊')
                else:
                    banciid = list(list(result)[0])[0]
                    message = [pat_id, banciid]
                    cur.execute(sql_sen(16), message)
                    db.commit()
                    print('feiji')

        if flag == 0:
            self.box = QMessageBox(QMessageBox.Warning, "恭喜", "您更新了一名患者的行程！")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.show()
        elif flag == 1:
            self.box = QMessageBox(QMessageBox.Warning, "orz", "对不起您输入的信息有误")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.show()
        elif flag == 2:
            self.box = QMessageBox(QMessageBox.Warning, "orz", "对不起没有找到您所说的患者")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.show()
        elif flag == 3:
            self.box = QMessageBox(QMessageBox.Warning, "恭喜", "您更新了一名患者的行程！")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            self.box.show()

class USER_CONTROL(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        self.setWindowTitle('用户管理')
        self.setGeometry(600, 600, 400, 400)

        btn_1 = QPushButton('查看当前所有用户')
        btn_2 = QPushButton('增加一名用户')
        btn_3 = QPushButton('修改用户余额')
        btn_4 = QPushButton('删除用户')
        layout = QGridLayout()
        layout.addWidget(btn_1, 1, 0)
        layout.addWidget(btn_2, 2, 0)
        layout.addWidget(btn_3, 3, 0)
        layout.addWidget(btn_4, 4, 0)
        self.setLayout(layout)
        btn_1.clicked.connect(self.aa)
        btn_2.clicked.connect(self.bb)
        btn_3.clicked.connect(self.cc)
        btn_4.clicked.connect(self.dd)

    def aa(self):
        print('展示')
        self.showall_page = SHOWALL()
        self.showall_page.show()
    def bb(self):
        self.reg_page = REGISTER1()
        self.reg_page.show()
    def cc(self):
        print(2)
        self.yue_page = YUE()
        self.yue_page.show()
    def dd(self):
        self.del_page = USERDEL()
        self.del_page.show()



class SHOWALL(QWidget):
    def __init__(self):
        super(SHOWALL,self).__init__()

        self.initUI()

    def initUI(self):
        self.setWindowTitle("所有用户")
        self.resize(1000, 800)
        try:
            cur.execute(sql_sen(22))
            result = cur.fetchall()
            print(result)

        except Exception as e:
            print("failed: ", e)

        layout = QHBoxLayout()
        print(len(result))
        TableWidget = QTableWidget(len(result), 4)
        TableWidget.setHorizontalHeaderLabels(['id', '用户名', '注册日期','密码'])
        # TableWidget.setEditTriggers(QAbstractItemView.NoEditTriggers)
        for i in range(len(result)):
            for j in range(4):
                temp = list(list(result)[i])[j]
                if type(temp)=='str':
                    newItem = QTableWidgetItem(temp)
                else:
                    newItem = QTableWidgetItem(str(temp))
                TableWidget.setItem(i,j,newItem)
        layout.addWidget(TableWidget)
        self.setLayout(layout)


class USERDEL(QWidget):
    def __init__(self):
        super(USERDEL,self).__init__()

        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('删除用户')
        self.setGeometry(600, 600, 800, 800)

        nameLabel = QLabel("请输入用户名")
        self.nameLineEdit = QLineEdit("test")

        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)


        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('确定')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.zhanshi)
        save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def zhanshi(self):
        name = self.nameLineEdit.text()  # 获取文本框内容
        name = name.strip()

        cur.execute(sql_sen(2),name)
        result = cur.fetchall()
        if result == ():
            print('对不起没有找到这个用户')
            self.box = QMessageBox(QMessageBox.Warning, "失败", "该用户并不存在！")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            self.box.show()
        else:
            id_temp = list(list(result)[0])[1]

            cur.execute(sql_sen(23),id_temp)
            db.commit()
            cur.execute(sql_sen(24),id_temp)
            db.commit()
            cur.execute(sql_sen(25),id_temp)
            db.commit()
            self.box = QMessageBox(QMessageBox.Warning, "成功", "您已经成功删除了这名用户！")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            self.box.show()







class YUE(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('修改用户余额')
        self.setGeometry(600, 600, 800, 800)

        nameLabel = QLabel("请输入用户名")
        self.nameLineEdit = QLineEdit("test")
        leftLabel = QLabel("请输入余额")
        self.yueLineEdit = QLineEdit('10')
        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)
        layout.addWidget(leftLabel, 2, 0)
        layout.addWidget(self.yueLineEdit, 2, 1)

        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('确定')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.zhanshi)
        save_Btn.clicked.connect(self.close)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def zhanshi(self):
        name = self.nameLineEdit.text()  # 获取文本框内容
        yue = self.yueLineEdit.text()
        name = name.strip()
        yue =yue.strip()

        cur.execute(sql_sen(2),name)
        result = cur.fetchall()
        if result == ():
            print('对不起没有找到这个用户')
        else:
            id_temp = list(list(result)[0])[1]
            message = [yue,id_temp]
            cur.execute(sql_sen(21),message)
            result = cur.fetchall()
            db.commit()
            self.box = QMessageBox(QMessageBox.Warning, "成功", "您已经成功修改余额！")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            self.box.show()

class MENU_STRICT(QWidget):
    def __init__(self):
        super(MENU_STRICT,self).__init__()

        self.initUI()

    def initUI(self):
        self.setGeometry(600, 600, 800, 800)
        self.setWindowTitle('疫情查询菜单')
        # 全国疫情动态查询、患者状态更新维护、同行程查询、病例分布、轨迹查询、用户管理
        btn_1 = QPushButton('全国疫情动态查询')
        btn_2 = QPushButton('轨迹查询')
        btn_3 = QPushButton('同行程查询')
        btn_4 = QPushButton('病例分布')
        layout = QGridLayout()
        layout.addWidget(btn_1, 1, 0)
        layout.addWidget(btn_2, 2, 0)
        layout.addWidget(btn_3, 3, 0)
        layout.addWidget(btn_4, 4, 0)
        self.setLayout(layout)
        btn_1.clicked.connect(self.dynamic_show)
        btn_2.clicked.connect(self.guiji_show)
        btn_3.clicked.connect(self.trip_show)
        btn_4.clicked.connect(self.fenbu)


    def dynamic_show(self):
        self.dongtai_page = DONGTAI()
        self.dongtai_page.show()

    def guiji_show(self):
        self.guiji_page = GUIJI()
        self.guiji_page.show()

    def trip_show(self):
        self.trip_page = SAME_TRIP()
        self.trip_page.show()

    def fenbu(self):
        self.fenbu_page = FENBU()
        self.fenbu_page.show()



class MENU(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        self.setGeometry(600, 600, 800, 800)
        self.setWindowTitle('疫情查询菜单')
        # 全国疫情动态查询、患者状态更新维护、同行程查询、病例分布、轨迹查询、用户管理
        btn_1 = QPushButton('全国疫情动态查询')
        btn_2 = QPushButton('轨迹查询')
        btn_3 = QPushButton('同行程查询')
        btn_4 = QPushButton('病例分布')
        btn_5 = QPushButton('患者状态更新维护')
        btn_6 = QPushButton('用户管理')
        layout = QGridLayout()
        layout.addWidget(btn_1, 1, 0)
        layout.addWidget(btn_2, 2, 0)
        layout.addWidget(btn_3, 3, 0)
        layout.addWidget(btn_4, 4, 0)
        layout.addWidget(btn_5, 5, 0)
        layout.addWidget(btn_6, 6, 0)
        self.setLayout(layout)
        btn_1.clicked.connect(self.dynamic_show)
        btn_2.clicked.connect(self.guiji_show)
        btn_3.clicked.connect(self.trip_show)
        btn_4.clicked.connect(self.fenbu)
        btn_5.clicked.connect(self.zhuangtai_show)
        btn_6.clicked.connect(self.usercon_show)

    def usercon_show(self):
        self.usercon_page = USER_CONTROL()
        self.usercon_page.show()

    def zhuangtai_show(self):
        self.zhuangtai_page = ZHUANGTAI()
        self.zhuangtai_page.show()

    def dynamic_show(self):
        self.dongtai_page = DONGTAI()
        self.dongtai_page.show()

    def guiji_show(self):
        self.guiji_page = GUIJI()
        self.guiji_page.show()

    def trip_show(self):
        self.trip_page = SAME_TRIP()
        self.trip_page.show()

    def fenbu(self):
        self.fenbu_page = FENBU()
        self.fenbu_page.show()




class CHANGE(QMainWindow):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        self.setGeometry(600, 600, 400, 400)
        self.setWindowTitle('改')


class CHECK(QWidget):
    def __init__(self):
        super(CHECK, self).__init__()
        self.initUI()

    def initUI(self):
        layout = QGridLayout()
        self.setWindowTitle('查')
        self.setGeometry(600, 600, 400, 400)

        nameLabel = QLabel("姓名")
        self.nameLineEdit = QLineEdit(" ")
        sexLabel = QLabel("性别")
        self.sexLineEdit = QLineEdit(" ")
        emitLabel = QLabel("手机号")
        self.phoneLineEdit = QLineEdit("")
        timeLabel = QLabel("邮箱")
        self.mailEdit = QLineEdit("")
        # layout.setSpacing(10)
        layout.addWidget(nameLabel, 1, 0)
        layout.addWidget(self.nameLineEdit, 1, 1)
        layout.addWidget(sexLabel, 2, 0)
        layout.addWidget(self.sexLineEdit, 2, 1)
        layout.addWidget(emitLabel, 3, 0)
        layout.addWidget(self.phoneLineEdit, 3, 1)
        layout.addWidget(timeLabel, 4, 0)
        layout.addWidget(self.mailEdit, 4, 1)
        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('保存')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(self.close)
        save_Btn.clicked.connect(self.addNum)
        layout.addWidget(save_Btn)
        layout.addWidget(cancle_Btn)
        self.setLayout(layout)

    def addNum(self):
        name = self.nameLineEdit.text()  # 获取文本框内容
        sex = self.sexLineEdit.text()
        phone = self.phoneLineEdit.text()
        mail = self.mailEdit.text()
        print('姓名: %s 性别: %s 手机号: %s 邮箱: %s ' % (name,
                                                 sex, phone, mail))


if __name__ == '__main__':
    db = pymysql.connect(host="localhost", user="root", password="123456", db="yiqing", port=3306)
    cur = db.cursor()
    app = QApplication(sys.argv)
    first_page = First()
    register_page = REGISTER()
    signin_page = SIGNIN()
    menu_page = MENU()
    menu_strict_page = MENU_STRICT()
    # d = CHANGE()
    # e = CHECK()
    first_page.show()

    first_page.btn_register.clicked.connect(register_page.show)
    first_page.btn_signin.clicked.connect(signin_page.show)
    first_page.btn_signin.clicked.connect(first_page.close)

    # menu_page.show()
    # a.btn_change.clicked.connect(d.show)
    # a.btn_check.clicked.connect(e.show)

    sys.exit(app.exec_())
    cur.close()
    db.close()

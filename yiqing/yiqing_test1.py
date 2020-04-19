import pymysql
import sys
from PyQt5.QtCore import *
from PyQt5.QtWidgets import QMainWindow, QWidget, QApplication, QGroupBox, QPushButton, QLabel, QHBoxLayout, \
    QVBoxLayout, QGridLayout, QFormLayout, QLineEdit, QTextEdit
from PyQt5.QtWidgets import QMessageBox
import datetime


# 首先完成数据库连接、用户登陆功能
# 1. 往用户信息表里面加数据
# 2.检查用户
# 3. 往用户密码表里加数据
# 4. 往用户钱表里加数据
# 5. 查询对应id的密码
def sql_sen(num):
    ss = ["insert into user_info(user_name, reg_date, user_type)values (%s,%s,%s);", \
          "select user_info.user_name,user_info.user_id from user_info where user_name=%s;", \
          "insert into user_pass(user_id, password) VALUES (%s,%s);", \
          "insert into user_limit(user_id,user_type, money_left) VALUES(%s,%s,%s);", \
          "select user_pass.password  from user_pass where user_id=%s;", \
          ]
    return ss[num - 1]


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
        flag=0
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
                cur.execute(sql_sen(5),id_temp)
                result1 = cur.fetchall()
                result1= list(list(result1)[0])[0]
                if result1 == password:
                    flag = 1
                    print('登陆成功')
        except Exception as e:
            print("failed: ", e)

        if flag == 1:
            self.box = QMessageBox(QMessageBox.Warning, "恭喜", "您已经登陆成功！")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            # self.box.show()
            reply = self.box.exec()
            if reply == 0:
                print('收到了')
                
        elif flag == 0:
            self.box = QMessageBox(QMessageBox.Warning, "orz", "对不起由于账号密码不匹配请重试")
            self.box.addButton(self.tr("确定"), QMessageBox.YesRole)
            # self.box.addButton(self.tr("取消"), QMessageBox.NoRole)
            self.box.show()


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

    # d = CHANGE()
    # e = CHECK()
    first_page.show()

    first_page.btn_register.clicked.connect(register_page.show)
    first_page.btn_signin.clicked.connect(signin_page.show)

    # a.btn_change.clicked.connect(d.show)
    # a.btn_check.clicked.connect(e.show)

    sys.exit(app.exec_())
    cur.close()
    db.close()

import sys
from PyQt5.QtCore import*
from PyQt5.QtWidgets import QMainWindow,QWidget, QApplication, QGroupBox, QPushButton, QLabel, QHBoxLayout,  QVBoxLayout, QGridLayout, QFormLayout, QLineEdit, QTextEdit





class First(QMainWindow):

    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        self.btn_add = QPushButton("增", self)
        self.btn_add.move(30, 50)
        self.btn_delete = QPushButton("删", self)
        self.btn_delete.move(150, 50)
        self.btn_change = QPushButton("改", self)
        self.btn_change.move(30, 150)
        self.btn_check = QPushButton("查", self)
        self.btn_check.move(150, 150)

        self.setGeometry(300, 300, 500, 500)
        self.setWindowTitle('Event sender')
        self.show()


class ADD(QMainWindow):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        self.setGeometry(600, 600, 400, 400)
        self.setWindowTitle('增')

class DELETE(QMainWindow):
    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):
        self.setGeometry(600, 600, 400, 400)
        self.setWindowTitle('删')

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
        save_Btn.clicked.connect(self.close)
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
    app = QApplication(sys.argv)
    a = First()

    b = ADD()
    c = DELETE()
    d = CHANGE()
    e = CHECK()
    a.show()

    a.btn_add.clicked.connect(b.show)
    a.btn_delete.clicked.connect(c.show)
    a.btn_change.clicked.connect(d.show)
    a.btn_check.clicked.connect(e.show)




    sys.exit(app.exec_())

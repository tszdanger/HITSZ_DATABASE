import sys
from PyQt5.QtCore import*
from PyQt5.QtWidgets import QWidget, QApplication, QGroupBox, QPushButton, QLabel, QHBoxLayout,  QVBoxLayout, QGridLayout, QFormLayout, QLineEdit, QTextEdit


class login(QWidget):
    def __init__(self):
        super(login,self).__init__()
        self.initUi()

    def initUi(self):
        self.setWindowTitle("login")
        layout = QGridLayout()
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
        layout.addWidget(nameLabel,1,0)
        layout.addWidget(self.nameLineEdit,1,1)
        layout.addWidget(sexLabel, 2, 0)
        layout.addWidget(self.sexLineEdit, 2, 1)
        layout.addWidget(emitLabel,3,0)
        layout.addWidget(self.phoneLineEdit,3,1)
        layout.addWidget(timeLabel,4,0)
        layout.addWidget(self.mailEdit,4,1)
        layout.setColumnStretch(1, 10)
        save_Btn = QPushButton('保存')
        cancle_Btn = QPushButton('取消')
        cancle_Btn.clicked.connect(QCoreApplication.quit)
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
    app = QApplication(sys.argv)
    a = login()
    a.show()
    sys.exit(app.exec_())
# #!/usr/bin/env python
# # -*- coding: utf-8 -*-
#
# import json
# import requests
# import pymysql
# # 装了anaconda的可以pip install pyecharts安装pyecharts
# from pyecharts.charts import Map,Geo
# from pyecharts import options as opts
# from pyecharts.globals import GeoType,RenderType
# # 绘图包参加网址https://pyecharts.org/#/zh-cn/geography_charts
#
# id = 432
#
# url="https://view.inews.qq.com/g2/getOnsInfo?name=disease_h5"
# resp=requests.get(url)
# html=resp.json()
# data=json.loads(html["data"])
# time = data["lastUpdateTime"]
# data_info = time.split(' ')[0]
# detail_time = time.split(' ')[1]
# # 获取json数据的全国省份疫情情况数据
# china=data["areaTree"][0]["children"]
# # 用于保存城市名称和确诊人数
# map_data = []
# for i in china :
#     print(i)
#     # 获得省份名称
#     province = i["name"]
#     print("province:",province)
#     province_confirm = i["total"]["confirm"]
#     # 保存省份名称和该省确诊人数
#     map_data.append((i["name"],province_confirm))
#     # 各省份下有各市，获取各市的疫情数据
#     for child in i["children"]:
#         print(child)
#         # 获取城市名称
#         city = child["name"]
#         print("city:",city)
#         # 获取确诊人数
#         confirm = int(child["total"]["confirm"])
#         # 获取疑似人数
#         suspect = int(child["total"]["suspect"])
#         # 获取死亡人数
#         dead = int(child["total"]["dead"])
#         # 获取治愈人数
#         heal = int(child["total"]["heal"])
#         # 插入数据库中
#
# c = (
#     # 声明一个map对象
#     Map()
#     # 添加数据
#     .add("确诊", map_data, "china")
#     # 设置标题和颜色
#     .set_global_opts(title_opts=opts.TitleOpts(title="全国疫情图"),
#                      visualmap_opts=opts.VisualMapOpts(split_number=6,is_piecewise=True,
#                                                        pieces=[{"min":1,"max":9,"label":"1-9人","color":"#ffefd7"},
#                                                                {"min":10,"max":99,"label":"10-99人","color":"#ffd2a0"},
#                                                                {"min":100,"max":499,"label":"100-499人","color":"#fe8664"},
#                                                                {"min":500,"max":999,"label":"500-999人","color":"#e64b47"},
#                                                                {"min":1000,"max":9999,"label":"1000-9999人","color":"#c91014"},
#                                                                {"min":10000,"label":"10000人及以上","color":"#9c0a0d"}
#                                                        ]))
#     )
# # 生成html文件
# c.render("yiqing.html")
# #
# # china_total="确诊" + str(data["chinaTotal"]["confirm"])+ "疑似" + str(data["chinaTotal"]["suspect"])+  "死亡" + str(data["chinaTotal"]["dead"]) + "治愈" + str(data["chinaTotal"]["heal"]) + "更新日期" + data["lastUpdateTime"]
# # print(china_total)
#
#
#
#
#
#
#
#
# # import requests
# # import json
# #
# #
# # def _get_request(_url):
# #     s.headers = {"Conetent-Type": "application/json;charset=UTF-8"}
# #     response = json.loads(requests.get(_url).text)
# #     status = response["status"]
# #     data = response["geocodes"][0]
# #     # addr = data["formatted_address"]
# #     # city = data["city"]
# #     # location = data["location"]
# #     # level = data["level"]
# #     adcode = data["adcode"]
# #     return adcode
# #
# # host = 'restapi.amap.com'
# # addr = '北京市昌平区科技园区昌盛路18号B1座1-5层'
# # addr_key = 'b3542a766f9d7d760d7665c725a0f115'  # 申请的key
# # s = requests.session()
# # _url = 'https://{}:443/v3/geocode/geo?address={}&output=JSON&key={}'.format(host, addr, addr_key)
# # print(_get_request(_url))



import sys
from PyQt5.QtWidgets import *

class Table(QWidget):
    def __init__(self):
        super(Table, self).__init__()
        self.initUI()
    def initUI(self):
        self.setWindowTitle("QTableWidget例子")
        self.resize(400,300)
        layout=QHBoxLayout()

        #实现的效果是一样的，四行三列，所以要灵活运用函数，这里只是示范一下如何单独设置行列
        TableWidget=QTableWidget(4,3)

        # TableWidget = QTableWidget()
        # TableWidget.setRowCount(4)
        # TableWidget.setColumnCount(3)



        #设置水平方向的表头标签与垂直方向上的表头标签，注意必须在初始化行列之后进行，否则，没有效果
        TableWidget.setHorizontalHeaderLabels(['姓名','性别','体重（kg）'])
        #Todo 优化1 设置垂直方向的表头标签
        #TableWidget.setVerticalHeaderLabels(['行1', '行2', '行3', '行4'])

        #TODO 优化 2 设置水平方向表格为自适应的伸缩模式
        ##TableWidget.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)

        #TODO 优化3 将表格变为禁止编辑
        #TableWidget.setEditTriggers(QAbstractItemView.NoEditTriggers)

        #TODO 优化 4 设置表格整行选中
        #TableWidget.setSelectionBehavior(QAbstractItemView.SelectRows)

        #TODO 优化 5 将行与列的高度设置为所显示的内容的宽度高度匹配
        #QTableWidget.resizeColumnsToContents(TableWidget)
        #QTableWidget.resizeRowsToContents(TableWidget)

        #TODO 优化 6 表格头的显示与隐藏
        #TableWidget.verticalHeader().setVisible(False)
        #TableWidget.horizontalHeader().setVisible(False)

        #TOdo 优化7 在单元格内放置控件
        # comBox=QComboBox()
        # comBox.addItems(['男','女'])
        # comBox.addItem('未知')
        # comBox.setStyleSheet('QComboBox{margin:3px}')
        # TableWidget.setCellWidget(0,1,comBox)
        #
        # searchBtn=QPushButton('修改')
        # searchBtn.setDown(True)
        # searchBtn.setStyleSheet('QPushButton{margin:3px}')
        # TableWidget.setCellWidget(0,2,searchBtn)


        #添加数据
        newItem=QTableWidgetItem('张三')
        TableWidget.setItem(0,0,newItem)

        newItem=QTableWidgetItem('男')
        TableWidget.setItem(0,1,newItem)

        newItem=QTableWidgetItem('160')
        TableWidget.setItem(0,2,newItem)

        layout.addWidget(TableWidget)

        self.setLayout(layout)
if __name__ == '__main__':
    app=QApplication(sys.argv)
    win=Table()
    win.show()
    sys.exit(app.exec_())

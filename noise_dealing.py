from PIL import Image
from PIL import ImageFilter
import numpy as np

# 得到Image对象
img = Image.open(r'D:\firefox\firefox_download\大三下\图像处理\pro1\pro1\saber_noise.png')
# joan_noise.png
print(img.mode)
width, height = img.size


# image_array = np.array(img)
# print(image_array)

# detF = img.filter(ImageFilter.DETAIL)
# detF.show()



#
MedF = img.filter(ImageFilter.MedianFilter)                ##z值滤波
# bluF = img.filter(ImageFilter.BLUR)                ##均值滤波
# conF = img.filter(ImageFilter.CONTOUR)             ##找轮廓
# edgeF = img.filter(ImageFilter.FIND_EDGES)         ##边缘检测
# img.show()
MedF.show()

detF = MedF.filter(ImageFilter.DETAIL)
detF.show()

# bluF.show()

# conF.show()
# edgeF.show()

# for i in range(0, width):
#     for j in range(0, height):
#         dot = img.getpixel((i, j))
#         print(dot,end=' ')
#     print('\n')
#
# print(img.histogram())

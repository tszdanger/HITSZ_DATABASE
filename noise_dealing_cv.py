import numpy as np
# import cv2
import tensorflow as tf
from PIL import Image
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import os
import scipy.signal as signal

input_images = np.zeros((632,474))
img = Image.open(r'D:\firefox\firefox_download\大三下\图像处理\pro1\pro1\saber_noise.png')


width = img.size[0]
height = img.size[1]

plt.subplot(2,1,1)
plt.imshow(img,cmap=cm.gray) # 绘制图像image

plt.subplot(2,1,2)
image_array = np.array(img)
plt.hist(image_array.flatten(),256)



# plt.subplot(2,1,1)
# data = signal.medfilt2d(np.array(img), kernel_size=3)  # 二维中值滤波
# for h in range(0, height):
#     for w in range(0, width):
#         if data[h][w] < 128:
#             input_images[h, w] = 0
#         else:
#             input_images[h, w] = 1
# img2 = Image.fromarray(input_images)
# plt.imshow(img2,cmap=cm.gray) # 绘制图像image

# plt.subplot(1,3,1)
# data = signal.medfilt2d(np.array(img), kernel_size=3)  # 二维中值滤波
# for h in range(0, height):
#     for w in range(0, width):
#         input_images[h,w] = data[h,w]
# img2 = Image.fromarray(input_images)
# plt.imshow(img2,cmap=cm.gray) # 绘制图像image
#
# plt.subplot(1,3,2)
# data = signal.medfilt2d(np.array(img), kernel_size=5)  # 二维中值滤波
# for h in range(0, height):
#     for w in range(0, width):
#         input_images[h,w] = data[h,w]
# img2 = Image.fromarray(input_images)
# plt.imshow(img2,cmap=cm.gray) # 绘制图像image
#
# plt.subplot(1,3,3)
# data = signal.medfilt2d(np.array(img), kernel_size=7)  # 二维中值滤波
# for h in range(0, height):
#     for w in range(0, width):
#         input_images[h,w] = data[h,w]
# img2 = Image.fromarray(input_images)
# plt.imshow(img2,cmap=cm.gray) # 绘制图像image



plt.show()
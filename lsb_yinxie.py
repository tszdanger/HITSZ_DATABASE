
from PIL import Image
#得到Image对象
img = Image.open(r'D:\firefox\firefox_download\大三下\图像处理\pro1\pro1\image_secret.png')
print (img.mode)
width,height = img.size

for i in range(0,width):
	for j in range(0,height):

		dot = img.getpixel((i,j))
		if dot&0x1 == 0:
			img.putpixel((i,j),0)
		else:
			img.putpixel((i,j),255)
img.show()



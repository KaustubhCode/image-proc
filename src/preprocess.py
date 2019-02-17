import cv2
import os
img = cv2.imread(os.getcwd()+'/./data/lenet_data/1.png',0)
print(os.getcwd()+'/./data/lenet_data/1.png')
if img.shape != [28,28]:
    img2 = cv2.resize(img,(28,28))
    img = img2.reshape(28,28,-1);
else:
    img = img.reshape(28,28,-1);
#revert the image,and normalize it to 0-1 range
# img = 1.0 - img/255.0
o = open(os.getcwd()+"/./data/lenet_data/1.txt","w")
for i in range(img.shape[0]):
	for j in range(img.shape[1]):
		o.write(str(img[i][j][0])+"\n")
o.close()
#估算方框图中心坐标

import cv2
import numpy as np
img = cv2.imread('../data/left/1.bmp')

# set blue thresh
lower_blue=np.array([100,43,46])
upper_blue=np.array([124,255,255])

# lower_red = np.array([0,43,46])
# upper_red = np.array([10,255,255])

hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)
blue = cv2.bitwise_and(img, img, mask=mask_blue)
img_blue = cv2.cvtColor(blue, cv2.COLOR_HSV2BGR)
gray_blue = cv2.cvtColor(img_blue,cv2.COLOR_BGR2GRAY)

# minLineLength = 30
# maxLineGap = 10
# lines = cv2.HoughLinesP(edges,1,np.pi/180,15,minLineLength,maxLineGap)
lines_blue = np.squeeze(cv2.HoughLines(gray_blue,1,np.pi/180,200))

points = []
xs = []
ys = []
for line in lines_blue:
    rho, theta = line[0], line[1]
    a = np.cos(theta)
    b = np.sin(theta)
    x0 = a*rho
    y0 = b*rho
    x1 = int(x0 + 1000*(-b))
    y1 = int(y0 + 1000*(a))
    x2 = int(x0 - 1000*(-b))
    y2 = int(y0 - 1000*(a))
    cv2.line(img,(x1,y1),(x2,y2),(0,255,10),1)
    print(x1,y1, x2,y2)
    xs.append(x1)
    xs.append(x2)
    ys.append(y1)
    ys.append(y2)

point_left = [488, 393]
point_right = [165, 396]

# cv2.imshow('hough',img)
# cv2.waitKey(0)
# cv2.destroyAllWindows()

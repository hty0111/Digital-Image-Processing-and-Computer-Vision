import numpy as np
import cv2
from matplotlib import pyplot as plt

image_l = cv2.imread('../images/tsukuba_l.png', 0)
image_r = cv2.imread('../images/tsukuba_r.png', 0)
stereo = cv2.StereoBM_create(numDisparities=48, blockSize=15)
disparity = stereo.compute(image_l, image_r)

plt.title("BM disparity")
plt.imshow(disparity, 'gray')
plt.axis("off")
plt.savefig("../images/BM_disparity.png")
plt.show()

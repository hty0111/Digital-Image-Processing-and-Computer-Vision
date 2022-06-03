import numpy as np
import cv2
from matplotlib import pyplot as plt
# import imageio
import cv2

image_l = cv2.imread('../images/tsukuba_l.png', 0)
image_r = cv2.imread('../images/tsukuba_r.png', 0)

stereo = cv2.StereoSGBM_create(0, 48, 3, 8 * 3 * 3, 32 * 3 * 3, 2, 63, 15, 100, 1, cv2.StereoSGBM_MODE_SGBM_3WAY)
disparity = stereo.compute(image_l, image_r)

plt.title("SGBM disparity")
plt.imshow(disparity, 'gray')
plt.axis("off")
plt.savefig("../images/SGBM_disparity`.png")
plt.show()

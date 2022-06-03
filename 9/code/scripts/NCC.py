#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Description: 
# @version: v1.0
# @Author: HTY
# @Date: 2022-06-02 15:06:21

import cv2
import numpy as np
import argparse
import os
from scipy.ndimage import uniform_filter
from matplotlib import pyplot as plt


def get_args():
    parser = argparse.ArgumentParser(description="Arguments", formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--image_name", type=str,   nargs=2,    default=["tsukuba_l.png", "tsukuba_r.png"],
                        help="Input image names")
    parser.add_argument("--image_root", type=str,   default="../images/",   help="Root of images")
    parser.add_argument("--steps",      type=int,   default=30,             help="Steps of slip window")
    parser.add_argument("--offset",     type=int,   default=5,              help="Offset")
    parser.add_argument("--window_size",type=int,   default=12,             help="Window size")
    return parser.parse_args()


def read_image(root, names):
    """
    read image from root
    :param root: image_root
    :param names: image_name
    :return: gray images  type: np.ndarray
    """
    path1 = os.path.join(root, names[0])
    path2 = os.path.join(root, names[1])
    if not os.path.isfile(path1) or not os.path.isfile(path2):
        print("Path error!")
        exit()
    image_l = cv2.imread(path1, 0)
    image_r = cv2.imread(path2, 0)
    return image_l, image_r


def ncc(image_l, image_r, window_size, steps, offset):
    """
    ncc disparity map
    :param image_l: left image
    :param image_r: right image
    :param window_size: size of slipping window
    :param steps:
    :param offset: start index of pixel
    :return: disparity image ranging from [0, 255]
    """
    h, w = image_l.shape
    # init matrix
    mean_l = np.zeros((h, w))
    mean_r = np.zeros((h, w))
    sum_l_r = np.zeros((h, w))  # numerator of NCC formular
    sum_l = np.zeros((h, w))    # denominator of NCC formular
    sum_r = np.zeros((h, w))    # denominator of NCC formular
    depth = np.zeros((steps, h, w))
    # get mean value of silp window
    uniform_filter(image_l, window_size, mean_l)
    uniform_filter(image_r, window_size, mean_r)
    # normalize
    norm_l = image_l - mean_l
    norm_r = image_r - mean_r

    # recurrent to calculate sum
    for i in range(steps):
        uniform_filter(np.roll(norm_l, -i - offset) * norm_r, window_size, sum_l_r)
        uniform_filter(np.roll(norm_l, -i - offset) * np.roll(norm_l, -i - offset), window_size, sum_l)
        uniform_filter(norm_r * norm_r, window_size, sum_r)
        depth[i] = sum_l_r / np.sqrt(sum_l * sum_r)

    # renormalization
    disparity = np.argmax(depth, axis=0)
    disparity = (disparity / disparity.max() * 255).astype(np.uint8)
    return disparity


def callback(window_size):
    """
    :param window_size: param on trackbar
    :return:
    """
    args = get_args()
    image_l, image_r = read_image(args.image_root, args.image_name)
    disparity = ncc(image_l, image_r, window_size, args.steps, args.offset)
    cv2.imshow("NCC disparity", disparity)


def main():
    args = get_args()
    image_l, image_r = read_image(args.image_root, args.image_name)
    print(f"image shape: {image_l.shape}")
    disparity = ncc(image_l, image_r, args.window_size, args.steps, args.offset)

    # create trackbar of window size
    win_name = "NCC disparity"
    cv2.namedWindow(win_name, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(win_name, 600, 600)
    cv2.imshow(win_name, disparity)
    cv2.createTrackbar("Window size", win_name, 0, 20, callback)
    cv2.setTrackbarPos("Window size", win_name, 12)
    cv2.waitKey(0)

    # visualize
    plt.figure(figsize=(15, 6))
    plt.subplot(131), plt.title("Left image"), plt.axis("off"), plt.imshow(image_l, "gray")
    plt.subplot(133), plt.title("Right image"), plt.axis("off"), plt.imshow(image_r, "gray")
    plt.subplot(132), plt.title("NCC disparity"), plt.axis("off"), plt.imshow(disparity, "gray")
    plt.savefig(args.image_root + "NCC.png")
    plt.tight_layout()
    plt.show()

    plt.title("NCC disparity")
    plt.imshow(disparity, "gray")
    plt.axis("off")
    plt.savefig(args.image_root + "NCC_disparity.png")


if __name__ == "__main__":
    main()

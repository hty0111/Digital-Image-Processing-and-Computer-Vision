#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Description:
# @version: v1.0
# @Author: HTY
# @Date: 2022-05-26 16:22:44

import cv2
import os
import argparse
import numpy as np
from matplotlib import pyplot as plt


def get_args():
    parser = argparse.ArgumentParser(description="Arguments", formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--image_name",     type=str,   default="monkey.bmp",   help="Input image name")
    parser.add_argument("--image_root",     type=str,   default="./images",     help="Root of images")
    parser.add_argument("--save_image",     action="store_true",                help="Save images to image_root")
    parser.add_argument("--motion_angle",   type=float, default=11,             help="Rotation angle of image")
    parser.add_argument("--motion_offset",  type=float, default=30,             help="Offset of image")
    parser.add_argument("--noise_var",      type=float, default=0.02,           help="Variance of Gaussian noise")
    return parser.parse_args()


def read_image(root, name):
    image_path = os.path.join(root, name)
    if not os.path.isfile(image_path):
        print("Path error!")
        exit()
    input_image = cv2.imread(image_path, 0)
    print("image shape: {}".format(input_image.shape))
    return input_image


def get_psf(src, angle, offset):
    x_center = int((src.shape[0] - 1) / 2)
    y_center = int((src.shape[1] - 1) / 2)
    psf = np.zeros(src.shape)
    for i in range(offset):
        x_offset = round(np.sin(angle * np.pi / 180) * i)
        y_offset = round(np.cos(angle * np.pi / 180) * i)
        psf[int(x_center - x_offset), int(y_center + y_offset)] = 1
    return psf


def wiener(src, psf, var, K=0.02):
    src_fft = np.fft.fft2(src)
    psf_fft = np.fft.fft2(psf) + var
    dst = np.fft.ifft2(src_fft * np.conj(psf_fft) / (np.abs(psf_fft) ** 2 + K))
    dst = np.abs(np.fft.fftshift(dst))
    return dst


def show_image(win_name, image, save_name=None, save_path=None, isSave=False, wait_key=0, size=None):
    if size is not None and size != (0, 0):
        cv2.namedWindow(win_name, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(win_name, size[0], size[1])
    else:
        cv2.namedWindow(win_name, cv2.WINDOW_AUTOSIZE)

    cv2.imshow(win_name, image)
    if wait_key >= 0:
        cv2.waitKey(wait_key)
    if save_path is not None and isSave:
        cv2.imwrite(os.path.join(save_path, save_name), image)


def main():
    args = get_args()
    input_image = read_image(args.image_root, args.image_name)
    psf_image = get_psf(input_image, args.motion_angle, args.motion_offset)
    inverse_image = wiener(input_image, psf_image / psf_image.sum(), 0).astype(np.uint8)
    wiener_image = wiener(input_image, psf_image / psf_image.sum(), args.noise_var).astype(np.uint8)

    # visualize
    show_image("Original image", input_image, size=(600, 600))
    show_image("PSF", psf_image, size=(600, 600))
    show_image("Inverse image", inverse_image, "inverse.png", args.image_root, args.save_image, size=(600, 600))
    show_image("Wiener image", wiener_image, "wiener.png", args.image_root, args.save_image, size=(600, 600))
    plt.figure(figsize=(18, 8))
    plt.subplot(141), plt.title("Original image"), plt.axis('off'), plt.imshow(input_image, 'gray')
    plt.subplot(142), plt.title("PSF"), plt.axis('off'), plt.imshow(psf_image, 'gray')
    plt.subplot(143), plt.title("Inverse filter"), plt.axis('off'), plt.imshow(inverse_image, 'gray')
    plt.subplot(144), plt.title("Wiener filter"), plt.axis('off'), plt.imshow(wiener_image, 'gray')
    plt.tight_layout()
    plt.savefig(os.path.join(args.image_root, "results.png"))
    plt.show()


if __name__ == "__main__":
    main()

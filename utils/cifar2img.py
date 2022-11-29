import pickle
from PIL import Image
import numpy as np
import os
import cv2 as cv

def unpickle(file):
    with open(file, 'rb') as fo:
        dict = pickle.load(fo, encoding='bytes')
    return dict


if __name__ == "__main__":
    # Load the dataset
    cifar10 = unpickle("/home/parth/Downloads/cifar-10-python/cifar-10-batches-py/data_batch_1")
    labels = unpickle("/home/parth/Downloads/cifar-10-python/cifar-10-batches-py/batches.meta")
    
    # print images to files
    with open("val_input.csv", "w") as f:
        for i in range(10000):
            # img = cifar10[b'data'][i].reshape((3, 32, 32)).transpose(1, 2, 0)
            # img = Image.fromarray(img, 'RGB')
            # create directory if it doesn't exist
            img = cifar10[b'data'][i].reshape((3, 32, 32)).transpose(1, 2, 0)
            # crop image to 28x28
            img = img[2:30, 2:30]
            # reshape to 1d array
            img = img.reshape(1, 3*28*28)
            # dir = f"../images/binary/{labels[b'label_names'][cifar10[b'labels'][i]].decode('utf-8')}"
            # os.makedirs(dir, exist_ok=True)
            # save image as csv
            # np.savetxt(f"{dir}/{i}.csv", img, delimiter=",", newline="")
            for j in range((3*28*28)-1):
                f.write(f"{img[0][j]},")
            f.write(f"{img[0][-1]}\n")
            # with open(f"{dir}/{i}.txt", 'wb') as f:
            #     f.write(img)
            
    # write labels to csv
    with open("val_output.csv", "w") as f:
        for i in range(10000):
            one_hot = [0]*10
            one_hot[cifar10[b'labels'][i]] = 1
            for j in range(9):
                f.write(f"{one_hot[j]},")
            f.write(f"{one_hot[-1]}\n")

                    
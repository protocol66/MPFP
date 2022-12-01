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
    # cifar10 = unpickle("/home/parth/Downloads/cifar-10-python/cifar-10-batches-py/test_batch")
    # labels = unpickle("/home/parth/Downloads/cifar-10-python/cifar-10-batches-py/batches.meta")
    cifar10 = unpickle(r"C:\Users\Parth\Downloads\cifar-10-batches-py\test_batch")
    labels = unpickle(r"C:\Users\Parth\Downloads\cifar-10-batches-py\batches.meta")
    
    with open("image.h", "w") as f:
        # write image as a header file
        img = cifar10[b'data'][3]
        f.write('static const uint8_t image_data[] = {')
        for i in range(3*32*32):
            if i % 32 == 0:
                f.write('\n')
            f.write(f"0x{img[i]:02x}, ")
        f.write('\n};\n')
        
        label = cifar10[b'labels'][3]
        f.write(f'static const uint8_t label = {label};\n')

    exit()
    num_images = 10000
    
    # print images to files
    with open("val_input.csv", "w") as f:
        for i in range(num_images):
            # img = cifar10[b'data'][i].reshape((3, 32, 32)).transpose(1, 2, 0)
            # img = Image.fromarray(img, 'RGB')
            # create directory if it doesn't exist
            img = cifar10[b'data'][i]
            # crop image to 28x28
            # img = img[2:30, 2:30]
            # reshape to 1d array
            # img = img.reshape(1, 3*32*32)
            # dir = f"../images/binary/{labels[b'label_names'][cifar10[b'labels'][i]].decode('utf-8')}"
            # os.makedirs(dir, exist_ok=True)
            # save image as csv
            # np.savetxt(f"{dir}/{i}.csv", img, delimiter=",", newline="")
            # skip airplanes
            # if cifar10[b'labels'][i] == 0:
            #     continue
            
            for j in range((3*32*32)-1):
                f.write(f"{img[j]},")
            f.write(f"{img[-1]}\n")
            # with open(f"{dir}/{i}.txt", 'wb') as f:
            #     f.write(img)
            
    # write labels to csv
    with open("val_output.csv", "w") as f:
        for i in range(num_images):
            # if cifar10[b'labels'][i] == 0:
            #     continue
            one_hot = [0]*10
            one_hot[cifar10[b'labels'][i]] = 1
            for j in range(9):
                f.write(f"{one_hot[j]},")
            f.write(f"{one_hot[-1]}\n")

                    
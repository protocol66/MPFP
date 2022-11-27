from PIL import Image
import numpy as np
import argparse


IMAGE_COLOR_BITS = 8    #bits per subpixel - So this code may have been reused from a fpga project :)


def process_image(image_file, size, show_img=False):
    image = Image.open(image_file)

    print(f"Loaded image: {image_file}")
    print(f"Format: {image.format}")
    print(f"Color : {image.mode} -> RGB")
    print(f"Resizing image: {image.size} -> {size}")
    print(f"Quantizing image: {2**IMAGE_COLOR_BITS} colors per subpixel")

    image = image.resize(size)
    image = image.convert("RGB")        # some images have more than 3 channels

    #show non-downscaled image
    if show_img:
        image.show()

    image_array = np.array(image)
    image_array = image_array / (255.0 / (2**IMAGE_COLOR_BITS))  # downscale colors
    image_array = np.array(image_array, dtype=np.int32)
    image_array = np.clip(image_array, 0, (2**IMAGE_COLOR_BITS)-1)

    #show downscaled image
    if show_img:
        image = Image.fromarray(np.array(image_array*(255.0 / (2**IMAGE_COLOR_BITS)), dtype=np.int8), "RGB")
        image.show()

    comp_array = np.empty((image_array.shape[0], image_array.shape[1]), dtype=np.int32)
    for i in range(image_array.shape[0]):
        for j in range(image_array.shape[1]):
            comp_array[i][j] = (image_array[i][j][0] << IMAGE_COLOR_BITS*2) + (image_array[i][j][1] << IMAGE_COLOR_BITS*1) + (image_array[i][j][2] << IMAGE_COLOR_BITS*0)

    return comp_array


def write_c_header_file(img_array, outfile):
    with open(outfile, "w") as f:
        f.write('static const uint32_t image_data[] = {\n')

        for i in range(img_array.shape[0]):
            for j in range(img_array.shape[1]):
                f.write(f"0x{img_array[i][j]:08x}, ")
            f.write('\n')



if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Convert image to vhdl code')
    parser.add_argument("-i", "--image", required=True,action="store")
    parser.add_argument("-c", "--code_output", required=False, default="image.h", action="store")
    args = parser.parse_args()

    img_array = process_image(args.image, (28, 28), show_img=False)
    write_c_header_file(img_array, args.code_output)

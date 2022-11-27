import torch
import onnx
from onnx2keras import onnx_to_keras
from torchinfo import summary

from lenet import LeNet

# helpfull link: https://pypi.org/project/onnx2keras/


# ------------- Stage 0 Load model -------------

model = LeNet()
model.load_state_dict(torch.load("lenet_cifar10.pt"))
model.cpu()
model.eval()

dummy_input = torch.randn(1, 3, 28, 28)


# ------------- Stage 1 Convert to ONNX -------------

input_names = ["input"]
output_names = ["output"]

torch.onnx.export(model, dummy_input, "lenet_cifar10.onnx", verbose=True, input_names=input_names, output_names=output_names)


# ------------- Stage 2 Convert to Keras -------------

onnx_model = onnx.load("lenet_cifar10.onnx")
k_model = onnx_to_keras(onnx_model, input_names, change_ordering=True, name_policy="renumerate")


# ------------- Stage 3 Save Keras model and give summary -------------

k_model.save("lenet_cifar10.h5")


# ------------- Stage 4 Show model summarys -------------

print('\n'*2)

print("#"*40)
print("             Original model")
print("#"*40)
summary(model, input_data=dummy_input)

print('\n'*2)

print("#"*40)
print("             Keras model")
print("#"*40)
k_model.summary()
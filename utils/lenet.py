import torch
import torch.nn as nn
import torchmetrics
from torchvision import datasets, transforms
import pytorch_lightning as pl

class LeNet(pl.LightningModule):
    def __init__(self, num_classes=10):
        super().__init__()
        self.accuracy = torchmetrics.Accuracy()
        
        self.feature_extractor = nn.Sequential(
            nn.LazyConv2d(6, 5, padding=2),
            nn.ReLU(),
            nn.MaxPool2d(2,2),
            nn.LazyConv2d(16, 5, padding=2),
            nn.ReLU(),
            nn.MaxPool2d(2,2),
            nn.LazyConv2d(120, 5, padding=2),
            nn.ReLU(),
        )
        
        self.classifier = nn.Sequential(
            # nn.Flatten(),
            nn.LazyLinear(84),
            nn.ReLU(),
            nn.LazyLinear(num_classes),
        )
        
        self.num_classes = num_classes
        
    def forward(self, x):
        x = self.feature_extractor(x)
        x = torch.flatten(x, 1)
        logits = self.classifier(x)
        # probs = torch.softmax(logits, dim=1)
        return logits
    
    def configure_optimizers(self):
        return torch.optim.Adam(self.parameters(), lr=1e-3)
    
    def training_step(self, batch, batch_idx):
        x, y = batch
        # x = x.view(x.size(0), -1)
        x_hat = self.forward(x)
        loss = nn.functional.cross_entropy(x_hat, y)
        pred = torch.argmax(x_hat, dim=1)

        acc = self.accuracy(pred, y)
        
        self.log("train_loss", loss)
        self.log("train_acc", acc)
        return loss

    def validation_step(self, batch, batch_idx):
        x, y = batch
        # x = x.view(x.size(0), -1)
        x_hat = self.forward(x)
        loss = nn.functional.cross_entropy(x_hat, y)
        pred = torch.argmax(x_hat, dim=1)

        acc = self.accuracy(pred, y)
        acc = self.accuracy(pred, y)
        
        self.log("val_loss", loss)
        self.log("val_acc", acc)
        return pred
    
    # def test_accuracy(self, loader : DataLoader):
    #     self.eval()
    #     acc_sum = 0
    #     for idx, (x, y) in enumerate(loader):
    #         x_hat = nn.functional.softmax(self.forward(x), dim=1)
    #         pred = torch.argmax(x_hat, dim=1)
    #         acc = self.accuracy(pred, y)
    #         acc_sum += acc

    #     print(f"Accuracy: {acc_sum / (idx + 1)}")
    #     return acc_sum / (idx + 1)
    #         # print(f"Accuracy: {acc}")

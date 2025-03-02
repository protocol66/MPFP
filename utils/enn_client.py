import cv2
import numpy as np
import asyncio
import serial
import logging


logging.basicConfig(level=logging.DEBUG)

class EnnClient:

    # see at_cmd for explanation, these are from the perspective of the device
    AT_CMD_TS_CMD_INVALID    =  "AT+CMD_INVALID\n"
    AT_CMD_RQ_STATUS         =  "AT+STATUS?\n"
    AT_CMD_TS_STATUS_READY   =  "AT+STATUS=READY\n"
    AT_CMD_TS_STATUS_BUSY    =  "AT+STATUS=BUSY\n"
    AT_CMD_TS_STATUS_ERROR   =  "AT+STATUS=ERROR\n"
    AT_CMD_RD_IMAGE          =  "AT+IMAGE=\n"
    AT_CMD_RC_RESET          =  "AT+RESET!\n"

    def __init__(self, video_device=0, refresh_period=0.1, img_out_size=(32,32), fov=0.5, image_file=None, **kwargs):
        self.video = cv2.VideoCapture(video_device)
        self.frame = None
        self.scaled_frame = None
        self.img_out_size = img_out_size
        self.refresh_period = refresh_period
        self.fov = fov

        if image_file is not None:
            self.frame = cv2.imread(image_file)
            self.frame = cv2.cvtColor(self.frame, cv2.COLOR_BGR2RGB)
            self.img_catpure = False
        else:
            self.img_catpure = True

        self.connected = False
        self.com_status = None

        port = kwargs.get('port', '/dev/ttyACM0')
        baud = kwargs.get('baud', 115200)
        self.ser = serial.Serial(port, baud)

    async def get_frame(self):
        if self.img_catpure:
            while True:
                ret, frame = self.video.read()
                if ret:
                    # center crop to square image to reduce fov
                    h, w, _ = frame.shape
                    frame = frame[int(h/2 - w*self.fov/2):int(h/2 + w*self.fov/2), int(w/2 - w*self.fov/2):int(w/2 + w*self.fov/2)]

                    self.frame = frame

                await asyncio.sleep(self.refresh_period)

    async def show_frame(self):
        while True:
            if self.frame is not None:
                cv2.imshow('Input Image', self.frame)
                cv2.waitKey(1)
            if self.scaled_frame is not None:
                cv2.imshow('Scaled Image', cv2.resize(np.moveaxis(self.scaled_frame,0,-1), dsize=(200,200)))   # upscale for easier viewing
                cv2.waitKey(1)
            await asyncio.sleep(self.refresh_period)

    async def scale_img(self):
        while True:
            self.scaled_frame= cv2.resize(self.frame, dsize=self.img_out_size, interpolation=cv2.INTER_CUBIC)
            # make channels first
            self.scaled_frame = np.moveaxis(self.scaled_frame, -1, 0)
            
            await asyncio.sleep(self.refresh_period)

    async def _get_status(self):
        for _ in range(20):
            for __ in range(3):      # try 3 times, before sending reset
                self.ser.flushInput()  # clear buffer
                self.ser.write(self.AT_CMD_RQ_STATUS.encode())
                await asyncio.sleep(0.1)
                rx = self.ser.read(self.ser.inWaiting())

                if rx == self.AT_CMD_TS_STATUS_READY.encode():
                    self.connected = True
                    self.com_status = 'READY'
                    return
                elif rx == self.AT_CMD_TS_STATUS_BUSY.encode():
                    self.connected = True
                    self.com_status = 'BUSY'
                    return
                elif rx == self.AT_CMD_TS_STATUS_ERROR.encode():
                    logging.warning('Device in error state')
                    self.connected = True
                    self.com_status = 'ERROR'
                    return
                else:
                    logging.debug(f'rx: invalid response: {rx.decode()}')
                    self.connected = False
                    self.com_status = None
                    
        
        logging.error('Failed to get status from device')
        exit()

    async def send_frame(self):
        self.ser.write(self.AT_CMD_RD_IMAGE.encode())
        await asyncio.sleep(0.01)
        self.ser.write(self.scaled_frame.tobytes())

    async def com_link(self):
        await asyncio.sleep(0.2)
        await self._get_status()
        logging.info('Device connected')

        while True:
            await self._get_status()
            logging.debug(f'com_status: {self.com_status}')
            if self.com_status == 'READY':
                if self.scaled_frame is not None:
                    await self.send_frame()
                    logging.debug('sent frame')

            if self.com_status == 'ERROR':
                logging.warning('Device in error state, resetting device')
                self.ser.write(self.AT_CMD_RC_RESET.encode())

            await asyncio.sleep(self.refresh_period)
                


    def run(self):
        loop = asyncio.get_event_loop()
        tasks = [
            asyncio.ensure_future(self.get_frame()),
            asyncio.ensure_future(self.scale_img()),
            asyncio.ensure_future(self.show_frame()),
            asyncio.ensure_future(self.com_link()),
        ]
        logging.info('Starting tasks')
        loop.run_until_complete(asyncio.wait(tasks))
        loop.close()
            
            
           
            


if __name__ == '__main__':
    logging.debug('Creating enn client')
    ec = EnnClient(refresh_period=0.2, video_device=2, image_file='images/8.png')
    # ec = EnnClient(refresh_period=0.2, video_device=0)
    logging.debug('Running enn client')
    ec.run()
https://toptechboy.com/ai-on-the-jetson-nano-lesson-58-controlling-an-led-with-gpio-pins-and-button-switch/

## Resize windows

```python
# resize widowns 
import cv2
print(cv2.__version__)
width=640
height=480
flip=0
camSet='nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=3264, height=2464, framerate=21/1,format=NV12 ! nvvidconv flip-method='+str(flip)+' ! video/x-raw, width='+str(width)+', height='+str(height)+', format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink'
#camSet ='v4l2src device=/dev/video1 ! video/x-raw,width='+str(width)+',height='+str(height)+',framerate=24/1 ! videoconvert ! appsink'
cam=cv2.VideoCapture(camSet)

# web camera
# cam = cv2.VideoCapture(1)
# cam.set(cv2.CAP_PROP_FRAME_WIDTH, width)
# cam.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

while True:
    _, frame = cam.read()
    cv2.imshow('myCam',frame)
    cv2.moveWindow('myCam',700,0)
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    frameSmall=cv2.resize(frame, (320, 240))
    graySmall=cv2.resize(gray, (320, 240))

    cv2.moveWindow('BW', 0, 265)
    cv2.moveWindow('nanoSmall', 0, 0)

    cv2.imshow('BW', graySmall)
    cv2.imshow('nanoSmall', frameSmall)
    
    cv2.moveWindow('BW2', 385, 265)
    cv2.moveWindow('nanoCam2', 385, 0)
    cv2.imshow('BW2', graySmall)
    cv2.imshow('nanoCam2', frameSmall)
    if cv2.waitKey(1)==ord('q'):
        break
cam.release()
cv2.destroyAllWindows()
```

##  Writting video files in opencv

```python
# save Read
# resize widowns 
import cv2
print(cv2.__version__)
width=640
height=480
flip=0
camSet='nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=3264, height=2464, framerate=21/1,format=NV12 ! nvvidconv flip-method='+str(flip)+' ! video/x-raw, width='+str(width)+', height='+str(height)+', format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink'
#camSet ='v4l2src device=/dev/video1 ! video/x-raw,width='+str(width)+',height='+str(height)+',framerate=24/1 ! videoconvert ! appsink'
cam=cv2.VideoCapture(camSet)

outVid=cv2.VideoWriter('../videos/myCam.avi', cv2.VideoWriter_fourcc(*'XVID'), 21, (width, height))
# web camera
# cam = cv2.VideoCapture(1)
# cam.set(cv2.CAP_PROP_FRAME_WIDTH, width)
# cam.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

while True:
    _, frame = cam.read()
    cv2.imshow('myCam',frame)
    cv2.moveWindow('myCam',0,0)
    outVid.write(frame)
    if cv2.waitKey(1)==ord('q'):
        break
cam.release()
outVid.release()
cv2.destroyAllWindows()
```

> **VideoWriter(filename, fourcc, fps, frame_size, isColor)**
>
> outVid=cv2.VideoWriter('../videos/myCam.avi', cv2.VideoWriter_fourcc(*'XVID'), 21, (width, height))
>
> VideoWriter（）是用来保存视频的函数，其作用是将录制的视频保存成 filename
>
> - Filename : 文件路径，
>
> - fourcc :  设置视频格式
>
>   > **VideoWriter_fourcc()** 
>   >
>   > - cv2.VideoWriter_fourcc(**‘I’,‘4’,‘2’,‘0’**)—未压缩的**YUV**颜色编码，4:2:0色度子采样。兼容性好，但文件较大。文件扩展名.avi。
>   > - cv2.VideoWriter_fourcc(**‘P’,‘I’,‘M’,‘1’**)—**MPEG-1**编码类型，文件扩展名.avi。随机访问，灵活的帧率、可变的图像尺寸、定义了I-帧、P-帧和B-帧 、运动补偿可跨越多个帧 、半像素精度的运动向量 、量化矩阵、GOF结构 、slice结构 、技术细节、输入视频格式。
>   > - cv2.VideoWriter_fourcc(**‘X’,‘V’,‘I’,‘D’**)—**MPEG-4**编码类型，视频大小为平均值，MPEG4所需要的空间是MPEG1或M-JPEG的1/10，它对运动物体可以保证有良好的清晰度，间/时间/画质具有可调性。文件扩展名.avi。
>   > - cv2.VideoWriter_fourcc(**‘T’,‘H’,‘E’,‘O’**)—**OGGVorbis**，音频压缩格式，有损压缩，类似于MP3等的音乐格式。，兼容性差，件扩展名.ogv。
>   > - cv2.VideoWriter_fourcc(**‘F’,‘L’,‘V’,‘1’**)—**FLV**是FLASH VIDEO的简称，FLV流媒体格式是一种新的视频格式。由于它形成的文件极小、加载速度极快，使得网络观看视频文件成为可能，它的出现有效地解决了视频文件导入Flash后，使导出的SWF文件体积庞大，不能在网络上很好的使用等缺点。文件扩展名为.flv。
>
> - fps 设置帧率大小，要和cam 设置的相同 （int）
> - frame_size 设置frame的大小，（640， 480）
> - isColor 是否是彩色图片， bool， 默认为true

> 到最后需要将wiriter释放
>
> outVid.release()

## Readding video files in opencv

```python
# save Read
# resize widowns 
import cv2
print(cv2.__version__)
width=640
height=480
flip=0
camSet='nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=3264, height=2464, framerate=21/1,format=NV12 ! nvvidconv flip-method='+str(flip)+' ! video/x-raw, width='+str(width)+', height='+str(height)+', format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink'
#camSet ='v4l2src device=/dev/video1 ! video/x-raw,width='+str(width)+',height='+str(height)+',framerate=24/1 ! videoconvert ! appsink'
# cam=cv2.VideoCapture(camSet)
cam=cv2.VideoCapture('../videos/myCam.avi')
# outVid=cv2.VideoWriter('../videos/myCam.avi', cv2.VideoWriter_fourcc(*'XVID'), 21, (width, height))
# web camera
# cam = cv2.VideoCapture(1)
# cam.set(cv2.CAP_PROP_FRAME_WIDTH, width)
# cam.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

while True:
    _, frame = cam.read()
    cv2.imshow('myCam',frame)
    cv2.moveWindow('myCam',0,0)
    # outVid.write(frame)
    if cv2.waitKey(50)==ord('q'):
        break
cam.release()
# outVid.release()
cv2.destroyAllWindows()


# [mpeg4 @ 0x260b7810] ac-tex damaged at 9 11
# [mpeg4 @ 0x260b7810] Error at MB: 460

```

> 直接通过cam=cv2.VideoCapture('../videos/myCam.avi')，将video获取即可
>
> 可以通过    if cv2.waitKey(50)==ord('q'): 控制视频的快慢。

https://www.bilibili.com/video/BV12Z4y1x72C?t=26.4&p=15


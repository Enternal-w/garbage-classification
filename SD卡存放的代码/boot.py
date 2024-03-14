import sensor,image,lcd,time
import KPU as kpu
from machine import UART
from fpioa_manager import fm
lcd.init(freq=15000000)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_hmirror(0)
#sensor.set_vflip(1)
sensor.run(1)
task = kpu.load("/sd/yolov2.kmodel")
f=open("anchors.txt","r")
anchor_txt=f.read()
L=[]
for i in anchor_txt.split(","):
    L.append(float(i))
anchor=tuple(L)
f.close()
f=open("lable.txt","r")
lable_txt=f.read()
lable = lable_txt.split(",")
f.close()
fm.register(10, fm.fpioa.UART1_TX, force=True)
fm.register(11, fm.fpioa.UART1_RX, force=True)
uart_A = UART(UART.UART1, 115200, 8, 1, 0, timeout=1000, read_buf_len=4096)
anchor = (0.57273, 0.677385, 1.87446, 2.06253, 3.33843, 5.47434, 7.88282, 3.52778, 9.77052, 9.16828)
sensor.set_windowing((224, 224))
a = kpu.init_yolo2(task, 0.5, 0.3, 5, anchor)
classes=["battery","pills","bananas","cans","apples","bottle","ceram","green vegatable","broccoli","boxes","glass","cigarette","ora","eggshell","spitball","tile"]
classess=["batteryy","pillss","banana","canss","apples","bottl","cera","greeve","brocco","boxes","glas","cigarett","ora","eggshe","spitball","tile"]
#   可回收物  5      厨余垃圾 6           有害垃圾 8           其他垃圾4
#类别=[“电池”、“药丸”、“香蕉”、“罐头”、“苹果”、“瓶子”、“陶瓷”、“绿色蔬菜”、“花椰菜”、“盒子”、“玻璃”、“香烟”、“ora”、“蛋壳”、“唾沫球”、“瓷砖”]
while(True):
     img = sensor.snapshot()
     code = kpu.run_yolo2(task, img)
     if code:
         for i in code:
             a=img.draw_rectangle(i.rect())
             a = lcd.display(img)
             list1=list(i.rect())
             b=(list1[0]+list1[2])/2
             c=(list1[1]+list1[3])/2
             print("物体是：",classes[i.classid()])
             print("概率为：",100.00*i.value())
             print("坐标为：",b,c)
             for i in code:
                 lcd.draw_string(i.x(), i.y(), classes[i.classid()], lcd.RED, lcd.WHITE)
                 lcd.draw_string(i.x(), i.y()+12, '%f'%i.value(), lcd.RED, lcd.WHITE)
                 lcd.draw_string(50, 200,str(b), lcd.RED, lcd.WHITE)
                 lcd.draw_string(110, 200,str(c), lcd.RED, lcd.WHITE)
                 uart_A.write(classess[i.classid()]+'\r\n')
     else:
         a = lcd.display(img)
uart_A.deinit()
del uart_A
a = kpu.deinit(task)

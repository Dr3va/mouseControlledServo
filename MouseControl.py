from tkinter import *
import serial
import time
ser = serial.Serial('COM4',9600)
rootWidth=960
rootHeight=1000
windowWidth=500
windowHeight=500
rootDimensions=str(rootWidth) +"x"+ str(rootHeight) + "+0+0"
window = Tk()
root = Tk()
window.title("Servo PrecisionControl")
window.geometry(str(windowWidth) + "x" + str(windowHeight) + "+960+0")
window.config(background = 'black')
root.title("Servo MouseControl")
root.config(background='white')
root.geometry(rootDimensions)

def motion(event):
    x, y = event.x, event.y
    servoX = int((x / rootWidth) * 180)
    servoY = int((y / rootHeight) * 180)
    serialString = ('X{}Y{}'.format(servoX, servoY))
    ser.write(serialString.encode())
    print(serialString.encode())
    time.sleep(0.04)

def move(event):
    x, y = thetaX.get(), thetaY.get()
    if int(x) > 180:
        x = 180
    elif int(y) > 180:
        y = 180
    serialString = ('X{}Y{}'.format(x,y))
    ser.write(serialString.encode())

def moveButton():
    x, y = thetaX.get(), thetaY.get()
    serialString = ('X{}Y{}'.format(x,y))
    ser.write(serialString.encode())
def quit(event):
    root.quit()
    window.quit()
#def click(event)
thetaX = IntVar()
thetaY = IntVar()
entryX = Entry(window, textvariable=thetaX, width = 3)
entryX.pack()
entryY = Entry(window, textvariable=thetaY, width = 3)
entryY.pack()
Button(window, text='GO', command=moveButton).pack()
root.bind('<Motion>', motion)
window.bind('<Return>', move)
window.bind('<Escape>', quit)
root.bind('<Escape>', quit)
#root.bind('<Button-1>', click)
window.mainloop()
root.mainloop()



import serial
import signal

from time import sleep
import sys
from PyQt5.QtWidgets import (QApplication, QWidget, QPushButton, QTextEdit, QMessageBox)
from PyQt5.QtCore import QThread

class SerialReadThread(QThread):
    def __init__(self, serial):
        QThread.__init__(self)
        self.ser = serial

    def __del__(self):
        self.wait()

    def run(self):
        self.logFile = open("LogFile.txt", "w")
        running = True
        while running:
            try:
                line = self.ser.readline()
                self.logFile.write(str(line) + "\n")
                self.logFile.flush()
                print(line)
            except:
                running = False
                sleep(0.1)

        print('Closing Serial port\n')
        self.ser.close()  # close port
        print('Program ended\n')

##*------------------------------------------
class SerialGUI(QWidget):
    def closeEvent(self, event):
        reply = QMessageBox.question(self, 'Message',
                                     "Are you sure to quit?", QMessageBox.Yes |
                                     QMessageBox.No, QMessageBox.No)

        if reply == QMessageBox.Yes:
            event.accept()
            print('Closing port')
            self.ser.close();
            sys.exit(0)
        else:
            event.ignore()

    def closeHandler(self, signal, frame):
        print('Pressed ctrl+c, closing port')
        self.ser.close();
        sys.exit(0)

    def startRecord(self):
        self.ser.write(bytearray("custommod ble_record 1\r", "ascii"))
        self.ser.flush()
        sleep(0.1)

    def stopRecord(self):
        self.ser.write(bytearray("custommod stop\r", "ascii"))
        self.ser.flush()
        sleep(0.1)

    def sendEvent(self):
        bytes = bytearray(self.sendText.toPlainText()+'\r\n',"UTF-8")
        self.ser.write(bytes)

    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        #self.sendButton = QPushButton('Send', self)
        #self.sendButton.clicked.connect(self.sendEvent);
        #self.sendText = QTextEdit('Send', self)
        #self.sendButton.resize(self.sendButton.sizeHint())
        #self.sendText.resize(self.sendText.sizeHint())
        #self.sendButton.move(50, 50)
        #self.sendText.move(50, 100)

        self.startButton = QPushButton('Start', self)
        self.startButton.resize(self.startButton.sizeHint())
        self.startButton.clicked.connect(self.startRecord);
        self.startButton.move(50, 50)

        self.stopButton = QPushButton('Stop', self)
        self.stopButton.resize(self.stopButton.sizeHint())
        self.stopButton.clicked.connect(self.stopRecord);
        self.stopButton.move(50, 100)


        self.setGeometry(300, 300, 300, 220)
        self.setWindowTitle('SerialWritter')
        self.show()

        signal.signal(signal.SIGINT, self.closeHandler)

        print('Trying to open serial port\n')
        try:
            self.ser = serial.Serial('/dev/ttyACM0', 38400, timeout=1, parity=serial.PARITY_NONE, rtscts=1)
        except:
            print('Error openning serial port!')
            exit()        # your logic here

        print('Serial port openned\n')

        self.ser.write(b'status\n')
        self.readThread = SerialReadThread(self.ser)
        self.readThread.start()


app = QApplication(sys.argv)
ex = SerialGUI()
sys.exit(app.exec_())
##*------------------------------------------


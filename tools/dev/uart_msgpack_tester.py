import serial
import time
import os
import sys
import msgpack
from data_printer import DataPrinter


printer = DataPrinter(100);

def printAndParse(ans):
    if len(ans) > 0:
        ans = ans[0:-5]
        sys.stdout.write("\nAnswer: ")

        for x in ans:
            sys.stdout.write(x.encode("hex") + " ")

        parsed = msgpack.unpackb(ans)
        print("")
        print(parsed)
        printer(parsed);


def main():
    serialFile = os.path.abspath(sys.argv[1])
    inputFile = os.path.abspath(sys.argv[2])
    outputFile = os.path.abspath(sys.argv[3])
    
    ser = serial.Serial(serialFile, 115200, timeout=0.5)

    while True:
        with open(inputFile, "rb") as f:
            if os.path.getsize(inputFile) > 0:
                sys.stdout.write("\nCommand: ")
                command = f.readline()
                print(msgpack.unpackb(command))
                ser.write(command)

        open(inputFile, "wb")
        time.sleep(0.01)

        answer = ser.read_until("\0\0\0\0\0")
        printAndParse(answer)
        with open(outputFile, "r+b") as o:
            answer = ser.read_until("\0\0\0\0\0")
            o.write(answer)
            printAndParse(answer)

        ser.flushInput()
        ser.flushOutput()


if __name__ == "__main__":
    main()

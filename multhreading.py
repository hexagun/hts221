import subprocess
import threading
def temp():
        for i in range(0,10):
                subprocess.call('hts221.sh')

def light():
        for j in range(0,10):
                subprocess.call('veml7700.sh')

temp_thread=threading.Thread(target=temp)
light_thread=threading.Thread(target=light)

temp_thread.start()
light_thread.start()

temp_thread.join()
light_thread.join()

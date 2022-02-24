import datetime as dt
import time as t
import subprocess
import random
i=0
while i<100:
	p = subprocess.Popen("TH.sh", stdout=subprocess.PIPE, shell=True)
	time = str(dt.datetime.now())
	(output, err) = p.communicate()
	p_status = p.wait()
	q = subprocess.Popen("LIGHT_SENSOR.sh", stdout=subprocess.PIPE, shell=True)
	(output_q, err) = q.communicate()
	lux = " , Light Intensity : "+str(output_q)[2:len(output_q)-1]+"\n"
	file = open("weather.txt", "a")
	file.writelines("\n"+time+" , "+", ".join(str(output)[2:len(output)-1].split("\\n"))+lux+"\n")
	file.close()
	i+=1
	t.sleep(10)


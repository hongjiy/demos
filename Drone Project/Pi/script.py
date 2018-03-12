import subprocess

subprocess.call(['cd /home/pi'],shell = True)
subprocess.call(['echo a'],shell = True)
subprocess.call(['. /home/pi/.profile'],shell = True)
subprocess.call(['workon cv'],shell = True)
subprocess.call(['python Final.py'],shell = True)

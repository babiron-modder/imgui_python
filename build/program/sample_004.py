import time
import threading

def sleep_test():
  print('sleep start')
  time.sleep(10)
  print('sleep end') # joinで待たないとここまでプロセスは持ちません

print('始めます')
thread1 = threading.Thread(target=sleep_test)
thread1.start()
print('プログラムは終わりました')
thread1.join()
import multiprocessing as mp
import os

class SweepTask:
    
    def prepareSetting(self):
        pass

    def doThings(self):
        pass

def doAndRelease(func, sem):
    func()
    sem.release()

class Sweeper:
    
    def __init__(self, Task):
        issubclass(Task, SweepTask) 
        self.Task = Task

    def sweep(self):
        # numOfCores = mp.cpu_count()-2
        numOfCores = 2
        doingSem = mp.Semaphore(numOfCores)
        print("number of cpu cores: ", numOfCores)
        processList = []
        finished = False
        while(finished == False):
            task = self.Task()
            finished = not task.prepareSetting()
            proc = mp.Process(target=doAndRelease, args=(task.doThings, doingSem,))
            processList.append(proc)
            doingSem.acquire()
            proc.start()

        for process in processList:
            process.join()


if __name__ == '__main__':
    class SweepTaskTest(SweepTask):
        limit = 100*1000
        curser = 0
        
        def prepareSetting(self):
            SweepTaskTest.curser+=1
            return SweepTaskTest.curser < SweepTaskTest.limit;
        def doThings(self):
            return (SweepTaskTest.curser)

    sweeper = Sweeper(SweepTaskTest)
    sweeper.sweep()

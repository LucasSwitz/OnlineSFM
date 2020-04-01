
from time import time
from contextlib import contextmanager
from datetime import datetime
import os
import json


class PerfTestRunner:
    def __init__(self):
        self._tests = []

    def add_test(self, test):
        self._tests.append(test)

    def run(self):
        for test in self._tests:
            print(f"Runnning test {test.name()}...")
            test.run()
            test.exit()

class PerfTest:
    def __init__(self, name):
        self._steps = {}
        self._extras = {}
        self._dir = f"{name}-{datetime.now()}"
        self._name = name

        if os.path.isdir(self._dir):
            os.rmdir(self._dir)
        os.mkdir(self._dir)

    def name(self):
        return self._name
    
    @contextmanager
    def timer(self, step_name):
        start = time()
        try:
            yield
        finally:
            elapsed_ms = (time() - start)*1000
            self._steps[step_name] = elapsed_ms

    def store(self, name, b):
        with open(f"{self._dir}/{name}", "wb+") as out:
            out.write(b)
    
    def report(self, key, v):
        self._extras[key] = v

    def exit(self):
        report = {**{"test" : self._name,
                     "steps" : self._steps}, 
                  **self._extras} 
        with open(f"{self._dir}/report.json", "w+") as out:
            json.dump(report, out, indent=True)
    
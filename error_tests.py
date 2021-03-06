from subprocess import Popen, PIPE
from os import listdir
from os.path import isfile, join
from filecmp import cmp
import time

dirs = [('error_tests', ''),
        ('Tests', ''),
	    ('ex1_Tests', ''),
        ('rotate_error_tests', '-rotate')]

for dir, args in dirs:
    print("run %s tests" % dir)
    tests = [f for f in listdir(dir) if isfile(join(dir, f)) and join(dir, f).endswith(".in")]
    for test in tests:
        if args == '':
            p = Popen(['./ex3', join(dir,test), 'b'], stdin=PIPE, stdout=PIPE, stderr=PIPE)
        else:
            p = Popen(['./ex3', join(dir, test), 'b', args], stdin=PIPE, stdout=PIPE, stderr=PIPE)
        time.sleep(0.1)
        output_file = join(dir,test.replace('.in','.out'))
        with open("b") as f1:
            with open(output_file) as f2:
                str1 = f1.read()
                str2 = f2.read()
                if str1 != str2:
                    print("test - %s\noutput - %s\n" % (test, output_file))
                    print(str1 + "\n\n" + str2)
                    
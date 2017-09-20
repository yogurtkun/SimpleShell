import os

import tester


GENERAL_ERROR = "[eE]rror:.*"

t = tester.HomeworkTester()

"""Modify this file with your tests.

The test is already filled out with some basic tests.

Basically, your main usage is:

	t.add_test("command to execute 1", "expected output as a regex string")
	t.add_test("command to execute 2", "expected output as a regex string")
	...
	t.add_test("command to execute 3", "expected output as a regex string")
	t.run()
	t.print_results()
	t.reset()
"""
##################### Personal Tests 1 #########################
# history loop
t.add_test("history 1",GENERAL_ERROR)
t.add_test("history 0",GENERAL_ERROR)
t.add_test("history 1",GENERAL_ERROR)
t.run()
t.print_results()
t.reset()

##################### Personal Tests 2 #########################
# pipe
t.add_test("/bin/echo hello world | /usr/bin/wc","1       2      12")
t.add_test("history 0 | /usr/bin/wc","1       3      24")
t.run()
t.print_results()
t.reset()

##################### Personal Tests 2 #########################
# history max
for i in range(120):
	t.add_test("ls "+str(i),GENERAL_ERROR)
expected_output = "";
for i in range(99):
	expected_output += (str(i) + " ls "+str(i+21) +'\n')
expected_output += '99 history'
t.add_test('history',expected_output)
t.run()
t.print_results()
t.reset()

##################### Basic Executables #########################
# ls should not be found
t.add_test("ls", GENERAL_ERROR)

# But /bin/echo should work
t.add_test("/bin/echo hello world", "hello world")
t.run()
t.print_results()
t.reset()

############################# Builtins ##########################
# Test that cd works
t.add_test("cd /tmp", "")
t.add_test("/bin/pwd", "/tmp")
t.add_test("cd /var", "")
t.add_test("/bin/pwd", "/var")
t.add_test("cd", GENERAL_ERROR)
t.run()
t.print_results()
t.reset()

# Test that history works as expected
t.add_test("history", "0 history")
t.add_test("history -c", "")
t.add_test("    abc   abc   ", GENERAL_ERROR)
t.add_test("def", GENERAL_ERROR)
expected_output = [
    "0     abc   abc   |0 abc   abc   ",
    "1 def",
    "2 history",
]
t.add_test("history", "\n".join(expected_output))
t.add_test("history -c", "")
t.add_test("history", "0 history")
t.add_test("history blahblahblah", GENERAL_ERROR)
expected_output = [
        "0 history",
        "1 history blahblahblah",
        "2 history",
]
t.add_test("history", "\n".join(expected_output))
t.add_test("/bin/echo hello", "hello")

expected_output.extend(["3 /bin/echo hello", "4 history 0"])

t.add_test("history 0", "\n".join(expected_output))

expected_output.append("5 history")

t.add_test("history", "\n".join(expected_output))
t.run()
t.print_results()
t.reset()

############################# Pipes #############################
t.add_test("/bin/echo hello world | /bin/grep hello", "hello world")
t.add_test("/bin/echo blah          |/usr/bin/cut -b 3,4", "ah")
t.add_test("/bin/echo blah|/usr/bin/cut -b 3,4", "ah")
t.run()
t.print_results()
t.reset()

t.add_test("/bin/echo hello world", "hello world")
t.add_test("history 0 | /usr/bin/wc -w", "2")
t.run()
t.print_results()
t.reset()

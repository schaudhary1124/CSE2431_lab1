# Lab1: Process Creation and Memory Performance Analysis, README file

## Build cleaning and compiling
...$ make clean
...$ make all

## [run] executing 

# I like to use
...$ time ./run_all.sh > lab1_outputFile.txt

# But can use this also
...$ bash run_all.sh > lab1_outputFile.txt 2>&1

## Short answers

# The fork() system call helps to create a new child process that is like a
# replica of the parent with a different Process ID(PID) and different return
# value. The "exec" functions includes for example the "execvp()" helps to
# replace the current process's memory image, data, and code with the new
# program, but keeps the original PID. The "wait" and "waitpid" is used by
# the parent process to essentially pause its execution till the child
# process terminates. This allows the parent to get the childs exit status
# and reap the process resources.

# A zombie process is a child process which has terminated but still has an
# entry in the process table because the parent hasn't yet called "wait()"
# or "waitpid()" to get the exit status. Zombies are prevented by making
# sure that every parent process reaps its immediate children. So the parent
# must call "waitpid()" on the childs PID after the child has terminated.
# This will tell the OS that the parent finished with the childs status, and
# that the kernel can remove the child's resources and entry from process table.

# After looking at the output of file in Part B I realized an interesting trend.
# I noticed that malloc generally had a less "AVG_TIME_NS" than alloca for
# all sizes less then and including 65536. But After that alloca had a less
# "AVG_TIME_NS". I decided to visualize the data using chatgpts built in visual
# chart generator by pasting the result to Part B.
# Located in "lab1/analysis/performance_chart.png

#

#

## Files

#- src/fork_identity.c
#- src/proc_tree.c
#- src/fork_exec.c
#- src/memory_perf.c
#- Makefile
#- README.md
#- run_all.sh
#- analysis/analysis_report.pdf
#- analysis/performance_chart.png

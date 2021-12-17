from invoke import task, Collection

@task
def task_x_exec(c, command):
    c.run(command)

@task
def task_x_compile(c):
    c.run('g++ ./src/main.cpp -Wall -std=c++11 -o ./bin/program')

ns = Collection()
ns_x = Collection('x')
ns_x.add_task(task_x_compile, 'compile')
ns.add_collection(ns_x, 'x')

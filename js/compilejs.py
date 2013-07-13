#!/usr/bin/env python
# -*- coding: utf-8 -*-

import subprocess
from os import listdir, path, sep

comp_path = "D:\\Documents\\js\\compiler-latest\\compiler.jar"

def compile_js(src, dest):
    cmd = (
        "java -jar %s --language_in=ECMASCRIPT5_STRICT"
        " --js %s --js_output_file %s"
        % (comp_path, src, dest))
    print(cmd)
    subprocess.check_call(cmd)

def main():
    curr = path.abspath(path.dirname(__file__))
    exe(curr, "lib")
    exe(curr, "test")

def exe(curr, dname):
    dir = path.join(curr, dname, "src")
    for f in listdir(dir):
        src = path.join(dir, f)
        if path.isdir(src):
            continue
        src_time = path.getmtime(src)
        dest = path.join(curr, dname, f)
        if path.isfile(dest):
            dest_time = path.getmtime(dest)
        else:
            dest_time = 0
        if src_time > dest_time:
            compile_js(src, dest)

if __name__ == "__main__":
	main()

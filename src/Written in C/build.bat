@echo off
tcc\tcc.exe stage0\meta_environment.c -o stage0\meta_environment.exe
stage0\meta_environment.exe -c stage0\program.meta stage0\program.bin
stage0\meta_environment.exe -r stage0\program.bin
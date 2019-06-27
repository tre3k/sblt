#!/bin/bash

PID_FILE=/tmp/sbltd.pid
SOCK_FILE=/tmp/sbltd.sock

kill -9 `cat ${PID_FILE}` 
rm -rf ${PID_FILE}
rm -rf ${SOCK_FILE}

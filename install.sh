#!/bin/bash
# 
# File:   install.bash
# Author: pconroy
#
# Created on May 13, 2019, 3:41:01 PM
sudo cp ./dist/Debug/GNU-Linux/*.a /usr/local/lib
sudo cp ./log4c.h /usr/local/include
sudo ldconfig -v

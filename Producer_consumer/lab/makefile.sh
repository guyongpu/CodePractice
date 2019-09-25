#!/bin/bash
echo "Start build the main.cc to main.."
g++ main.cc -o main -pthread
echo "Build finish!"

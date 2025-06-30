#!/bin/bash
while (( 1 )); do mv client client-running || echo ok; ./client-running; done

#!/bin/bash

cat $1 | tr "\233" "\012" > $2

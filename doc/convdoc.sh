#!/bin/bash

cat $1.DOC | tr "\012" "\233" > $1.TXT
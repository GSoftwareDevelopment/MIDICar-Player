#!/bin/bash

xxd -r -p header-gr8.hex header-gr8.obj
xxd -r -p footer-gr8.hex footer-gr8-2.obj
xxd -r -p uvmeter.hex uvmeter.obj
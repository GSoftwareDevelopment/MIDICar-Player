#!/bin/bash

xxd -r -p header-gr8.hex header-gr8.obj
xxd -r -p workarea.hex workarea.obj
xxd -r -p footer-gr8-3.hex footer-gr8-3.obj
xxd -r -p uvmeter.hex uvmeter.obj
xxd -r -p help.hex help.obj
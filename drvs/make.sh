#!/bin/bash

switches='-l -t'

mads ./MIDIBox_drv.a65 $switches -o:../bin/MIDIBox.drv
mads ./MIDIMate_drv.a65 $switches -o:../bin/MIDIMate.drv

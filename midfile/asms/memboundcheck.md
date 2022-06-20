A procedure to check for memory area boundary crossing.

_Must be customized individually for the project._


## Memory under XDOS 2.43N:

| $0700..$1df0 XDOS         |                             |
| ------------------------- | --------------------------- |
| $4000..$7fff Free mem     | extended memory bank window |
| with BASIC                | without BASIC               |
| $8000..$9c1f Free mem     | $8000..$bc1f Free mem       |
| $9c20..$9c3f Display List | $bc20..$bc3f Display List   |
| $9c40..$9fff Screen       | $bc40..$bfff Screen         |
| $a000..$bfff Free mem     |                             |
| RAM under ROM             |                             |
| $c000..$cfff Free mem     |                             |
| $d000..$d7ff I/O Area     |                             |
| $d800..$dfff Free mem     |                             |
| $e000..$e3ff Font set     |                             |
| $e400..$fffd Free mem     |                             |



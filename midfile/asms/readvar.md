#

A procedure that reads a variable length value called a delta.

The primary task of a delta is to determine the relative time (in ticks)
between MIDI events. It is also used to determine the amount of data for
SysEx events.

The value occurs ONLY in the MID file - it is not part of the MIDI protocol.

---
It is *important* that it is as __fast as possible__.

#

  The 7th bit of each byte indicates whether the next delta byte occurs

  One byte of delta
~~~
  7654 3210
  0aaa aaaa
 =0aaa aaaa
~~~

  Two bytes of delta
~~~
  7654 3210 7654 3210
  1bbb bbbb 0aaa aaaa
 =00bb bbbb Baaa aaaa
~~~

  Three bytes of delta
~~~
  7654 3210 7654 3210 7654 3210
  1ccc cccc 1bbb bbbb 0aaa aaaa
 =000c cccc CCbb bbbb Baaa aaaa
~~~

  Four bytes of delta
~~~
  7654 3210 7654 3210 7654 3210 7654 3210
  1ddd dddd 1ccc cccc 1bbb bbbb 0aaa aaaa
 =0000 dddd DDDc cccc CCbb bbbb Baaa aaaa
~~~

## Pascal version of procedure

~~~pascal
const
  OrC:array[0..3] of byte = (
    %00000000, %01000000, %10000000, %11000000
  );

  OrD:array[0..7] of byte = (
    %00000000, %00100000, %01000000, %01100000,
    %10000000, %10100000, %11000000, %11100000
  );

var
  a,b,c,d,y:byte;
  resultPTR:^Byte;

begin
  a:=0; b:=0; c:=0; d:=0;

// read block
  readB; a:=_tmp;
  if a and $80=$80 then
  begin
    b:=a and $7f;
    readB; a:=_tmp;
    if a and $80=$80 then
    begin
      c:=b; b:=a and $7f;
      readB; a:=_tmp;
      if a and $80=$80 then
      begin
        d:=c; c:=b; b:=a and $7f;
        readB; a:=_tmp;
      end;
    end;
  end;

// decode block
  if b>0 then
  begin
    if b and 1=1 then a:=a or 128;
    b:=b shr 1;
  end;
  if c>0 then
  begin
    y:=c and %11;
    if y<>0 then
      b:=b or OrC[y];
    c:=c shr 2;
  end;
  if d>0 then
  begin
    y:=d and %111;
    if y<>0 then
      c:=c or OrD[y];
    d:=d shr 3;
  end;

// result block
  ResultPTR:=@Result;
  ResultPTR^:=a; inc(ResultPTR);
  ResultPTR^:=b; inc(ResultPTR);
  ResultPTR^:=c; inc(ResultPTR);
  ResultPTR^:=d;
end;
~~~
#!/bin/bash

WORKDIR="bin"
TEMPDIR="bin/temp"
OUTPUT=$TEMPDIR/mcp.bin
MCPBIN=$WORKDIR/mcp.bin
XEX="./tools/xex-filter.pl"
ZX5="./tools/zx5 -f"

SADR=0
EADR=0
LEN=0

rm $TEMPDIR/*

function get_block() {
  $XEX -o $TEMPDIR/data.blk -i $1 -d $MCPBIN > $TEMPDIR/result
  SADR=$(cat $TEMPDIR/result | grep "  $1:" | cut -c 20-23)
  EADR=$(cat $TEMPDIR/result | grep "  $1:" | cut -c 26-29)
  LEN=$(printf '%.4x' $(calc 0x$EADR-0x$SADR+1))
  echo -n "$1: $SADR..$EADR ($LEN)"
}

function compress_block() {
  $ZX5 -f $TEMPDIR/data.blk $TEMPDIR/data.zx5 > /dev/null
}

bswap16() {
  local num="0x${1:-0}"
  printf "%.4x" $(( ((num & 0xFF) << 8) | (num >> 8) ))
}

calcEnd() {
  local num="0x${1:-0}"
  printf "%.4x" $(( 0x4200 + num - 1 ))
}

makeLoadAddr() {
  echo "8000 8100 $(bswap16 $SADR)" | xxd -r -p >> $OUTPUT # > $TEMPDIR/load.bin
}

makeDataHeader() {
  CLEN=$(printf "%.4x" $(wc -c "$TEMPDIR/data.zx5" | awk '{print $1}'))
  echo "$(bswap16 "4200") $(bswap16 $(calcEnd $CLEN))" | xxd -r -p >> $OUTPUT # > $TEMPDIR/header.bin
}

makeInitBlock() {
  echo "e202 e302 0040" | xxd -r -p >> $OUTPUT # > $TEMPDIR/init.bin
}

makeRunBlock() {
  echo "e002 e102" | xxd -r -p >> $OUTPUT # > $TEMPDIR/init.bin
  cat $TEMPDIR/data.blk >> $OUTPUT
}

makeFile() {
  compress_block
  makeLoadAddr
  makeDataHeader
  cat $TEMPDIR/data.zx5 >> $OUTPUT
  makeInitBlock
  echo
}

cat $WORKDIR/decomp.bin >> $OUTPUT

get_block 1
makeFile

get_block 2
makeFile

get_block 4
makeFile

get_block 3
makeFile

get_block 5
makeRunBlock

{$define testBit(val,bit):=((val and bit)=bit)}
{$define isBitSet(val,bit):=((val and bit)<>0)}
{$define isBitClr(val,bit):=((val and bit)=0)}

{$define setBit(val,bit):=val:=val or bit}
{$define clrBit(val,bit):=val:=val and not bit}
{$define tglBit(val,bit):=val:=val xor bit}

{$define timer(tm,e):=(byte(_tm-tm)>=e)}
{$define resetTimer(tm):=tm:=_tm}
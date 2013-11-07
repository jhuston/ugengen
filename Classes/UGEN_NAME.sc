UGEN_NAME : UGen {
  *ar { arg in;
	^this.multiNew('audio', in).madd(mul, add)
  }
}

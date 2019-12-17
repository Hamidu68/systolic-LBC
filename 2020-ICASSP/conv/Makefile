all: conv

conv:
	vivado_hls -f script.tcl

clean:
	rm vivado_hls.log || true
	# rm -rf summary* || true
	# rm -rf poly-mult* || true

.PHONY: all clean
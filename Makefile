all:
	scons -j `echo \`sysctl -n hw.ncpu\` + 1 | bc`

clean:
	scons -c

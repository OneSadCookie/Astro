all:
	scons -j `bc <<< "\`sysctl -n hw.ncpu\` + 1"`

clean:
	scons -c

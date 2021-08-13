cpcompile(){
	local cpdirectory=${PWD%Codes*};
	cpdirectory=${cpdirectory%tmp*};
	g++ $1 -o "${cpdirectory}/build/${1%.*}" -g -std=c++17 -O2 -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector;
}

cprun(){
	local cpdirectory=${PWD%Codes*};
	cpdirectory=${cpdirectory%tmp*};
	/usr/bin/time -f "\n\n%U seconds" "${cpdirectory}/build/${1%.*}" < "${cpdirectory}/test/test.in" > "${cpdirectory}/test/test.out" ;
}

ehe(){
	cpcompile $1 && printf "\nBuild completed.\n\n" && cprun $1 && printf "Run completed.\n\n"; 
}
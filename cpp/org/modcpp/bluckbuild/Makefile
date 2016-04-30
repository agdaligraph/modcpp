CCFLAGS=-Dtestvisible=private -std=c++1z -Wall -O3 -Wno-unknown-attributes -fno-rtti -fno-stack-protector -I../../../
CXX=clang++-3.8

bluck.out: Console.o Files.o String.o Cstrings.o BluckBuild.o Parser.o Md5Hasher.o Stamper.o CppExecutor.o BluckEnvironment.o jsoncpp.o bluck.cpp
	$(CXX) $(CCFLAGS) $^ -o $@

deploy:
	cp ../../../../bluck-out/cpp/org/modcpp/bluckbuild/bluckbuild.out $(HOME)/bin/bluck

local_deploy: bluck.out
	mkdir -p ../../../../bin && mv bluck.out ../../../../bin/bluck
	$(MAKE) clean

clean:
	rm *.o

CppExecutor.o: CppExecutor.cpp CppExecutor.h
	$(CXX) $(CCFLAGS) -c $< -o $@

BluckEnvironment.o: BluckEnvironment.cpp BluckEnvironment.h
	$(CXX) $(CCFLAGS) -c $< -o $@

Stamper.o: Stamper.cpp Stamper.h
	$(CXX) $(CCFLAGS) -c $< -o $@

Console.o: ../logging/Console.cpp ../logging/Console.h
	$(CXX) $(CCFLAGS) -c $< -o $@

BluckBuild.o: BluckBuild.cpp BluckBuild.h
	$(CXX) $(CCFLAGS) -c $< -o $@

Parser.o: Parser.cpp Parser.h
	$(CXX) $(CCFLAGS) -c $< -o $@

Cstrings.o: ../string/Cstrings.cpp ../string/Cstrings.h
	$(CXX) $(CCFLAGS) -c $< -o $@

String.o: ../string/String.cpp ../string/String.h
	$(CXX) $(CCFLAGS) -c $< -o $@

Files.o: ../io/Files.cpp ../io/Files.h
	$(CXX) $(CCFLAGS) -c $< -o $@

jsoncpp.o: ../../jsoncpp/jsoncpp.cpp ../../jsoncpp/jsoncpp.h
	$(CXX) $(CCFLAGS) -c $< -o $@

Md5Hasher.o: ../hashing/Md5Hasher.cpp ../hashing/Md5Hasher.h
	$(CXX) $(CCFLAGS) -c $< -o $@

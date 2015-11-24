all: client serverA serverB serverC serverD
.PHONY : all

client: client.cpp client.h
	g++ -o clientoutput client.cpp -lsocket -lnsl -lresolv
serverA: serverA.cpp serverA.h
	g++ -o serverAoutput serverA.cpp -lsocket -lnsl -lresolv
serverB: serverB.cpp serverB.h
	g++ -o serverBoutput serverB.cpp -lsocket -lnsl -lresolv
serverC: serverC.cpp serverC.h
	g++ -o serverCoutput serverC.cpp -lsocket -lnsl -lresolv
serverD: serverD.cpp serverD.h
	g++ -o serverDoutput serverD.cpp -lsocket -lnsl -lresolv

.PHONY : clean
clean:
	rm -f *output

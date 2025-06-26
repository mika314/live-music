all: client server

client: FORCE
	echo Entering directory \`client\'
	cd client; coddle
	echo Leaving directory \`client\'

server: FORCE
	echo Entering directory \`server\'
	cd server; coddle
	echo Leaving directory \`server\'

FORCE:

#!/bin/bash

PWD="$(pwd)"
cd lib/
BASE_DIR="$(pwd)"
ARDUPI_DIR="$BASE_DIR/arduPi"
ARDUPIAPI_DIR="$BASE_DIR/arduPi-api"
LIBRARY_DIR="$BASE_DIR/libraries/arduPiLoRa"
JSON_DIR="$BASE_DIR/json"
cd ..
PROJECT_DIR="$(pwd)"



cd "$PROJECT_DIR"
file="./jsoncpp.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "jsoncpp.o -> purged"
rm ./jsoncpp.o
else
echo "jsoncpp already compiled"
fi
else if [ "$1" != "-clean" ]; then
echo "Compiling jsoncpp..."
g++ -c jsoncpp.cpp -o jsoncpp.o -I "$JSON_DIR" -I .
fi
fi


cd "$PROJECT_DIR"
file="./configuration.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "configuration.o -> purged"
rm ./configuration.o
else
echo "configuration already compiled"
fi
else if [ "$1" != "-clean" ]; then
echo "Compiling configuration..."
g++ -c configuration.cpp -o configuration.o  -I"$LIBRARY_DIR" -I"$ARDUPI_DIR" -I .
fi
fi

cd "$PROJECT_DIR"
file="./configFunction.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "config_function.o -> purged"
rm ./configFunction.o
else
echo "configFunction already compiled"
fi
else if [ "$1" != "-clean" ]; then
echo "Compiling configFunction... "
g++ -c configFunction.cpp -o configFunction.o -I  "$LIBRARY_DIR" -I "$ARDUPI_DIR" -I .

fi
fi

cd "$PROJECT_DIR"
file="./loraPacket.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "loraPacket.o -> purged"
rm ./loraPacket.o
else
echo "loraPacket already compiled"
fi
else if [ "$1" != "-clean" ]; then
echo "Compiling loraPacket... "
g++ -c loraPacket.cpp -o loraPacket.o -I  "$LIBRARY_DIR" -I "$ARDUPI_DIR" -I .

fi
fi

cd "$PROJECT_DIR"
file="./lora.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "lora.o -> purged"
rm ./lora.o
else
echo "lora already compiled"
fi
else if [ "$1" != "-clean" ]; then
echo "Compiling lora... "
g++ -c lora.cpp -o lora.o -I  "$LIBRARY_DIR" -I "$ARDUPI_DIR" -I .

fi
fi

cd "$PROJECT_DIR"
file="./trafficGenerator.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "trafficGenerator.o -> purged"
rm ./trafficGenerator.o
else
echo "lora already compiled"
fi
else if [ "$1" != "-clean" ]; then
echo "Compiling trafficGenerator... "
g++ -c trafficGenerator.cpp -o trafficGenerator.o -I  "$LIBRARY_DIR" -I "$ARDUPI_DIR" -I .

fi
fi

cd "$PROJECT_DIR"
file="./loraLog.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "loraLog.o -> purged"
rm ./loraLog.o
else
echo "lora already compiled"
fi
else if [ "$1" != "-clean" ]; then
echo "Compiling loraLog... "
g++ -c loraLog.cpp -o loraLog.o -I  "$LIBRARY_DIR" -I "$ARDUPI_DIR" -I .

fi
fi


cd "$PROJECT_DIR"
file="./loraEndNode.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "loraEndNode.o -> purged"
rm ./loraEndNode.o
else
echo "loraEndNode already compiled"
fi
else if [ "$1" != "-clean" ]; then
echo "Compiling loraEndNode... "
g++ -c loraEndNode.cpp -o loraEndNode.o -I  "$LIBRARY_DIR" -I "$ARDUPI_DIR" -I .

fi
fi



#compile arduPi
cd "$ARDUPI_DIR"
file="./arduPi.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "arduPi.o -> purged"
rm ./arduPi.o
else
echo "arduPi already compiled"
fi
else
if [ "$1" != "-clean" ]; then
echo "Compiling arduPi..."
g++ -c arduPi.cpp -o arduPi.o
fi
fi



#compile arduPi-api
cd "$ARDUPIAPI_DIR"
file="./arduPiUART.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "arduPiUART.o -> purged"
rm ./arduPiUART.o
else
echo "arduPiUART already compiled"
fi
else
if [ "$1" != "-clean" ]; then
echo "Compiling arduPiUART..."
g++ -c arduPiUART.cpp -o arduPiUART.o
fi
fi


file="./arduPiUtils.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "arduPiUtils.o -> purged"
rm ./arduPiUtils.o
else
echo "arduPiUtils already compiled"
fi
else
if [ "$1" != "-clean" ]; then
echo "Compiling arduPiUtils..."
g++ -c arduPiUtils.cpp -o arduPiUtils.o
fi
fi

file="./arduPiMultiprotocol.o"
if [ -e $file ]; then
if [ "$1" == "-clean" ]; then
echo "arduPiMultiprotocol.o -> purged"
rm ./arduPiMultiprotocol.o
else
echo "arduPiMultiprotocol already compiled"
fi
else
if [ "$1" != "-clean" ]; then
echo "Compiling arduPiMultiprotocol..."
g++ -c arduPiMultiprotocol.cpp -o arduPiMultiprotocol.o
fi
fi



#compile library
cd "$LIBRARY_DIR"
file="./arduPiLoRa.o"
if [ -e $file ]; then
	if [ "$1" == "-clean" ]; then
		echo "arduPiLoRa.o -> purged"
		rm ./arduPiLoRa.o
	else
		echo "arduPiLoRa already compiled"
	fi
else
	if [ "$1" != "-clean" ]; then
		echo "Compiling arduPiLoRa..."
		g++ -c arduPiLoRa.cpp \
			-I"$ARDUPIAPI_DIR" \
			-I"$ARDUPI_DIR" \
			-o arduPiLoRa.o
	fi
fi

sleep 1

cd "$PROJECT_DIR"
file="./$1"

if [ "$1" != "-clean" ]; then
if [ -e $file ]; then
if [ "$1" != "" ]; then
echo "Compiling PROJECT..."

g++ -lrt -lpthread -lstdc++ "$1" \
				"$LIBRARY_DIR/arduPiLoRa.o" \
				"$ARDUPIAPI_DIR/arduPiUART.o" \
				"$ARDUPIAPI_DIR/arduPiUtils.o" \
				"$ARDUPIAPI_DIR/arduPiMultiprotocol.o" \
				"$ARDUPI_DIR/arduPi.o" \
				"$PROJECT_DIR/configuration.o"\
				"$PROJECT_DIR/configFunction.o"\
				"$PROJECT_DIR/jsoncpp.o"\
				"$PROJECT_DIR/lora.o"\
				"$PROJECT_DIR/loraPacket.o"\
				"$PROJECT_DIR/trafficGenerator.o"\
				"$PROJECT_DIR/loraLog.o"\
				"$PROJECT_DIR/loraEndNode.o"\
				-I"$ARDUPI_DIR"\
				-I"$ARDUPIAPI_DIR"\
				-I"$LIBRARY_DIR"\
				-I"$PROJECT_DIR"\
				-o "$1_exe"
						else
						echo "---------------HELP------------------"
						echo "Compiling: ./mak.sh filetocompile.cpp"
						echo "Cleaning:  ./mak.sh -clean"
						echo "-------------------------------------"
						fi
						else
						echo "ERROR No such file or directory: $file"
						fi
						else
						echo "¡¡Spotless Kitchen!!"
						fi

						sleep 1

						exit 0


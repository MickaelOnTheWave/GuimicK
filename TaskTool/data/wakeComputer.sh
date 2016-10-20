#!/bin/sh

# User-defined variables
MACADDRESS=""
IPADDRESS=""
BROADCASTADDRESS=""
MAXRETRIES=3

# "Constant" variables - only modifiable by dev
ERROR=0
SUCCESS=1
WAIT_BEFORE_PING_SEC=30
MAX_PINGS=240
PING_INTERVAL_SEC=1
WAKE_BIN="/opt/bin/wakelan"

Usage()
{
	echo "Usage : wakeComputer --ip [IPADDRESS] --mac [MACADDRESS] --broadcast [BROADCASTADDRESS]"			
}

CheckRequirements()
{
	if [ "$IPADDRESS" = "" ]
	then
		echo "Error : no Ip address has been specified."			
		exit $ERROR		
	fi
	
	if [ "$MACADDRESS" = "" ]
	then
		echo "Error : no Mac address has been specified."			
		exit $ERROR		
	fi
	
	if [ "$BROADCASTADDRESS" = "" ]
	then
		echo "Error : no broadcast address has been specified."			
		exit $ERROR		
	fi
	
	if [ ! -x $WAKE_BIN ]
	then
		echo "Error : wakelan program not found."			
		exit $ERROR		
	fi
}

RunWake()
{
	retryCount=1
	while [[ $status == 0 ]] && [[ $retryCount -le $MAXRETRIES ]]
	do
		$WAKE_BIN -m $MACADDRESS -b $BROADCASTADDRESS

		pingCount=0
		sleep $WAIT_BEFORE_PING_SEC
		while [[ $pingCount -le $MAX_PINGS ]]
		do
			ping -q -c 1 $IPADDRESS > /dev/null
			if [ $? -eq 0 ]
			then
				exit $SUCCESS
			fi
			pingCount=$((pingCount+1))
			sleep $PING_INTERVAL_SEC
		done
		retryCount=$((retryCount+1))
	done
	exit $ERROR
}



### Main ###

# First thing to do in order to work : manager command line parameters. And in main!
if [ "$#" -eq 0 ]
then
	Usage
	exit $ERROR
fi
while [ "$1" != "" ]; do
	case $1 in
		-i | --ip )		shift
						IPADDRESS="$1"
						;;
		-m | --mac )	
						shift
						MACADDRESS="$1"
						;;
		-b | --broadcast )	
						shift
						BROADCASTADDRESS=$1
						;;
		-r | --retries )	
						shift
						MAXRETRIES=$1
						;;
		-h | --help )   Usage
						exit $ERROR
						;;
		* )             Usage
						exit $ERROR
	esac
	shift
done

CheckRequirements
RunWake

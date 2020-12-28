#!/usr/bin/env python

# BLE Scanning software
# ShinwooTYEM 22/02/2017

# NOTE: Python's struct.pack() will add padding bytes unless you make the endianness explicit. Little endian
# should be used for BLE. Always start a struct.pack() format string with "<"

# print string format
#    'd' Signed integer decimal.   
#    'i' Signed integer decimal.   
#    'o' Signed octal value. (1) 
#    'u' Obsolete type – it is identical to 'd'. (6) 
#    'x' Signed hexadecimal (lowercase). (2) 
#    'X' Signed hexadecimal (uppercase). (2) 
#    'e' Floating point exponential format (lowercase). (3) 
#    'E' Floating point exponential format (uppercase). (3) 
#    'f' Floating point decimal format. (3) 
#    'F' Floating point decimal format. (3) 
#    'g' Floating point format. Uses lowercase exponential format if exponent is less than -4 or not less than precision, decimal format otherwise. (4) 
#    'G' Floating point format. Uses uppercase exponential format if exponent is less than -4 or not less than precision, decimal format otherwise. (4) 
#    'c' Single character (accepts integer or single character string).   
#    'r' String (converts any Python object using repr()). (5) 
#    's' String (converts any Python object using str()). (5) 
#    'a' String (converts any Python object using ascii()). (5) 
#    '%' No argument is converted, results in a '%' character in the result.

# Format characters have the following meaning
#   Format	C Type	Python type	Standard size	Notes
#   x	pad byte	no value	 	 
#   c	char	string of length 1	1	 
#   b	signed char	integer	1	(3)
#   B	unsigned char	integer	1	(3)
#   ?	_Bool	bool	1	(1)
#   h	short	integer	2	(3)
#   H	unsigned short	integer	2	(3)
#   i	int	integer	4	(3)
#   I	unsigned int	integer	4	(3)
#   l	long	integer	4	(3)
#   L	unsigned long	integer	4	(3)
#   q	long long	integer	8	(2), (3)
#   Q	unsigned long long	integer	8	(2), (3)
#   f	float	float	4	(4)
#   d	double	float	8	(4)
#   s	char[]	string	 	 
#   p	char[]	string	 	 
#   P	void *	integer	 	(5), (3)

import os
import select
import sys
import struct
import bluetooth._bluetooth as bluez
import datetime
import time
import threading
import ConfigParser
import socket
import array
import datetime
import subprocess
import logging
import logging.config
import logging.handlers

from Queue import Queue
from uuid import getnode as get_mac
from multiprocessing import Process, Queue
from socket import errno
from time import localtime, strftime

# SET Bluetooth Scan Parameters
LE_META_EVENT = 0x3e
LE_PUBLIC_ADDRESS=0x00
LE_RANDOM_ADDRESS=0x01
LE_SET_SCAN_PARAMETERS_CP_SIZE=7

# OGF_LE_CTL + OCF CODE
OGF_LE_CTL=0x08
OCF_LE_SET_EVENT_MASK = 0x0001
OCF_LE_SET_SCAN_PARAMETERS=0x000B
OCF_LE_SET_SCAN_ENABLE=0x000C
OCF_LE_CREATE_CONN=0x000D

# these are actually subevents of LE_META_EVENT
EVT_LE_CONN_COMPLETE=0x01
EVT_LE_ADVERTISING_REPORT=0x02
EVT_LE_CONN_UPDATE_COMPLETE=0x03
EVT_LE_READ_REMOTE_USED_FEATURES_COMPLETE=0x04

# Advertisment event types
ADV_IND=0x00
ADV_DIRECT_IND=0x01
ADV_SCAN_IND=0x02
ADV_NONCONN_IND=0x03
ADV_SCAN_RSP=0x04

# Beacon pattern Source
CookieCon_s = "0A 09 43 6F 6F 6B 69 65 43 6F 6E"
JUDGE_CookieCon = CookieCon_s.replace(" ","").decode('hex')
iffu_s = "02 01 06 1A FF 4C 00 02 15"
JUDGE_iffu = iffu_s.replace(" ","").decode('hex')


# load & set Network setting
conf_file = '/home/pi/config.ini'
conf_tcp_section = 'TCP_INFO'
conf_scan_section = 'BLE_INFO'

config = ConfigParser.ConfigParser()
config.read(conf_file)

TCP_IP = config.get(conf_tcp_section, 'IP')
TCP_PORT = int(config.get(conf_tcp_section, 'PORT'))
BLE_TYPE = int(config.get(conf_scan_section, 'BLETYPE'))

scanner_mac0 = config.get(conf_scan_section, 'ReaderMAC',16)
scanner_mac_hex = scanner_mac0.decode('hex')
scanner_mac_array = bytearray(scanner_mac_hex)

BUFFER_SIZE = 1024

#Sharing Variables
th_q = Queue(1000)
#th_p = Queue(1000)

MAX_TRY = 50
try_count = 0

# Logging Path
DIR_LOG = './log/'


def check_try_count():
    try:
        global try_count
        try_count += 1
        if (try_count >= MAX_TRY) :
            os.system("sudo reboot")
    except Exception as e:
        print("Exception on check_try_count! msg: %s" % str(e))
        
def print_packet_to_string(pkt):
    ret_str = ""
    try:
        for c in pkt:
            ret_str = ret_str + str("%02x" % struct.unpack("B",c)[0])
    except Exception as e:
        print ("print_packet_to_string exception: %s" % str(e))
        #logging.debug("print_packet_to_string exception: %s" % str(e))
    return ret_str

def bytes_to_int(bytes_value):
    return int(bytes_value.encode('hex'), 16)

def bytesToNumber(b):
    try:
        total = 0
        multiplier = 1
        for count in xrange(len(b)-1, -1, -1):
            byte = b[count]
            total += multiplier * byte
            multiplier *= 256
    except Exception as e:
        print(str(e))
        #logging.debug("bytesToNumber Error! e:%s" % str(e))
    return total


def numberToByteArray(n, howManyBytes=None):
    try:
        if howManyBytes == None:
            howManyBytes = numBytes(n)
        b = bytearray(howManyBytes)
        for count in xrange(howManyBytes-1, -1, -1):
            b[count] = int(n % 256)
            n >>= 8
    except Exception as e:
        print(str(e))
        #logging.debug("numberToByteArray Error! e:%s" % str(e))
    return b

def getFiletime(dt):
    microseconds = dt / 10
    seconds, microseconds = divmod(microseconds, 1000000)
    days, seconds = divmod(seconds, 86400)
    return datetime.datetime(0001, 1, 1) + datetime.timedelta(days, seconds, microseconds)
    

def check_n_create_log_file():
    try:
        
        file_name_wts = DIR_LOG + "log_{0}.log".format(datetime.datetime.now().strftime("%y.%m.%d"))
        
        if not os.path.exists(DIR_LOG):
            os.mkdir(DIR_LOG)

        #rotate file logger
        logger = logging.getLogger(file_name_wts)
        
 
        # add a rotating handler
        handler = logging.handlers.RotatingFileHandler(path, maxBytes= 100 * 1024 * 1024, backupCount=10)
        logger.addHandler(handler)

        logger.setLevel(logging.INFO)
        
    except Exception as err:
        print "Error on check_n_create_log_file() msg:"
        print(str(err))
        ##logging.debug("Error on check_n_create_log_file() msg:" % str(err))
    except :
        print "Error on check_n_create_log_file()"
        ##logging.debug("Error on check_n_create_log_file()")
      

class Client(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.tcp_s = None
        self.chk_send = True
        self.sendcnt = 0
        self.heart_beat_cnt = 0
        self.threads = []
        
    def open_socket(self):
        running = 1
        while running:
            try:
                self.tcp_s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.tcp_s.settimeout(15)
                self.tcp_s.connect((TCP_IP, TCP_PORT))
                running = 0
            except socket.error,message:
                if self.tcp_s:
                    self.tcp_s.close()
                self.tcp_s = None
                print "Could not open socket: "
                print message
                check_try_count()
            time.sleep(2)

    def run(self):
        
        #Server side
        print (TCP_IP)
        print (TCP_PORT)

        th_parse = Bluez()
        
        cmd = 'ifconfig wlan0 down'
        os.system(cmd)
        time.sleep(10)
        cmd = 'ifconfig wlan0 up'
        os.system(cmd)
        time.sleep(10)
        
        t1 = datetime.datetime.now()

        heart_beat = bytearray()
        
        # Protocol - DATA 00~01 - Header
        # Heart Beat - 0x00 01 00 04
        heart_beat.append(0x00)
        heart_beat.append(0x01)
        # Protocol - DATA 02~03 - Length
        heart_beat.append(0x00)
        heart_beat.append(0x0a) # Data Length
        # Protocol - DATA 11~16 - scanner MAC address                        
        for i in range(0, 6, 1):
            heart_beat.append(scanner_mac_array[i])
                
        self.open_socket()


        th_parse.start()
        self.threads.append(th_parse) 

        running = 1
        while running:
            try:
                self.sendcnt = th_q.qsize()
                #sended_cnt = 0
                for cnt in range(self.sendcnt):
                    if(self.chk_send):
                        data_str = th_q.get()
                        self.chk_send = False
                    try:
                        self.tcp_s.send(data_str)
                        self.chk_send = True
                        #sended_cnt += 1
                    except IOError, e:
                        print "IO Error!"
                        print e
                        cmd = 'ifconfig wlan0 down'
                        os.system(cmd)
                        time.sleep(2)
                        cmd = 'ifconfig wlan0 up'
                        os.system(cmd)
                        time.sleep(2)
                        check_try_count()
                        try:
                            if(self.tcp_s != None):
                                self.tcp_s.close()
                            self.tcp_s = None
                            self.tcp_s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                            self.tcp_s.connect((TCP_IP, TCP_PORT))
                        except socket.error, se:
                            print "IOError Socket Error!"
                            print se
                            check_try_count()
                        except:
                            print "IOError Sending Data Unkown Error!"
                            check_try_count()
                    except :
                        print "Unkown Error!"
                        cmd = 'ifconfig wlan0 down'
                        os.system(cmd)
                        time.sleep(2)
                        cmd = 'ifconfig wlan0 up'
                        os.system(cmd)
                        time.sleep(2)
                        check_try_count()
                        try:
                            if(self.tcp_s != None):
                                self.tcp_s.close()
                            self.tcp_s = None
                            self.tcp_s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                            self.tcp_s.connect((TCP_IP, TCP_PORT))
                        except socket.error, se:
                            print "Unkown Error Socket Error!"
                            print se
                            check_try_count()
                        except:
                            print "Unkown Error Sending Data Unkown Error!"     
                            check_try_count()
                self.heart_beat_cnt += 1
                try:
                    if (self.heart_beat_cnt >= 1000):
                        self.tcp_s.send(heart_beat)
                        self.heart_beat_cnt = 0
                        print "%s %s" % ("HeartBeat Sent Time:", datetime.datetime.now())
                except socket.error, se:
                    print "IOError Socket Error!"
                    print se
                    check_try_count()
                except IOError, e:
                    print "IO Error!"
                    print e
                    cmd = 'ifconfig wlan0 down'
                    os.system(cmd)
                    time.sleep(2)
                    cmd = 'ifconfig wlan0 up'
                    os.system(cmd)
                    time.sleep(2)
                    check_try_count()
                except :
                    print "Unkown Error!"
                    cmd = 'ifconfig wlan0 down'
                    os.system(cmd)
                    time.sleep(2)
                    cmd = 'ifconfig wlan0 up'
                    os.system(cmd)
                    time.sleep(2)
                    check_try_count()
                time.sleep(0.01)
            except Exception as e:
                print "Error on Client run(self)! e:%s" % str(e)
                ##logging.debug("Error on Client run(self)! e:%s" % str(e))
                check_try_count()
            except :
                print "Error on Client run(self)!"
                ##logging.debug("Error on Client run(self)!")
                check_try_count()
                
        self.tcp_s.close()
        self.tcp_s = None
    

#Bluez side
class Bluez(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.dev_id = 0
        
        try:
            self.sock = bluez.hci_open_dev(self.dev_id)
            print "ble thread started"
            ##logging.debug("ble thread started")
            
            flt = bluez.hci_filter_new()
            bluez.hci_filter_all_events(flt)
            bluez.hci_filter_set_ptype(flt, bluez.HCI_EVENT_PKT)
            self.sock.setsockopt(bluez.SOL_HCI, bluez.HCI_FILTER, flt)
        
            print "hci_disable_le_scan"
            ##logging.debug("hci_disable_le_scan")
            self.hci_disable_le_scan()
            test = self.sock.recv(50)
            print "pkt : %s" % print_packet_to_string(test[:])
            ##logging.debug("pkt : %s" % print_packet_to_string(test[:]))
            print "hci_le_set_event_mask"
            ##logging.debug("hci_le_set_event_mask")
            self.hci_le_set_event_mask()
            test = self.sock.recv(50)
            print "pkt : %s" % print_packet_to_string(test[:])
            ##logging.debug("pkt : %s" % print_packet_to_string(test[:]))
            print "hci_le_set_scan parameters"
            ##logging.debug("hci_le_set_scan parameters")
            self.hci_le_set_scan_parameters()
            test = self.sock.recv(50)
            print "pkt : %s" % print_packet_to_string(test[:])
            #logging.debug("pkt : %s" % print_packet_to_string(test[:]))
            print "hci_enable_le_scan"
            #logging.debug("hci_enable_le_scan")
            self.hci_enable_le_scan()
            test = self.sock.recv(50)
            print "pkt : %s" % print_packet_to_string(test[:])
            #logging.debug("pkt : %s" % print_packet_to_string(test[:]))
            
            self.open_ble_socket()

        except Exception as e:
            print ( "error accessing bluetooth device... msg: %s" % str(e) )
            #logging.debug( "error accessing bluetooth device... msg: %s" % str(e) )
            os.system("sudo reboot")
    
    def hci_le_set_event_mask(self):
        cmd_pkt = struct.pack("BBBBBBBB", 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)    
        bluez.hci_send_cmd(self.sock, OGF_LE_CTL, OCF_LE_SET_EVENT_MASK, cmd_pkt)
        
    def hci_le_set_scan_parameters(self):
        cmd_pkt = struct.pack("BBBBBBB", 0x00, 0xa0, 0x0c, 0xa0, 0x0c, 0x00, 0x00)
        bluez.hci_send_cmd(self.sock, OGF_LE_CTL, OCF_LE_SET_SCAN_PARAMETERS, cmd_pkt)
    
    # user function
#    def printpacket(self,pkt):
#        for c in pkt:
#            sys.stdout.write("%02x " % struct.unpack("B",c)[0])

    def hci_enable_le_scan(self):
        cmd_pkt = struct.pack("BB", 0x01, 0x00)
        bluez.hci_send_cmd(self.sock, OGF_LE_CTL, OCF_LE_SET_SCAN_ENABLE, cmd_pkt)    

    def hci_disable_le_scan(self):
        cmd_pkt = struct.pack("BB", 0x00, 0x00)
        bluez.hci_send_cmd(self.sock, OGF_LE_CTL, OCF_LE_SET_SCAN_ENABLE, cmd_pkt)    

    def parse_events(self):    
        pkt = self.sock.recv(50)
        return pkt

            
    def open_ble_socket(self):
        try:      
            flt = bluez.hci_filter_new()
            bluez.hci_filter_all_ptypes(flt)
            bluez.hci_filter_all_events(flt)
            self.sock.setsockopt(bluez.SOL_HCI, bluez.HCI_FILTER, flt)
        except Exception as e:
            print "open_ble_socket error!"
            #logging.debug("open_ble_socket error!")
            check_try_count()
        
    def run(self):
        
        running = 1
        
        while running:
            try:
                data = self.parse_events()
                if(th_q.qsize()==1000):
                    for i in range(1, 11):
                        temp = th_q.get()
                datalen, = struct.unpack("B", data[2])
                if(len(data) != (datalen + 3)):
                    print "pkt :  %s" % print_packet_to_string(data[:])
                    #logging.debug("pkt :  %s" % print_packet_to_string(data[:]))
                #print "pkt :  %s" % print_packet_to_string(data[:])
                event, = struct.unpack("B", data[1])
                if event == LE_META_EVENT:
                    subevent, = struct.unpack("B", data[3])
                    if subevent == EVT_LE_ADVERTISING_REPORT:
                        data = data[4:]
                        judge_cookiecon = data[13:24]
                        if (judge_cookiecon == JUDGE_CookieCon):
                            now = datetime.datetime.now()
                            t_data = bytearray()
                            # Protocol - DATA 00~01 - Header #02 Data (Beacon DATA)
                            t_data.append(0x00)
                            t_data.append(0x02)
                            # Protocol - DATA 02~03 - Length
                            t_data.append(0x00)
                            t_data.append(0x1a) # Data Length
                            # Protocol - DATA 04~10 - Time, year, momth, day, hour, minute, second, microsecond
                            t_data.append(now.year % 100)
                            t_data.append(now.month)
                            t_data.append(now.day)
                            t_data.append(now.hour)
                            t_data.append(now.minute)
                            t_data.append(now.second)
                            t_data.append(now.microsecond/10000)
                            # Protocol - DATA 11~16 - scanner MAC address                        
                            for i in range(0, 6, 1):
                                t_data.append(scanner_mac_array[i])
                            # Protocol - DATA 17~22 - Beacon MAC address
                            for i in range(8, 2, -1):
                                t_data.append(data[i])
                            # Protocol - DATA 23 - Battery Level (0~100)
                            t_data.append(data[-4])
                            # Protocol - DATA 24 - Button Event (0, 1, 2)
                            t_data.append(data[-2])
                            # Protocol - DATA 25 - RSSI
                            t_data.append(data[-1])
                            th_q.put(t_data)
                            #logging.info("bm[%s] T[%s]" % (print_packet_to_string(data[8:2:-1]), datetime.datetime.now() ) )
                            logger.info("bm[%s] T[%s]" % (print_packet_to_string(data[8:2:-1]), datetime.datetime.now() ) )
                            
                    elif subevent != EVT_LE_ADVERTISING_REPORT:
                        print "pkt :  %s" % print_packet_to_string(data[:])
                        #logging.debug("pkt :  %s" % print_packet_to_string(data[:]))
                time.sleep(0.01)
            except Exception as e:
                print "Error on Bluez run(self)! e:%s" % str(e)
                #logging.debug("Error on Bluez run(self)!")
                check_try_count()
            except :
                print "Error on Bluez run(self)!"
                #logging.debug("Error on Bluez run(self)!")
                check_try_count()

        # close all threads        
        self.client.close()
        for b in self.threads:
            b.join()
           


class TCPThreadedServer(threading.Thread):
    def __init__(self, TCPPort):
        threading.Thread.__init__(self)
        self.TCPPort = TCPPort
        self.stopflag = 0

    def processing_data(self,flag,data):
        # TODO...
        try:
            
            if flag == 0:
            
                hostordernum = struct.unpack('<Q', data)
                hostordernum = getFiletime(hostordernum[0])


                local_dtm = datetime.datetime.now()
                local_time_str = local_dtm.strftime("%m%d%H%M%Y.%S")
                date_str = hostordernum.strftime('%m%d%H%M%Y.%S')

                t1 = hostordernum
                t2 = local_dtm

                print "%s %s" % ("Local Time:", local_dtm)
                print "%s %s" % ("Server Time:", hostordernum)

                #logging.debug("%s %s" % ("Local Time:", local_dtm.strftime("%Y-%m-%d %H:%M:%S")))
                #logging.debug("%s %s" % ("Server Time:", hostordernum.strftime("%Y-%m-%d %H:%M:%S")))
                
                if(t1>t2):
                    difference = t1 - t2

                    print ("Delta Secs: %d" % difference.seconds)

                    if(difference.seconds > 5):
                        date_command = "date %s" % date_str
                        os.system(date_command)

            elif flag == 1 :#0x0001 – Heart Beat
                #TODO
                print("Heart Beat data:")
                #logging.debug("Heart Beat data:")
                print ( data )
                #logging.debug(data)
     
            elif flag == 2 :#0x0002 – Data (비콘 정보)
            #TODO
                print("Data (비콘 정보) data:")
                #logging.debug("Data (비콘 정보) data:")
                print(data)    
                #logging.debug(data)  
            elif flag == 3 :#0x0003 – Request (M/W Server에서 Reader로 전송되는 명령)
                #TODO
                print("Request data:")
                #logging.debug("Request data:")
                print(data)    
                #logging.debug(data)  
            elif flag == 4 :#0x0004 – Response (M/W Server에서 전송되는 명령의 응답)
                #TODO
                print("Response data:")
                #logging.debug("Response data:")
                print(data)     
                #logging.debug(data) 
            else :#Unknown
                #TODO
                print("Unknown data:")
                #logging.debug("Unknown data:")
                print(data)
                #logging.debug(data)
        except Exception as err:
            print("processing_data err %s" % str(err))
            #logging.debug("processing_data err: %s" % str(err))
        except :
            print("processing_data err %s" % str(err))
            #logging.debug("processing_data err: %s" % str(err))
        

    def run(self):
        global TCPServerCommand
        buffer = ""
        receivedData = ""
      
        #Standard TCP socket
        server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)

        # by using an empty string we are saying any you could also use
        # localhost or socket.gethostname() if you want to expose the server
        # past your network granted you have the correct ports fowarded
        server.bind(("",self.TCPPort))

        # tell the socket to listen for incoming connections
        server.listen(2)

        # tell the socket not to block while waiting for connections
        server.setblocking(False)
        read_fds = [server]
      
        write_fds = [server]
        
        while not(self.stopflag):
            input_ready,output_ready,errors = select.select(read_fds, write_fds, [])
            
            for sock in input_ready:
                if sock is server:
                    try:
                        client,address = sock.accept()
                        print "Accepting socket from",address[0]
                        #logging.debug("Accepting socket from %s" % str(address[0]))
                    
                        ba_res_msg = bytearray()
                        # Protocol - DATA 00~01 - Header
                        # Heart Beat - 0x00 01 00 04
                        ba_res_msg.append(0x00)
                        ba_res_msg.append(0x04)
                        # Protocol - DATA 02~03 - Length
                        ba_res_msg.append(0x00)
                        ba_res_msg.append(0x0c) # Data Length  12
                        # Protocol - DATA 11~16 - scanner MAC address
                        ba_res_msg.append(0x00) # CMD 0x00 response mac
                        for i in range(0, 6, 1):
                            ba_res_msg.append(scanner_mac_array[i])
                        ba_res_msg.append(0x01)

                        client.send(ba_res_msg)
                    
                        read_fds.append(client)
                    except Exception as err:
                        print "Exception on Send Mac!"
                        #logging.debug("Exception on Send Mac!")
                        sock.close()
                        read_fds.remove(sock)
                        print(str(err))
                        #logging.debug(str(err))
                        check_try_count()
                    except :
                        print "Unknown error on Send Mac."
                        #logging.debug("Unknown error on Send Mac.")
                        sock.close()
                        read_fds.remove(sock)
                        check_try_count()
                                                                    
                else:
                    
                    #receivedData = sock.recv(4096)
                    #if receivedData:
                    #    print("Data Received")
                    #    print(receivedData)

                    # Check the receivedData
                    try:
                        iflag = bytearray()
                        ilength = bytearray()
                        chunk = bytearray()
                        
                        iflag = sock.recv(2)
                        ilength = sock.recv(2)
                                                
                        flag = bytes_to_int(iflag)
                        length = bytes_to_int(ilength)

                        flag = socket.htons(flag)
                        length = socket.htons(length)
                        
                        print "Flag:"
                        print flag
                        print "length:"
                        print length
                        #logging.debug("Flag: %d" % flag)
                        #logging.debug("length: %d" % length)
                        
                        while len(chunk) < length:
                            
                            chunk = chunk +  sock.recv(length - len(chunk))
                            print "chunk len:"
                            print len(chunk)
                            #logging.debug("chunk len: %d" % len(chunk))
                            #Apply new configuration. We'd like to be able to
                            #create a StringIO and pass that in, but unfortunately
                            #1.5.2 ConfigParser does not support reading file
                            #objects, only actual files. So we create a temporary
                            #file and remove it later.
                        
                        self.processing_data(flag,chunk)

                    except socket.error, e:
                        print "socket.error!"
                        print e
                        #logging.debug("socket.error!: %s" % str(e))
                        sock.close()
                        read_fds.remove(sock)
                        print "Dropped connection"
                        #logging.debug("Dropped connection")
                        check_try_count()
                    except IOError as ioe:
                        print "IOError!"
                        #logging.debug("IOError!")
                        sock.close()
                        read_fds.remove(sock)
                        print(str(ioe))
                        #logging.debug(str(ioe))
                        check_try_count()
                    except Exception as err:
                        print "Exception!"
                        #logging.debug("Exception!")
                        sock.close()
                        read_fds.remove(sock)
                        print(str(err))
                        #logging.debug(str(err))
                        check_try_count()
                    except :
                        print "Unknown error on receve data."
                        #logging.debug("Unknown error on receve data.")
                        sock.close()
                        read_fds.remove(sock)
                        check_try_count()
                    ##################################################################
                    #if (receivedData == "$"):
                    #   buffer = "$"   # Found the command start, reset the buffer
                    #elif ( (receivedData <> "!") and (receivedData <> "%") ):
                    #   buffer += receivedData
                    #elif ( (receivedData == "!") or (receivedData == "%") ):
                    #   buffer += receivedData
                    #   # Copy the valid command into the global buffer
                    #   TCPServerCommand = buffer
                    #   # Send back ACK to client
                    #   sock.send("OK"+chr(0x0D)+chr(0x0A))
                    #   print("Command --> " + buffer)
                    #   # Check if is a termination request
                    #   if (buffer == "$QUIT%"):
                    #      sock.close()
                    #      read_fds.remove(sock)
                    #      self.stopflag = 1
                    #   else:
                    #       sock.close()
                    #       read_fds.remove(sock)
                    #       print "Dropped connection",address[0]
                    ##################################################################
            time.sleep(0.01)
        print ("Exit from run")
        #logging.debug("Exit from run")
        return
   
    def stopserver(self):
        print ("(quitthread) Server shutdown!")
        #logging.debug("(quitthread) Server shutdown!")
        self.stopflag = 1
        return

def start_server():
   
    try:
        # Set & Start Logging
        check_n_create_log_file()
        
        print "Nonblocking TCP Server Started!"
        #logging.debug("Nonblocking TCP Server Started!")
        print "Listening on port 2626"
        #logging.debug("Listening on port 2626")

        #logging.debug("test log")
        # Setup a TCP server
        TCPserver = TCPThreadedServer(2626)
        TCPserver.setDaemon(True)
        TCPserver.start()

        # Setup a TCP client
        c = Client()
        c.setDaemon(True)
        c.start()
        
        # initialize the command buffer input
        TCPServerCommand = ""
      
        # Infinite loop (keep alive the task)
        while 1:
            try:
                time.sleep(0.01)
                if (TCPServerCommand != ""):
                    print("New command! " + TCPServerCommand)
                    #logging.debug("New command! %s" % TCPServerCommand)
                if (TCPServerCommand == "$QUIT%"):
                    break
                TCPServerCommand = ""
                pass
            except Exception as err:
                print "Exception ServerCommand!"
                #logging.debug("Exception ServerCommand!")
                print(str(err))
                #logging.debug(str(err))
                check_try_count()
            except :
                print "Unknown error on ServerCommand."
                #logging.debug("Unknown error on ServerCommand.")                                
                check_try_count()
                

         
    except (KeyboardInterrupt, SystemExit):
        print "(main) Exception detected"
        #logging.debug("(main) Exception detected")
        TCPserver.stopserver()
        
    finally:
        time.sleep(1)
        print "(main) Exiting from main program"
        #logging.debug("(main) Exiting from main program")
        os.system("sudo reboot")                         

if __name__ == "__main__":
    start_server()






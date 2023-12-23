from scapy.all import *
from scapy.layers.inet import *




def main():
    """Driver function"""
    while True:
        print_menu()
        option = input('Choose a menu option: ')
        if option == '1':
            print("Creating and sending packets ...")
            # TODO

            #send_ping()
            #continue
            number = int(input("Enter the number of packets to send: "))
            interval = int(input("Enter the interval between packets: "))
            send_pkt(number, interval)
            
        elif option == '2':
            print("Listening to all traffic to 8.8.4.4 for 1 minute ...")
            sniffed = sniff(filter="tcp", timeout=60)
            print_pkt(sniffed)
            # TODO
        elif option == '3':
            print("Listening continuously to only ping commands to 8.8.4.4 ...")
            
            def reply_to_ping(pkt):
                if IP in pkt and pkt[IP].dst == "8.8.4.4":
                    print("Source IP:", pkt[IP].src)
                    print("Destination IP:", pkt[IP].dst)
                    print("Protocol:", pkt[IP].proto)
                    print("TTL:", pkt[IP].ttl)
                    print("Length in bytes:", len(pkt))
                    if Raw in pkt:
                        print("Raw payload:", pkt[Raw].load)
                    else:
                        print("Raw payload: None")
                    print("-------------")


            # Start sniffing for ICMP packets
            try:
                sniff(filter="icmp and host 8.8.4.4", prn=reply_to_ping)
            except KeyboardInterrupt:
                print("Sniffing stopped by the user.")



        elif option == '4':
            print("Listening continuously to only outgoing telnet commands ...")
            # TODO
            def reply_to_telnet(pkt):
                if IP in pkt and pkt[IP].dst == "8.8.4.4" and pkt[TCP].dport == 23:
                    print("Source IP:", pkt[IP].src)
                    print("Destination IP:", pkt[IP].dst)
                    print("Protocol:", pkt[IP].proto)
                    print("TTL:", pkt[IP].ttl)
                    print("Length in bytes:", len(pkt))
                    if Raw in pkt:
                        print("Raw payload:", pkt[Raw].load)
                    else:
                        print("Raw payload: None")
                    print("-------------")
            try:
                sniff(filter = "host 8.8.4.4 and tcp", prn = reply_to_telnet)
            except KeyboardInterrupt:
                print("Sniffing stopped by the user.")

            
            
        elif option == '5':
            print("End")
            break
        else:
            print(f"\nInvalid entry\n")

def send_ping():
    """Send a single ping packet to
    """
    ping = IP(dst="8.8.4.4")/ICMP() # this is the stack !
    reply = sr1(ping)
    print("sent ping")
    print(reply.summary())



def send_pkt(number, interval):

    """
    Send a custom packet with the following fields

    #### Ethernet layer
    - Source MAC address: 00:11:22:33:44:55
    - Destination MAC address: 55:44:33:22:11:00

    #### IP layer
    - Source address: 192.168.10.4
    - Destination address: 8.8.4.4
    - Protocol: TCP
    - TTL: 26

    #### TCP layer
    - Source port: 23
    - Destination port: 80

    #### Raw payload
    - Payload: "RISC-V Education: https://riscvedu.org/"
    """
    ether = Ether(src="00:11:22:33:44:55", dst="55:44:33:22:11:00")
    ip = IP(src="192.168.10.4", dst="8.8.4.4", proto = 'tcp', ttl=26)  
    tcp = TCP(sport=23, dport=80)
    raw = Raw(load="RISC-V Education: https://riscvedu.org/")
    sendp(ether/ip/tcp/raw, count=number, inter=interval)



    pass

def print_single_pkt(pkt):
    """ 
    Print Packet fields

    - Source IP
    - Destination IP
    - Protocol number
    - TTL
    - Length in bytes
    - Raw payload (if any)
    """
    
    if IP in pkt and pkt[IP].dst == "8.8.4.4":
        print("Source IP:", pkt[IP].src)
        print("Destination IP:", pkt[IP].dst)
        print("Protocol:", pkt[IP].proto)
        print("TTL:", pkt[IP].ttl)
        if Raw in pkt:
            print("Length:", len(pkt))
            print("Raw payload:", pkt[Raw].load)
        print("-------------")

    # TODO
    pass
def print_pkt(pkt_list, dest_ip = "8.8.4.4"):
    """ 
    Print Packet fields

    - Source IP
    - Destination IP
    - Protocol number
    - TTL
    - Length in bytes
    - Raw payload (if any)
    """
    
    for pkt in pkt_list:
        
        if IP in pkt and pkt[IP].dst == dest_ip:
            print("Source IP:", pkt[IP].src)
            print("Destination IP:", pkt[IP].dst)
            print("Protocol:", pkt[IP].proto)
            print("TTL:", pkt[IP].ttl)
            if Raw in pkt:
                print("Length:", len(pkt))
                print("Raw payload:", pkt[Raw].load)
            print("-------------")
        

    # TODO
    pass


def print_menu():
    """Prints the menu of options"""
    print("*******************Main Menu*******************")
    print('1. Create and send packets')
    print('2. Listen to all traffic to 8.8.4.4 for 1 minute')
    print('3. Listen continuously to only ping commands to 8.8.4.4')
    print('4. Listen continuously to only outgoing telnet commands')
    print('5. Quit')
    print('***********************************************\n')


main()

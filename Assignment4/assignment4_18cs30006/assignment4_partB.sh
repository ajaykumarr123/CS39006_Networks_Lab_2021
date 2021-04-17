#adding namespaces
ip netns add H1
ip netns add H2
ip netns add H3
ip netns add H4
ip netns add R1
ip netns add R2
ip netns add R3

#creating veth pair's
ip link add v1 type veth peer name v2
ip link add v3 type veth peer name v4
ip link add v5 type veth peer name v6
ip link add v7 type veth peer name v8
ip link add v9 type veth peer name v10
ip link add v11 type veth peer name v12

#Associate the veth's with namespaces
ip link set v1 netns H1
ip link set v2 netns R1
ip link set v3 netns H2
ip link set v4 netns R1
ip link set v5 netns R1
ip link set v6 netns R2
ip link set v7 netns R2
ip link set v8 netns R3
ip link set v9 netns R3
ip link set v10 netns H3
ip link set v11 netns R3
ip link set v12 netns H4

#Configuring the veth interfaces and set it up.
ip netns exec H1 ip addr add 10.0.10.06/24 dev v1
ip netns exec H1 ip link set dev v1 up
ip netns exec R1 ip addr add 10.0.10.07/24 dev v2
ip netns exec R1 ip link set dev v2 up
ip netns exec H2 ip addr add 10.0.20.06/24 dev v3
ip netns exec H2 ip link set dev v3 up
ip netns exec R1 ip addr add 10.0.20.07/24 dev v4
ip netns exec R1 ip link set dev v4 up
ip netns exec R1 ip addr add 10.0.30.06/24 dev v5
ip netns exec R1 ip link set dev v5 up
ip netns exec R2 ip addr add 10.0.30.07/24 dev v6
ip netns exec R2 ip link set dev v6 up
ip netns exec R2 ip addr add 10.0.40.06/24 dev v7
ip netns exec R2 ip link set dev v7 up
ip netns exec R3 ip addr add 10.0.40.07/24 dev v8
ip netns exec R3 ip link set dev v8 up
ip netns exec R3 ip addr add 10.0.50.06/24 dev v9
ip netns exec R3 ip link set dev v9 up
ip netns exec H3 ip addr add 10.0.50.07/24 dev v10
ip netns exec H3 ip link set dev v10 up
ip netns exec R3 ip addr add 10.0.60.06/24 dev v11
ip netns exec R3 ip link set dev v11 up
ip netns exec H4 ip addr add 10.0.60.07/24 dev v12
ip netns exec H4 ip link set dev v12 up

# Enable loopback interface lo
ip netns exec H1 ip link set dev lo up
ip netns exec H2 ip link set dev lo up
ip netns exec H3 ip link set dev lo up
ip netns exec H4 ip link set dev lo up
ip netns exec R1 ip link set dev lo up
ip netns exec R2 ip link set dev lo up
ip netns exec R3 ip link set dev lo up

# configuring all the required routes
ip netns exec H1 ip route add default via 10.0.10.07 dev v1
ip netns exec H2 ip route add default via 10.0.20.07 dev v3
ip netns exec H3 ip route add default via 10.0.50.06 dev v10
ip netns exec H4 ip route add default via 10.0.60.06 dev v12

ip netns exec R1 ip route add default via 10.0.30.07 dev v5

ip netns exec R2 ip route add 10.0.10.0/24 via 10.0.30.06 dev v6
ip netns exec R2 ip route add 10.0.20.0/24 via 10.0.30.06 dev v6
ip netns exec R2 ip route add 10.0.50.0/24 via 10.0.40.07 dev v7
ip netns exec R2 ip route add 10.0.60.0/24 via 10.0.40.07 dev v7

ip netns exec R3 ip route add default via 10.0.40.06 dev v8


#Enable ipv4 ip forwarding
sysctl -w net.ipv4.ip_forward=1

#ping commands
 
echo "\n******************************PING from H1******************************\n" 
ip netns exec H1 ping -c 3 10.0.10.6
ip netns exec H1 ping -c 3 10.0.10.7
ip netns exec H1 ping -c 3 10.0.20.6
ip netns exec H1 ping -c 3 10.0.20.7
ip netns exec H1 ping -c 3 10.0.30.6
ip netns exec H1 ping -c 3 10.0.30.7
ip netns exec H1 ping -c 3 10.0.40.6
ip netns exec H1 ping -c 3 10.0.40.7
ip netns exec H1 ping -c 3 10.0.50.6
ip netns exec H1 ping -c 3 10.0.50.7
ip netns exec H1 ping -c 3 10.0.60.6
ip netns exec H1 ping -c 3 10.0.60.7

 
echo "\n******************************PING from H2******************************\n"

ip netns exec H2 ping -c 3 10.0.10.6
ip netns exec H2 ping -c 3 10.0.10.7
ip netns exec H2 ping -c 3 10.0.20.6
ip netns exec H2 ping -c 3 10.0.20.7
ip netns exec H2 ping -c 3 10.0.30.6
ip netns exec H2 ping -c 3 10.0.30.7
ip netns exec H2 ping -c 3 10.0.40.6
ip netns exec H2 ping -c 3 10.0.40.7
ip netns exec H2 ping -c 3 10.0.50.6
ip netns exec H2 ping -c 3 10.0.50.7
ip netns exec H2 ping -c 3 10.0.60.6
ip netns exec H2 ping -c 3 10.0.60.7

 
echo "\n******************************PING from H3******************************\n" 
ip netns exec H3 ping -c 3 10.0.10.6
ip netns exec H3 ping -c 3 10.0.10.7
ip netns exec H3 ping -c 3 10.0.20.6
ip netns exec H3 ping -c 3 10.0.20.7
ip netns exec H3 ping -c 3 10.0.30.6
ip netns exec H3 ping -c 3 10.0.30.7
ip netns exec H3 ping -c 3 10.0.40.6
ip netns exec H3 ping -c 3 10.0.40.7
ip netns exec H3 ping -c 3 10.0.50.6
ip netns exec H3 ping -c 3 10.0.50.7
ip netns exec H3 ping -c 3 10.0.60.6
ip netns exec H3 ping -c 3 10.0.60.7

 
echo "\n******************************PING from H4******************************\n" 
ip netns exec H4 ping -c 3 10.0.10.6
ip netns exec H4 ping -c 3 10.0.10.7
ip netns exec H4 ping -c 3 10.0.20.6
ip netns exec H4 ping -c 3 10.0.20.7
ip netns exec H4 ping -c 3 10.0.30.6
ip netns exec H4 ping -c 3 10.0.30.7
ip netns exec H4 ping -c 3 10.0.40.6
ip netns exec H4 ping -c 3 10.0.40.7
ip netns exec H4 ping -c 3 10.0.50.6
ip netns exec H4 ping -c 3 10.0.50.7
ip netns exec H4 ping -c 3 10.0.60.6
ip netns exec H4 ping -c 3 10.0.60.7

 
echo "\n******************************PING from R1******************************\n" 
ip netns exec R1 ping -c 3 10.0.10.6
ip netns exec R1 ping -c 3 10.0.10.7
ip netns exec R1 ping -c 3 10.0.20.6
ip netns exec R1 ping -c 3 10.0.20.7
ip netns exec R1 ping -c 3 10.0.30.6
ip netns exec R1 ping -c 3 10.0.30.7
ip netns exec R1 ping -c 3 10.0.40.6
ip netns exec R1 ping -c 3 10.0.40.7
ip netns exec R1 ping -c 3 10.0.50.6
ip netns exec R1 ping -c 3 10.0.50.7
ip netns exec R1 ping -c 3 10.0.60.6
ip netns exec R1 ping -c 3 10.0.60.7

 
echo "\n******************************PING from R2******************************\n" 
ip netns exec R2 ping -c 3 10.0.10.6
ip netns exec R2 ping -c 3 10.0.10.7
ip netns exec R2 ping -c 3 10.0.20.6
ip netns exec R2 ping -c 3 10.0.20.7
ip netns exec R2 ping -c 3 10.0.30.6
ip netns exec R2 ping -c 3 10.0.30.7
ip netns exec R2 ping -c 3 10.0.40.6
ip netns exec R2 ping -c 3 10.0.40.7
ip netns exec R2 ping -c 3 10.0.50.6
ip netns exec R2 ping -c 3 10.0.50.7
ip netns exec R2 ping -c 3 10.0.60.6
ip netns exec R2 ping -c 3 10.0.60.7

 
echo "\n******************************PING from R3******************************\n" 
ip netns exec R3 ping -c 3 10.0.10.6
ip netns exec R3 ping -c 3 10.0.10.7
ip netns exec R3 ping -c 3 10.0.20.6
ip netns exec R3 ping -c 3 10.0.20.7
ip netns exec R3 ping -c 3 10.0.30.6
ip netns exec R3 ping -c 3 10.0.30.7
ip netns exec R3 ping -c 3 10.0.40.6
ip netns exec R3 ping -c 3 10.0.40.7
ip netns exec R3 ping -c 3 10.0.50.6
ip netns exec R3 ping -c 3 10.0.50.7
ip netns exec R3 ping -c 3 10.0.60.6
ip netns exec R3 ping -c 3 10.0.60.7



echo "\n************************TraceRoute Commands****************************\n"
echo "\n                        HOPS from H1 to H3                            \n"
ip netns exec H1 traceroute 10.0.50.7


echo
echo "\n                        HOPS from H3 to H4                            \n"
ip netns exec H3 traceroute 10.0.60.7


echo
echo "\n                        HOPS from H4 to H2                            \n"
ip netns exec H4 traceroute 10.0.20.6
#adding namespaces
ip netns add N1
ip netns add N2
ip netns add N3
ip netns add N4

#creating veth pair's
ip link add v1 type veth peer name v2
ip link add v3 type veth peer name v4
ip link add v5 type veth peer name v6

#Associate the veth's with namespaces
ip link set v1 netns N1
ip link set v2 netns N2
ip link set v3 netns N2
ip link set v4 netns N3
ip link set v5 netns N3
ip link set v6 netns N4

#Configuring the veth interfaces and set it up.
ip netns exec N1 ip addr add 10.0.10.06/24 dev v1
ip netns exec N1 ip link set dev v1 up
ip netns exec N2 ip addr add 10.0.10.07/24 dev v2
ip netns exec N2 ip link set dev v2 up
ip netns exec N2 ip addr add 10.0.20.06/24 dev v3
ip netns exec N2 ip link set dev v3 up
ip netns exec N3 ip addr add 10.0.20.07/24 dev v4
ip netns exec N3 ip link set dev v4 up
ip netns exec N3 ip addr add 10.0.30.06/24 dev v5
ip netns exec N3 ip link set dev v5 up
ip netns exec N4 ip addr add 10.0.30.07/24 dev v6
ip netns exec N4 ip link set dev v6 up

# Enable loopback interface lo
ip netns exec N1 ip link set dev lo up
ip netns exec N2 ip link set dev lo up
ip netns exec N3 ip link set dev lo up
ip netns exec N4 ip link set dev lo up

# configuring all the required routes

ip netns exec N2 ip route add 10.0.30.0/24 via 10.0.20.07 dev v3
ip netns exec N3 ip route add 10.0.10.0/24 via 10.0.20.06 dev v4
ip netns exec N1 ip route add default via 10.0.10.07 dev v1
ip netns exec N4 ip route add default via 10.0.30.06 dev v6

#Enable ipv4 ip forwarding
sysctl -w net.ipv4.ip_forward=1

#ping commands
echo "\n******************************PING from N1******************************\n" 
ip netns exec N1 ping -c 3 10.0.10.6
ip netns exec N1 ping -c 3 10.0.10.7
ip netns exec N1 ping -c 3 10.0.20.6
ip netns exec N1 ping -c 3 10.0.20.7
ip netns exec N1 ping -c 3 10.0.30.6
ip netns exec N1 ping -c 3 10.0.30.7

echo "\n******************************PING from N2******************************\n" 
ip netns exec N2 ping -c 3 10.0.10.6
ip netns exec N2 ping -c 3 10.0.10.7
ip netns exec N2 ping -c 3 10.0.20.6
ip netns exec N2 ping -c 3 10.0.20.7
ip netns exec N2 ping -c 3 10.0.30.6
ip netns exec N2 ping -c 3 10.0.30.7

echo "\n******************************PING from N3******************************\n" 
ip netns exec N3 ping -c 3 10.0.10.6
ip netns exec N3 ping -c 3 10.0.10.7
ip netns exec N3 ping -c 3 10.0.20.6
ip netns exec N3 ping -c 3 10.0.20.7
ip netns exec N3 ping -c 3 10.0.30.6
ip netns exec N3 ping -c 3 10.0.30.7


echo "\n******************************PING from N4******************************\n" 
ip netns exec N4 ping -c 3 10.0.10.6
ip netns exec N4 ping -c 3 10.0.10.7
ip netns exec N4 ping -c 3 10.0.20.6
ip netns exec N4 ping -c 3 10.0.20.7
ip netns exec N4 ping -c 3 10.0.30.6
ip netns exec N4 ping -c 3 10.0.30.7

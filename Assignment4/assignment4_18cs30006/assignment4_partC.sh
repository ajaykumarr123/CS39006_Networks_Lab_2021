#adding namespaces
ip netns add N1
ip netns add N2
ip netns add N3
ip netns add N4
ip netns add N5
ip netns add N6


#creating veth pair's
ip link add v1 type veth peer name v2
ip link add v3 type veth peer name v4
ip link add v5 type veth peer name v6
ip link add v7 type veth peer name v8
ip link add v9 type veth peer name v10
ip link add v11 type veth peer name v12


#Associate the veth's with namespaces
ip link set v1 netns N1
ip link set v2 netns N2
ip link set v3 netns N2
ip link set v4 netns N3
ip link set v5 netns N3
ip link set v6 netns N4
ip link set v7 netns N4
ip link set v8 netns N5
ip link set v9 netns N5
ip link set v10 netns N6
ip link set v11 netns N6
ip link set v12 netns N1

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
ip netns exec N4 ip addr add 10.0.40.06/24 dev v7
ip netns exec N4 ip link set dev v7 up
ip netns exec N5 ip addr add 10.0.40.07/24 dev v8
ip netns exec N5 ip link set dev v8 up
ip netns exec N5 ip addr add 10.0.50.06/24 dev v9
ip netns exec N5 ip link set dev v9 up
ip netns exec N6 ip addr add 10.0.50.07/24 dev v10
ip netns exec N6 ip link set dev v10 up
ip netns exec N6 ip addr add 10.0.60.06/24 dev v11
ip netns exec N6 ip link set dev v11 up
ip netns exec N1 ip addr add 10.0.60.07/24 dev v12
ip netns exec N1 ip link set dev v12 up

# Enable loopback interface lo
ip netns exec N1 ip link set dev lo up
ip netns exec N2 ip link set dev lo up
ip netns exec N3 ip link set dev lo up
ip netns exec N4 ip link set dev lo up
ip netns exec N5 ip link set dev lo up
ip netns exec N6 ip link set dev lo up

# configuring all the required routes
ip netns exec N1 ip route add default via 10.0.10.07 dev v1
ip netns exec N2 ip route add default via 10.0.20.07 dev v3
ip netns exec N3 ip route add default via 10.0.30.07 dev v5
ip netns exec N4 ip route add default via 10.0.40.07 dev v7
ip netns exec N5 ip route add default via 10.0.50.07 dev v9
ip netns exec N6 ip route add default via 10.0.60.07 dev v11


#Enable ipv4 ip forwarding
sysctl -w net.ipv4.ip_forward=1


echo "\n***********************TraceRoute Commands**************************\n"
echo "                       HOST from N1 to N5                           \n"
ip netns exec N1 traceroute 10.0.40.7
echo "\n                       HOST from N3 to N5                           \n"
ip netns exec N3 traceroute 10.0.40.7
echo "\n                       HOST from N3 to N1                           \n"
sleep 3
ip netns exec N3 traceroute 10.0.60.7
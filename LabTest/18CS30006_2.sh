#adding namespaces
ip netns add H1
ip netns add H2
ip netns add H3
ip netns add H4
ip netns add R1
ip netns add R2
ip netns add R3
ip netns add R4
ip netns add R5
ip netns add R6

#creating veth pair's
ip link add v1 type veth peer name v2
ip link add v3 type veth peer name v4
ip link add v5 type veth peer name v6
ip link add v7 type veth peer name v8
ip link add v9 type veth peer name v10
ip link add v11 type veth peer name v12
ip link add v13 type veth peer name v14
ip link add v15 type veth peer name v16
ip link add v17 type veth peer name v18


#Associate the veth's with namespaces
ip link set v1 netns H1
ip link set v2 netns R1
ip link set v3 netns R1
ip link set v4 netns R2
ip link set v5 netns R2
ip link set v6 netns R3
ip link set v7 netns R3
ip link set v8 netns H2
ip link set v9 netns R2
ip link set v10 netns R4
ip link set v11 netns R4
ip link set v12 netns R5
ip link set v13 netns R5
ip link set v14 netns H3
ip link set v15 netns R4
ip link set v16 netns R6
ip link set v17 netns R6
ip link set v18 netns H4

#Configuring the veth interfaces and set it up.
ip netns exec H1 ip addr add 10.10.10.06/24 dev v1
ip netns exec H1 ip link set dev v1 up
ip netns exec R1 ip addr add 10.10.10.07/24 dev v2
ip netns exec R1 ip link set dev v2 up
ip netns exec R1 ip addr add 10.10.20.06/24 dev v3
ip netns exec R1 ip link set dev v3 up
ip netns exec R2 ip addr add 10.10.20.07/24 dev v4
ip netns exec R2 ip link set dev v4 up
ip netns exec R2 ip addr add 10.10.30.06/24 dev v5
ip netns exec R2 ip link set dev v5 up
ip netns exec R3 ip addr add 10.10.30.07/24 dev v6
ip netns exec R3 ip link set dev v6 up
ip netns exec R3 ip addr add 10.10.40.06/24 dev v7
ip netns exec R3 ip link set dev v7 up
ip netns exec H2 ip addr add 10.10.40.07/24 dev v8
ip netns exec H2 ip link set dev v8 up
ip netns exec R2 ip addr add 10.10.50.06/24 dev v9
ip netns exec R2 ip link set dev v9 up
ip netns exec R4 ip addr add 10.10.50.07/24 dev v10
ip netns exec R4 ip link set dev v10 up
ip netns exec R4 ip addr add 10.20.10.06/24 dev v11
ip netns exec R4 ip link set dev v11 up
ip netns exec R5 ip addr add 10.20.10.07/24 dev v12
ip netns exec R5 ip link set dev v12 up
ip netns exec R5 ip addr add 10.20.20.06/24 dev v13
ip netns exec R5 ip link set dev v13 up
ip netns exec H3 ip addr add 10.20.20.07/24 dev v14
ip netns exec H3 ip link set dev v14 up
ip netns exec R4 ip addr add 10.30.10.06/24 dev v15
ip netns exec R4 ip link set dev v15 up
ip netns exec R6 ip addr add 10.30.10.07/24 dev v16
ip netns exec R6 ip link set dev v16 up
ip netns exec R6 ip addr add 10.30.20.06/24 dev v17
ip netns exec R6 ip link set dev v17 up
ip netns exec H4 ip addr add 10.30.20.07/24 dev v18
ip netns exec H4 ip link set dev v18 up

# Enable loopback interface lo
ip netns exec H1 ip link set dev lo up
ip netns exec H2 ip link set dev lo up
ip netns exec H3 ip link set dev lo up
ip netns exec H4 ip link set dev lo up
ip netns exec R1 ip link set dev lo up
ip netns exec R2 ip link set dev lo up
ip netns exec R3 ip link set dev lo up
ip netns exec R4 ip link set dev lo up
ip netns exec R5 ip link set dev lo up
ip netns exec R6 ip link set dev lo up

# configuring all the required routes
ip netns exec H1 ip route add default via 10.10.10.07 dev v1
ip netns exec H2 ip route add default via 10.10.40.06 dev v8
ip netns exec H3 ip route add default via 10.20.20.06 dev v14
ip netns exec H4 ip route add default via 10.30.20.06 dev v18

ip netns exec R1 ip route add default via 10.10.20.07 dev v3
ip netns exec R3 ip route add default via 10.10.30.06 dev v6
ip netns exec R5 ip route add default via 10.20.10.06 dev v12
ip netns exec R6 ip route add default via 10.30.10.06 dev v16

sudo ip netns exec R2 ip route add 10.20.20.0/24 via 10.10.50.07 dev v9
sudo ip netns exec R2 ip route add 10.30.20.0/24 via 10.10.50.07 dev v9
sudo ip netns exec R2 ip route add 10.30.10.0/24 via 10.10.50.07 dev v9
sudo ip netns exec R2 ip route add 10.20.10.0/24 via 10.10.50.07 dev v9
sudo ip netns exec R2 ip route add 10.10.10.0/24 via 10.10.20.06 dev v4
sudo ip netns exec R2 ip route add 10.10.40.0/24 via 10.10.30.07 dev v5

sudo ip netns exec R4 ip route add 10.10.10.0/24 via 10.10.50.06 dev v10
sudo ip netns exec R4 ip route add 10.10.40.0/24 via 10.10.50.06 dev v10
sudo ip netns exec R4 ip route add 10.10.20.0/24 via 10.10.50.06 dev v10
sudo ip netns exec R4 ip route add 10.10.30.0/24 via 10.10.50.06 dev v10
sudo ip netns exec R4 ip route add 10.20.20.0/24 via 10.20.10.07 dev v11
sudo ip netns exec R4 ip route add 10.30.20.0/24 via 10.30.10.07 dev v15



#Enable ipv4 ip forwarding
sysctl -w net.ipv4.ip_forward=1


echo traceroute from H1 to H4
sudo ip netns exec H1 traceroute 10.30.20.07
echo "----------------------------------------------------------------------------------------"

echo traceroute from H3 to H2
sudo ip netns exec H3 traceroute 10.10.40.07
echo "----------------------------------------------------------------------------------------"

echo traceroute from H4 to H3
sudo ip netns exec H4 traceroute 10.20.20.07
echo "----------------------------------------------------------------------------------------"

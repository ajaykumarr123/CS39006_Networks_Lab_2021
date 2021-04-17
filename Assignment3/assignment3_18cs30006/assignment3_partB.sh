#adding namespaces
ip netns add H1
ip netns add H2
ip netns add H3
ip netns add R

#creating veth pair's
ip link add veth1 type veth peer name veth2
ip link add veth3 type veth peer name veth6
ip link add veth4 type veth peer name veth5

#Associate the non-bridge side with namespaces
ip link set veth1 netns H1
ip link set veth2 netns R
ip link set veth3 netns R
ip link set veth4 netns R
ip link set veth5 netns H3
ip link set veth6 netns H2

#Configuring the veth interfaces and set it up.
ip netns exec H1 ip addr add 10.0.10.06/24 dev veth1
ip netns exec H1 ip link set dev veth1 up
ip netns exec R ip addr add 10.0.10.1/24 dev veth2
ip netns exec R ip link set dev veth2 up
ip netns exec R ip addr add 10.0.20.1/24 dev veth3
ip netns exec R ip link set dev veth3 up
ip netns exec R ip addr add 10.0.30.1/24 dev veth4
ip netns exec R ip link set dev veth4 up
ip netns exec H3 ip addr add 10.0.30.06/24 dev veth5
ip netns exec H3 ip link set dev veth5 up
ip netns exec H2 ip addr add 10.0.20.06/24 dev veth6
ip netns exec H2 ip link set dev veth6 up


#Create the bridge device 'bridge_R'
#and set it up:
ip netns exec R brctl addbr bridge_R
ip netns exec R ip link set dev bridge_R up

#Add interfaces to the bridge.
ip netns exec R brctl addif bridge_R veth2 veth3 veth4

#Enable ipv4 ip forwarding at R 
ip netns exec R sysctl -w net.ipv4.ip_forward=1

#configure route
ip netns exec H1 ip route add default via 10.0.10.06
ip netns exec H2 ip route add default via 10.0.20.06
ip netns exec H3 ip route add default via 10.0.30.06


#To ping run command E.g.:-
# sudo ip netns exec H1 ping 10.0.20.06

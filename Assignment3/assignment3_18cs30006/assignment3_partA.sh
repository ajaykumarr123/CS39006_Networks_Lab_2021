#creating two namespaces
ip netns add ns1
ip netns add ns2

#creating veth pair
ip link add Veth0 type veth peer name Veth1

#attaching two virtual ethernet (veth) interfaces with the two namespaces
ip link set Veth0 netns ns1
ip link set Veth1 netns ns2

#Configuring the two interfaces(Veth0, Veth1)
ip netns exec ns1 ip addr add 10.1.1.0/24 dev Veth0
ip netns exec ns1 ip link set dev Veth0 up

ip netns exec ns2 ip addr add 10.1.2.0/24 dev Veth1
ip netns exec ns2 ip link set dev Veth1 up

#configuring route
ip netns exec ns1 ip route add default via 10.1.1.0
ip netns exec ns2 ip route add default via 10.1.2.0


#To ping run:
#sudo ip netns exec ns1 ping 10.1.2.0
#         or
#sudo ip netns exec ns12 ping 10.1.1.0


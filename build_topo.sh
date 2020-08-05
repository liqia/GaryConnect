ip netns add server
ip netns add gateway
ip netns add client
ip netns exec gateway sysctl net.ipv4.ip_forward=1
ip link add svr-veth type veth peer name svrgw-veth
ip link add cli-veth type veth peer name cligw-veth
#ip link show | grep veth


ip link set svr-veth netns server
ip link set svrgw-veth netns gateway
ip link set cligw-veth netns gateway
ip link set cli-veth netns client
#ip link show | grep veth


ip netns exec server ifconfig svr-veth 192.168.100.1
ip netns exec gateway ifconfig svrgw-veth 192.168.100.254
ip netns exec gateway ifconfig cligw-veth 10.0.100.254
ip netns exec client ifconfig cli-veth 10.0.100.1


ip netns exec client route add default gw 10.0.100.254
ip netns exec server route add default gw 192.168.100.254

#test 
#ip netns exec client ping 192.168.100.1 -I 10.0.100.1

#./client -a 10.10.1.1 -b 6667 -c 0.0.0.0 -d 6666
#./server -l 0.0.0.0 -p 6666
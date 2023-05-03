#include <iostream>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//

using namespace ns3;

int main(int argc, char *argv[]){
        
        Config::SetDefault ("ns3::CsmaNetDevice::EncapsulationMode", StringValue("Dix"));
    
        NodeContainer c;
        c.Create(5);
        
        NodeContainer c0 = NodeContainer(c.Get(0), c.Get(1));
        NodeContainer c1 = NodeContainer(c.Get(1), c.Get(2), c.Get(3), c.Get(4));
        
        PointToPointHelper p2p;
        p2p.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
        p2p.SetChannelAttribute("Delay", StringValue("2ms"));
        
        CsmaHelper csma;
        csma.SetChannelAttribute("DataRate", StringValue("10Mbps"));
        csma.SetChannelAttribute("Delay", StringValue("2ms"));
        
        NetDeviceContainer nd0 = p2p.Install(c0);
        NetDeviceContainer nd1 = csma.Install(c1);
        
        InternetStackHelper internet;
        internet.Install(c);
        
        Ipv4AddressHelper ipv4Addr;
        ipv4Addr.SetBase("10.1.1.0", "255.255.255.0");
        ipv4Addr.Assign(nd0);
        ipv4Addr.SetBase("10.1.2.0", "255.255.255.0");
        ipv4Addr.Assign(nd1);
        
        Ipv4Address multicastSource("10.1.1.1");
        Ipv4Address multicastGroup("225.1.2.4");
        
        Ipv4StaticRoutingHelper multicast;
        Ptr<Node> multicastRouter = c.Get(1);
        Ptr<NetDevice> inputIf = nd0.Get(1);


        NetDeviceContainer outputDevices;
        outputDevices.Add(nd1.Get(0));
        multicast.AddMulticastRoute(multicastRouter, multicastSource, multicastGroup, inputIf, outputDevices);
        
        Ptr<Node> sender = c.Get (0);
        Ptr<NetDevice> senderIf = nd0.Get(0);
        multicast.SetDefaultMulticastRoute(sender, senderIf);
        
        uint16_t multicastPort = 9;
        
        OnOffHelper onoff("ns3::UdpSocketFactory", Address(InetSocketAddress(multicastGroup, multicastPort)));
        onoff.SetConstantRate(DataRate("255b/s"));
        onoff.SetAttribute("PacketSize", UintegerValue(128));
        
        ApplicationContainer srcC = onoff.Install(c0.Get(0));
        srcC.Start(Seconds (1.0));
        srcC.Stop(Seconds (10.0));
        
        PacketSinkHelper sink("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), multicastPort));
        
        ApplicationContainer sinkC = sink.Install (c1.Get (2));
        sinkC.Start(Seconds(1.0));
        sinkC.Stop(Seconds(5.0));
        
        
        AnimationInterface anim("l4q1m.xml");
        
        Simulator::Run ();
        Simulator::Destroy ();
        
        return 0;
}

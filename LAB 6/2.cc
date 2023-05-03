#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/netanim-module.h"

using namespace ns3;

void TearDownLink (Ptr<Node> nodeA, Ptr<Node> nodeB, uint32_t interfaceA, uint32_t interfaceB){
        nodeA->GetObject<Ipv4>()->SetDown(interfaceA);
        nodeB->GetObject<Ipv4>()->SetDown(interfaceB);
}

int main(int argc, char **argv){
        bool verbose = false;
        bool printRoutingTables = true;
        bool showPings = false;
        std::string SplitHorizon("PoisonReverse");
        
        if (SplitHorizon == "NoSplitHorizon") Config::SetDefault ("ns3::Rip::SplitHorizon", EnumValue (RipNg::NO_SPLIT_HORIZON));
        else if (SplitHorizon == "SplitHorizon") Config::SetDefault ("ns3::Rip::SplitHorizon", EnumValue (RipNg::SPLIT_HORIZON));
        else Config::SetDefault ("ns3::Rip::SplitHorizon", EnumValue (RipNg::POISON_REVERSE));
        
        Ptr<Node> src = CreateObject<Node>();
        Names::Add("SrcNode", src);
        Ptr<Node> dst = CreateObject<Node>();
        Names::Add("DstNode", dst);
        Ptr<Node> a = CreateObject<Node>();
        Names::Add("RouterA", a);
        Ptr<Node> b = CreateObject<Node>();
        Names::Add("RouterB", b);
        Ptr<Node> c = CreateObject<Node>();
        Names::Add("RouterC", c);
        Ptr<Node> d = CreateObject<Node>();
        Names::Add("RouterD", d);
        Ptr<Node> e = CreateObject<Node>();
        Names::Add("RouterE", e);
        
        NodeContainer net1(src, a);
        NodeContainer net2(a, b);
        NodeContainer net3(a, c);
        NodeContainer net4(b, c);
        NodeContainer net5(c, d);
        NodeContainer net6(b, d);
        NodeContainer net7(d, dst);
        NodeContainer net8(b, e);
        NodeContainer net9(d, e);
        NodeContainer routers(a, b, c, d, e);
        NodeContainer nodes(src, dst);
        
        CsmaHelper csma;
        csma.SetChannelAttribute ("DataRate", DataRateValue(5000000));
        csma.SetChannelAttribute ("Delay", TimeValue(MilliSeconds (2)));
        
        NetDeviceContainer ndc1 = csma.Install(net1);
        NetDeviceContainer ndc2 = csma.Install(net2);
        NetDeviceContainer ndc3 = csma.Install(net3);
        NetDeviceContainer ndc4 = csma.Install(net4);
        NetDeviceContainer ndc5 = csma.Install(net5);
        NetDeviceContainer ndc6 = csma.Install(net6);
        NetDeviceContainer ndc7 = csma.Install(net7);
        NetDeviceContainer ndc8 = csma.Install(net8);
        NetDeviceContainer ndc9 = csma.Install(net9);
        
        RipHelper ripRouting;
        
        ripRouting.ExcludeInterface(a, 1);
        ripRouting.ExcludeInterface(d, 3);
        
        ripRouting.SetInterfaceMetric(c, 3, 10);
        ripRouting.SetInterfaceMetric(d, 1, 10);
        
        Ipv4ListRoutingHelper listRH;
        listRH.Add(ripRouting, 0);
        
        InternetStackHelper internet;
        internet.SetIpv6StackInstall(false);
        internet.SetRoutingHelper(listRH);
        internet.Install(routers);
        
        InternetStackHelper internetNodes;
        internetNodes.SetIpv6StackInstall(false);
        internetNodes.Install (nodes);
        
        Ipv4AddressHelper ipv4;
        
        ipv4.SetBase(Ipv4Address("10.0.0.0"), Ipv4Mask("255.255.255.0"));
        Ipv4InterfaceContainer iic1 = ipv4.Assign(ndc1);
        ipv4.SetBase(Ipv4Address("10.0.1.0"), Ipv4Mask("255.255.255.0"));
        Ipv4InterfaceContainer iic2 = ipv4.Assign(ndc2);
        ipv4.SetBase(Ipv4Address("10.0.2.0"), Ipv4Mask("255.255.255.0"));
        Ipv4InterfaceContainer iic3 = ipv4.Assign(ndc3);
        ipv4.SetBase(Ipv4Address("10.0.3.0"), Ipv4Mask("255.255.255.0"));
        Ipv4InterfaceContainer iic4 = ipv4.Assign(ndc4);
        ipv4.SetBase(Ipv4Address("10.0.4.0"), Ipv4Mask("255.255.255.0"));
        Ipv4InterfaceContainer iic5 = ipv4.Assign(ndc5);
        ipv4.SetBase(Ipv4Address("10.0.5.0"), Ipv4Mask("255.255.255.0"));
        Ipv4InterfaceContainer iic6 = ipv4.Assign(ndc6);
        ipv4.SetBase(Ipv4Address("10.0.6.0"), Ipv4Mask("255.255.255.0"));
        Ipv4InterfaceContainer iic7 = ipv4.Assign(ndc7);
        ipv4.SetBase(Ipv4Address("10.0.7.0"), Ipv4Mask("255.255.255.0"));
        Ipv4InterfaceContainer iic8 = ipv4.Assign(ndc8);
        ipv4.SetBase(Ipv4Address("10.0.8.0"), Ipv4Mask("255.255.255.0"));
        Ipv4InterfaceContainer iic9 = ipv4.Assign(ndc9);
        
        Ptr<Ipv4StaticRouting> staticRouting;
        staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting>(src->GetObject<Ipv4>()->GetRoutingProtocol());
        staticRouting->SetDefaultRoute("10.0.0.2", 1 );
        staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting>(dst->GetObject<Ipv4>()->GetRoutingProtocol ());
        staticRouting->SetDefaultRoute("10.0.6.1", 1 );
        
        if (printRoutingTables){
                RipHelper routingHelper;
                Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>(&std::cout);
                
                routingHelper.PrintRoutingTableAt(Seconds(30.0), a, routingStream);
                routingHelper.PrintRoutingTableAt(Seconds(30.0), b, routingStream);
                routingHelper.PrintRoutingTableAt(Seconds(30.0), c, routingStream);
                routingHelper.PrintRoutingTableAt(Seconds(30.0), d, routingStream);
                routingHelper.PrintRoutingTableAt(Seconds(60.0), a, routingStream);
                routingHelper.PrintRoutingTableAt(Seconds(60.0), b, routingStream);
                routingHelper.PrintRoutingTableAt(Seconds(60.0), c, routingStream);
                routingHelper.PrintRoutingTableAt(Seconds(60.0), d, routingStream);
                routingHelper.PrintRoutingTableAt(Seconds(90.0), a, routingStream);
                routingHelper.PrintRoutingTableAt(Seconds(90.0), b, routingStream);
                routingHelper.PrintRoutingTableAt(Seconds(90.0), c, routingStream);
                routingHelper.PrintRoutingTableAt(Seconds(90.0), d, routingStream);
        }
        
        uint32_t packetSize = 1024;
        Time interPacketInterval = Seconds(1.0);
        V4PingHelper ping("10.0.6.2");
        
        ping.SetAttribute ("Interval", TimeValue(interPacketInterval));
        ping.SetAttribute ("Size", UintegerValue(packetSize));
        if(showPings) ping.SetAttribute("Verbose", BooleanValue(true));
        
        ApplicationContainer apps = ping.Install(src);
        apps.Start(Seconds(1.0));
        apps.Stop(Seconds(110.0));
        
        AsciiTraceHelper ascii;
        csma.EnableAsciiAll(ascii.CreateFileStream ("1.tr"));
        
        std::string animFile = "1.xml";
        AnimationInterface anim(animFile);
        
        Simulator::Schedule(Seconds (40), &TearDownLink, b, d, 3, 2);
        
        Simulator::Stop(Seconds(131.0));
        Simulator::Run ();
        Simulator::Destroy ();
        
        return 1;
}


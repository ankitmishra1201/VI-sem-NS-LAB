#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"

using namespace ns3;

int main(int argc, char *argv[]){
	uint32_t nCsma = 5;
	uint32_t maxBytes = 1000;
        uint16_t port = 9;
        bool tracing = true;
        int s = 0;

        NodeContainer nodes;
        nodes.Create(10);
        
        PointToPointHelper pointToPoint;
        pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
        pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
        
        NetDeviceContainer devices[45];
        for(int i = 0; i<10; i++){
        	for(int j = i+1; j<10; j++){
        		devices[s] = pointToPoint.Install(nodes.Get(i), nodes.Get(j));
        		printf("i = %d j = %d s = %d\n", i, j, s);
        		s++;
        	}
        }
        
        InternetStackHelper stack;
        stack.Install(nodes);
        
        Ipv4AddressHelper address[45];
        address[0].SetBase("10.1.1.0", "255.255.255.0");
        address[1].SetBase("10.1.2.0", "255.255.255.0");
        address[2].SetBase("10.1.3.0", "255.255.255.0");
        address[3].SetBase("10.1.4.0", "255.255.255.0");
        address[4].SetBase("10.1.5.0", "255.255.255.0");
        address[5].SetBase("10.1.6.0", "255.255.255.0");
        address[6].SetBase("10.1.7.0", "255.255.255.0");
        address[7].SetBase("10.1.8.0", "255.255.255.0");
        address[8].SetBase("10.1.9.0", "255.255.255.0");
        address[9].SetBase("10.1.10.0", "255.255.255.0");
        address[10].SetBase("10.1.11.0", "255.255.255.0");
        address[11].SetBase("10.1.12.0", "255.255.255.0");
        address[12].SetBase("10.1.13.0", "255.255.255.0");
        address[13].SetBase("10.1.14.0", "255.255.255.0");
        address[14].SetBase("10.1.15.0", "255.255.255.0");
        address[15].SetBase("10.1.16.0", "255.255.255.0");
        address[16].SetBase("10.1.17.0", "255.255.255.0");
        address[17].SetBase("10.1.18.0", "255.255.255.0");
        address[18].SetBase("10.1.19.0", "255.255.255.0");
        address[19].SetBase("10.1.20.0", "255.255.255.0");
        address[20].SetBase("10.1.21.0", "255.255.255.0");
        address[21].SetBase("10.1.22.0", "255.255.255.0");
        address[22].SetBase("10.1.23.0", "255.255.255.0");
        address[23].SetBase("10.1.24.0", "255.255.255.0");
        address[24].SetBase("10.1.25.0", "255.255.255.0");
        address[25].SetBase("10.1.26.0", "255.255.255.0");
        address[26].SetBase("10.1.27.0", "255.255.255.0");
        address[27].SetBase("10.1.28.0", "255.255.255.0");
        address[28].SetBase("10.1.29.0", "255.255.255.0");
        address[29].SetBase("10.1.30.0", "255.255.255.0");
        address[30].SetBase("10.1.31.0", "255.255.255.0");
        address[31].SetBase("10.1.32.0", "255.255.255.0");
        address[32].SetBase("10.1.34.0", "255.255.255.0");
        address[33].SetBase("10.1.35.0", "255.255.255.0");
        address[34].SetBase("10.1.36.0", "255.255.255.0");
        address[35].SetBase("10.1.37.0", "255.255.255.0");
        address[36].SetBase("10.1.38.0", "255.255.255.0");
        address[37].SetBase("10.1.39.0", "255.255.255.0");
        address[38].SetBase("10.1.40.0", "255.255.255.0");
        address[39].SetBase("10.1.41.0", "255.255.255.0");
        address[40].SetBase("10.1.42.0", "255.255.255.0");
        address[41].SetBase("10.1.43.0", "255.255.255.0");
        address[42].SetBase("10.1.44.0", "255.255.255.0");
        address[43].SetBase("10.1.45.0", "255.255.255.0");
        address[44].SetBase("10.1.46.0", "255.255.255.0");
         
        Ipv4InterfaceContainer interfaces[45];
        for(int i = 0; i<45; i++){
        	interfaces[i] = address[i].Assign(devices[i]);
        }
        
        Ipv4GlobalRoutingHelper::PopulateRoutingTables();
        
        UdpEchoServerHelper echoServer(9);
        
        ApplicationContainer serverApps = echoServer.Install(nodes.Get(5));
        serverApps.Start(Seconds(1.0));
        serverApps.Stop(Seconds(10.0));
        
        UdpEchoClientHelper echoClient(interfaces[5].GetAddress(1), 9);
        echoClient.SetAttribute("MaxPackets", UintegerValue(10));
        echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
        echoClient.SetAttribute("PacketSize", UintegerValue(1024));
        
        ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
        clientApps.Start(Seconds(1.0));
        clientApps.Stop(Seconds(10.0));
        
        if (tracing){
        	AsciiTraceHelper ascii;
        	pointToPoint.EnableAsciiAll(ascii.CreateFileStream("1.tr"));
        }
        
        std::string animFile = "1.xml";
        AnimationInterface anim(animFile);
        
        Simulator::Run();
        Simulator::Destroy();
        
        return 0;
}

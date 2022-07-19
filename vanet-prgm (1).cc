// ****************************************************************************************
//Self Organizing Networking Group (SONG) @ Kongu Engineering College (KEC) 
//kecsong2013@gmail.com		 kecsong@gmail.com
//*****************************************************************************************
// Program V : Creating TCP ON-OFF Application for VANET scenario (Mobility Model Included)

#include "ns3/ocb-wifi-mac.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/aodv-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ns2-mobility-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h" 
#include "ns3/yans-wifi-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"
#include "ns3/internet-module.h"
#include "ns3/tcp-header.h"
#include "ns3/flow-monitor-helper.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("8Westwood-mobility");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  bool tracing = true;
  bool enableFlowMonitor = true;
  std::string phyMode ("OfdmRate6MbpsBW10MHz");
  std::string traceFile = "scratch/test.txt";
  double interval = 0.5; // seconds
 // uint32_t packetSize = 1000; // bytes

  CommandLine cmd;

  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("EnableMonitor", "Enable Flow Monitor", enableFlowMonitor);
  cmd.AddValue ("interval", "interval (seconds) between packets", interval);
  cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
  cmd.Parse (argc,argv);
  //Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpVegas"));
  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpWestwood"));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1000));
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold",
                      StringValue ("2200"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold",
                      StringValue ("2200"));
 Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", 
                      StringValue (phyMode));


  if (verbose)
    {
      LogComponentEnable ("TcpWestwood", LOG_LEVEL_INFO);
      LogComponentEnable ("TcpWestwood", LOG_LEVEL_INFO);
    }
  

  Ns2MobilityHelper ns2mobility = Ns2MobilityHelper (traceFile);
  NodeContainer vanetnodes;
  vanetnodes.Create (100);
  ns2mobility.Install ();


  // The below set of helpers will help us to put together the wifi NICs we want
  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  Ptr<YansWifiChannel> channel = wifiChannel.Create ();
  wifiPhy.SetChannel (channel);
  // ns-3 supports generate a pcap trace
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);
  NqosWaveMacHelper wifi80211pMac = NqosWaveMacHelper::Default ();
  Wifi80211pHelper wifi80211p = Wifi80211pHelper::Default ();
   wifi80211p.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                      "DataMode",StringValue (phyMode),
                                      "ControlMode",StringValue (phyMode));
  NetDeviceContainer vanetdevices = wifi80211p.Install (wifiPhy, wifi80211pMac, vanetnodes);


  if (verbose)
    {
     wifi80211p.EnableLogComponents ();      // Turn on all Wifi 802.11p logging
    }

  // Enable AODV
  AodvHelper aodv;
  InternetStackHelper stack;
  stack.SetRoutingHelper (aodv);
  stack.Install (vanetnodes);
     

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (vanetdevices);

 // Choosing Source and Destination Node for tcp traffic Flow 

  NS_LOG_INFO ("Create Applications.");
  uint16_t port = 9;   
 
// 8 traffic

  Ptr<Node> appSource1 = NodeList::GetNode (50);
  Ptr<Node> appSink1 = NodeList::GetNode (75);
  Ipv4Address remoteAddr1 = appSink1->GetObject<Ipv4> ()->GetAddress (1,0).GetLocal ();



  OnOffHelper onoff1 ("ns3::TcpSocketFactory", Address (InetSocketAddress (remoteAddr1, port)));
  onoff1.SetAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  onoff1.SetAttribute ("PacketSize", UintegerValue (1000));
  ApplicationContainer apps1 = onoff1.Install (vanetnodes.Get (50));
  apps1.Start (Seconds (1));
  apps1.Stop (Seconds (100));
  PacketSinkHelper sink1 ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
  apps1 = sink1.Install (vanetnodes.Get (75));
  apps1.Start (Seconds (1));
//2

  Ptr<Node> appSource2 = NodeList::GetNode (90);
  Ptr<Node> appSink2 = NodeList::GetNode (76);
  Ipv4Address remoteAddr2 = appSink2->GetObject<Ipv4> ()->GetAddress (1,0).GetLocal ();



  OnOffHelper onoff2 ("ns3::TcpSocketFactory", Address (InetSocketAddress (remoteAddr2, port)));
  onoff2.SetAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  onoff2.SetAttribute ("PacketSize", UintegerValue (1000));
  ApplicationContainer apps2 = onoff2.Install (vanetnodes.Get (90));
  apps2.Start (Seconds (1));
  apps2.Stop (Seconds (100));
  PacketSinkHelper sink2 ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
  apps2 = sink2.Install (vanetnodes.Get (76));
  apps2.Start (Seconds (1));

//3

Ptr<Node> appSource3 = NodeList::GetNode (91);
  Ptr<Node> appSink3 = NodeList::GetNode (79);
  Ipv4Address remoteAddr3 = appSink3->GetObject<Ipv4> ()->GetAddress (1,0).GetLocal ();



  OnOffHelper onoff3 ("ns3::TcpSocketFactory", Address (InetSocketAddress (remoteAddr3, port)));
  onoff3.SetAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  onoff3.SetAttribute ("PacketSize", UintegerValue (1000));
  ApplicationContainer apps3 = onoff3.Install (vanetnodes.Get (91));
  apps3.Start (Seconds (1));
  apps3.Stop (Seconds (100));
  PacketSinkHelper sink3 ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
  apps3 = sink3.Install (vanetnodes.Get (79));
  apps3.Start (Seconds (1));

//4

Ptr<Node> appSource4 = NodeList::GetNode (95);
  Ptr<Node> appSink4 = NodeList::GetNode (52);
  Ipv4Address remoteAddr4 = appSink4->GetObject<Ipv4> ()->GetAddress (1,0).GetLocal ();



  OnOffHelper onoff4 ("ns3::TcpSocketFactory", Address (InetSocketAddress (remoteAddr4, port)));
  onoff4.SetAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  onoff4.SetAttribute ("PacketSize", UintegerValue (1000));
  ApplicationContainer apps4 = onoff4.Install (vanetnodes.Get (95));
  apps4.Start (Seconds (1));
  apps4.Stop (Seconds (100));
  PacketSinkHelper sink4 ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
  apps4= sink4.Install (vanetnodes.Get (52));
  apps4.Start (Seconds (1));

//5

Ptr<Node> appSource5 = NodeList::GetNode (99);
  Ptr<Node> appSink5 = NodeList::GetNode (98);
  Ipv4Address remoteAddr5 = appSink5->GetObject<Ipv4> ()->GetAddress (1,0).GetLocal ();



  OnOffHelper onoff5 ("ns3::TcpSocketFactory", Address (InetSocketAddress (remoteAddr5, port)));
  onoff5.SetAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  onoff5.SetAttribute ("PacketSize", UintegerValue (1000));
  ApplicationContainer apps5 = onoff5.Install (vanetnodes.Get (99));
  apps5.Start (Seconds (1));
  apps5.Stop (Seconds (100));
  PacketSinkHelper sink5 ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
  apps5= sink5.Install (vanetnodes.Get (98));
  apps5.Start (Seconds (1));

//6
Ptr<Node> appSource6 = NodeList::GetNode (87);
  Ptr<Node> appSink6 = NodeList::GetNode (86);
  Ipv4Address remoteAddr6 = appSink6->GetObject<Ipv4> ()->GetAddress (1,0).GetLocal ();



  OnOffHelper onoff6 ("ns3::TcpSocketFactory", Address (InetSocketAddress (remoteAddr6, port)));
  onoff6.SetAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  onoff6.SetAttribute ("PacketSize", UintegerValue (1000));
  ApplicationContainer apps6= onoff6.Install (vanetnodes.Get (87));
  apps6.Start (Seconds (1));
  apps6.Stop (Seconds (100));
  PacketSinkHelper sink6 ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
  apps6= sink6.Install (vanetnodes.Get (86));
  apps6.Start (Seconds (1));

//7
Ptr<Node> appSource7 = NodeList::GetNode (97);
  Ptr<Node> appSink7 = NodeList::GetNode (67);
  Ipv4Address remoteAddr7 = appSink7->GetObject<Ipv4> ()->GetAddress (1,0).GetLocal ();



  OnOffHelper onoff7 ("ns3::TcpSocketFactory", Address (InetSocketAddress (remoteAddr7, port)));
  onoff7.SetAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  onoff7.SetAttribute ("PacketSize", UintegerValue (1000));
  ApplicationContainer apps7= onoff7.Install (vanetnodes.Get (97));
  apps7.Start (Seconds (1));
  apps7.Stop (Seconds (100));
  PacketSinkHelper sink7 ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
  apps7= sink7.Install (vanetnodes.Get (67));
  apps7.Start (Seconds (1));

//8
Ptr<Node> appSource8 = NodeList::GetNode (55);
  Ptr<Node> appSink8 = NodeList::GetNode (89);
  Ipv4Address remoteAddr8 = appSink8->GetObject<Ipv4> ()->GetAddress (1,0).GetLocal ();



  OnOffHelper onoff8 ("ns3::TcpSocketFactory", Address (InetSocketAddress (remoteAddr8, port)));
  onoff8.SetAttribute ("DataRate", DataRateValue (DataRate ("100Mb/s")));
  onoff8.SetAttribute ("PacketSize", UintegerValue (1000));
  ApplicationContainer apps8= onoff8.Install (vanetnodes.Get (55));
  apps8.Start (Seconds (1));
  apps8.Stop (Seconds (100));
  PacketSinkHelper sink8 ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
  apps8= sink8.Install (vanetnodes.Get (89));
  apps8.Start (Seconds (1));



// Tracing
  //wifiPhy.EnablePcap ("vanet-aodv-mobility", vanetdevices);

// Installing Flow Monitor
  FlowMonitorHelper flowmonHelper;
  if (enableFlowMonitor)
    {
      flowmonHelper.InstallAll ();
    }
  Simulator::Stop (Seconds (10));
  Simulator::Run ();

// Enabling the XML Flow Monitor
 if (enableFlowMonitor)
    {
      flowmonHelper.SerializeToXmlFile ("8Westwood-vanet-mobility.flowmon", false, false);
    }
  Simulator::Destroy ();
  return 0;
}

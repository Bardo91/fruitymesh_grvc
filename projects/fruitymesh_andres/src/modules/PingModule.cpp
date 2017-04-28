/**

Copyright (c) 2014-2015 "M-Way Solutions GmbH"
FruityMesh - Bluetooth Low Energy mesh protocol [http://mwaysolutions.com/]

This file is part of FruityMesh

FruityMesh is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <PingModule.h>
#include <Logger.h>
#include <Utility.h>
#include <Storage.h>
#include <Node.h>
#include <stdlib.h>
#include <LedWrapper.h>

#include <math.h>
//#include <thread>         // std::this_thread::sleep_for
//#include <chrono>         // std::chrono::seconds

//#include <iostream>       // std::cout, std::endl

#define kTimerInterval 1*1000

extern "C"{

}

PingModule::PingModule(u8 moduleId, Node* node, ConnectionManager* cm, const char* name, u16 storageSlot)
	: Module(moduleId, node, cm, name, storageSlot)
{
	//Register callbacks n' stuff
	Logger::getInstance().enableTag("PINGMOD");

	//Save configuration to base class variables
	//sizeof configuration must be a multiple of 4 bytes
	configurationPointer = &configuration;
	configurationLength = sizeof(PingModuleConfiguration);

	//Start module configuration loading
	LoadModuleConfiguration();
}

void PingModule::ConfigurationLoadedHandler()
{
	//Does basic testing on the loaded configuration
	Module::ConfigurationLoadedHandler();

	//Version migration can be added here
	if(configuration.moduleVersion == 1){/* ... */};

	//Do additional initialization upon loading the config
	configuration.beta = 0;
	configuration.dk = 0;
	configuration.dk_1 = 0;

	configuration.pingInterval = kTimerInterval;
	configuration.lastPingTimer = 0;


	//Start the Module...
	logt("PINGMOD", "ConfigLoaded");

}

void PingModule::TimerEventHandler(u16 passedTime, u32 appTimer)
{
	//Do stuff on timer...
	/*if(configuration.pingInterval != 0 && node->appTimerMs - configuration.lastPingTimer > configuration.pingInterval)
		{
			configuration.lastPingTimer = node->appTimerMs;
			SendPing(11392);
		}*/
}

void PingModule::ResetToDefaultConfiguration()
{
	//Set default configuration values
	configuration.moduleId = moduleId;
	configuration.moduleActive = false;
	configuration.moduleVersion = 1;

	//Set additional config values...
	configuration.beta = 0;
	configuration.dk = 0;
	configuration.dk_1 = 0;

	configuration.pingInterval = kTimerInterval;
	configuration.lastPingTimer = 0;

	//Set additional config values...
	logt("PINGMOD", "Reset");
}

bool PingModule::SendPing(nodeID targetNodeId)
{
	// logt("PINGMOD", "Trying to ping node %u from %u", targetNodeId, node->persistentConfig.nodeId);

	logt("PINGMOD", "Trying to ping node %u", targetNodeId);
    //Send ping packet to that node
    connPacketModule packet;
    packet.header.messageType = MESSAGE_TYPE_MODULE_TRIGGER_ACTION;
    packet.header.sender = node->persistentConfig.nodeId;
    packet.header.receiver = targetNodeId;

    packet.moduleId = moduleId;
    packet.actionType = PingModuleTriggerActionMessages::TRIGGER_PING;
    //Get Average RSSI of all connections
    			int a = cm->connections[0]->GetAverageRSSI();
    			int b = cm->connections[1]->GetAverageRSSI();
    			int c = cm->connections[2]->GetAverageRSSI();
    			int d = cm->connections[3]->GetAverageRSSI();
    			//algorithm_local algorithm;
    			// type u8 = packt.data
    			//algorithm.sum = -(a+b+c+d);
    			// sum positive
    			int sum = -(a+b+c+d);
    			// Convert RSSI to Meters

    			//MODELO 1
    			float all1 = 1.86 * exp(0.068*(sum));

    			//MODELO 2
    			float all2 = 1.1 * exp(0.07*(sum));

    			long cmtrs1 = all1;
    			long cmtrs2 = all2;
    			int d_cmtrs1 = cmtrs1;
    			int d_cmtrs2 = cmtrs2;

				logt("PINGMOD", "RSSI: [%d][%d][%d][%d]", a,b,c,d);
				logt("PINGMOD", "Distancia: [%d][%d]", d_cmtrs1,d_cmtrs2);

    packet.data[0] = sum;

    cm->SendMessageToReceiver(NULL, (u8*)&packet, SIZEOF_CONN_PACKET_MODULE + 1, true);
	return(true);
}

bool PingModule::TerminalCommandHandler(string commandName, vector<string> commandArgs)
{
	//React on commands, return true if handled, false otherwise
	//using namespace std::this_thread;

	//using namespace std::this_thread;

	if(commandName == "pingmod")
	{
		nodeID targetNodeId = atoi(commandArgs[0].c_str());

		for(int j=1; j<5; j++)
		{
			//if(configuration.pingInterval != 0 && (node->appTimerMs - configuration.lastPingTimer) > configuration.pingInterval)
			//		{
				//		configuration.lastPingTimer = node->appTimerMs;
						SendPing(targetNodeId);
						//std::this_thread::sleep_for (std::chrono::seconds(1));
				//	}
		}


			//Get the id of the target node
			//nodeID targetNodeId = atoi(commandArgs[0].c_str());
			logt("PINGMOD", "Trying to ping node %u", targetNodeId);

			//TODO: Send ping packet to that node
			connPacketModule packet;
			packet.header.messageType = MESSAGE_TYPE_MODULE_TRIGGER_ACTION;
			packet.header.sender = node->persistentConfig.nodeId;
			packet.header.receiver = targetNodeId;

			packet.moduleId = moduleId;
			packet.actionType = PingModuleTriggerActionMessages::TRIGGER_PING;

			//Get Average RSSI of all connections
			int a = cm->connections[0]->GetAverageRSSI();
			int b = cm->connections[1]->GetAverageRSSI();
			int c = cm->connections[2]->GetAverageRSSI();
			int d = cm->connections[3]->GetAverageRSSI();
			//algorithm_local algorithm;
			// type u8 = packt.data
			//algorithm.sum = -(a+b+c+d);
			// sum positive
			int sum = -(a+b+c+d);
			// Convert RSSI to Meters

			//MODELO 1
			//float all = 1.86 * exp(0.068*(sum));

			//MODELO 2
			float all = 1.1 * exp(0.07*(sum));

			long cmtrs = all;

			packet.data[0] = sum;

			cm->SendMessageToReceiver(NULL, (u8*)&packet, SIZEOF_CONN_PACKET_MODULE + 1, true);

			// Distance measurements k & k-1
			int dk_1;
			int dk;
			float beta_g;
			int beta_d;
			int bet;


			configuration.luk = 50;

			// Fixed distance traveled
			/*if(commandArgs.size() >= 4)
			{
				configuration.luk = atoi(commandArgs[2].c_str());
			}
			else
			{
				configuration.luk = 50;
			}*/



			//if(commandArgs[1] != moduleName) return false;
			if(commandArgs.size() >= 2 && commandArgs[1] == "a")
			{
				// First Estimation
				// Store 2 Results
				configuration.dk_1 = cmtrs;
				logt("PINGMOD", "RSSI K-1: %d", sum);
				logt("PINGMOD", "Distancia K-1: [%d]", configuration.dk_1);

				return true;
			}
			else if(commandArgs.size() >= 2 && commandArgs[1] == "b")
			{
				// Second Estimation
				// Store 2 Results
				configuration.dk = cmtrs;
				dk = configuration.dk;
				dk_1 = configuration.dk_1;
				int luk = configuration.luk;
				//int term1 = pow (luk, 2) + pow (dk, 2) - pow (dk_1, 2);
				int term1 = (luk*luk) + (dk*dk) - (dk_1*dk_1);
				int term2 = 2* luk * dk;
				float arg_cos = (float)term1/(float)term2;
				int a_cos = arg_cos;

				logt("PINGMOD", "Distancia K-1: [%d]", configuration.dk_1);
				logt("PINGMOD", "Distancia K: [%d]", configuration.dk);
				logt("PINGMOD", "RSSI K: %d", sum);
				//logt("PINGMOD", "Term1: [%d]", term1);
				//logt("PINGMOD", "Term2: [%d]", term2);
				//logt("PINGMOD", "Arg Coseno: [%d] [%f]", a_cos, arg_cos);

				// Result for cosine law boundaries
				if (arg_cos < -1)
				{
					configuration.beta = 3;
					bet = configuration.beta;
					beta_g = configuration.beta * 180 / 3.1416;
					beta_d = beta_g;
					//logt("PINGMOD", "Beta-1: [%d] [%d]", bet ,beta_d);
				}
				else if (arg_cos > 1)
				{
					configuration.beta = 0;
					bet = configuration.beta;
					beta_g = configuration.beta * 180 / 3.1416;
					beta_d = beta_g;
					//logt("PINGMOD", "Beta+1: [%d] [%d]", bet ,beta_d);
				}
				else
				{
					configuration.beta = acos(arg_cos);
					bet = configuration.beta;
					beta_g = configuration.beta * 180 / 3.1416;
					beta_d = beta_g;
					//logt("PINGMOD", "Beta: [%d] [%d]", bet ,beta_d);
				}

				float theta_wk_uk_1 = 3.1416 + configuration.beta;
				float theta_wk_uk_2 = 3.1416 - configuration.beta;

				// movimiento en Y positivo, + 90 grados orientación ajustada (1.5708).
				float x_estimado = (float)dk * cos(theta_wk_uk_1+1.5708);
				float y_estimado = (float)dk * sin(theta_wk_uk_1+1.5708);

				float x_estimado_2 = (float)dk * cos(theta_wk_uk_2+1.5708);
				float y_estimado_2 = (float)dk * sin(theta_wk_uk_2+1.5708);

				//------------------------------------------------------
				int th_1 = theta_wk_uk_1 * 180 / 3.1416;
				int th_2 = theta_wk_uk_2 * 180 / 3.1416;

				int x_1 = x_estimado;
				int y_1 = y_estimado;

				int x_2 = x_estimado_2;
				int y_2 = y_estimado_2;

				logt("PINGMOD", "Theta: [%d] [%d]", th_1 ,th_2);
				logt("PINGMOD", "Pos1: [%d] [%d]", x_1 ,y_1);
				logt("PINGMOD", "Pos2: [%d] [%d]", x_2 ,y_2);
				//logt("PINGMOD", "Beta: [%d]", ang_beta);

				return true;
			}
			else if(commandArgs.size() >= 2 && commandArgs[1] == "c")
			{
				logt("PINGMOD", "Argumento F");
				return true;
			}
			else if(commandArgs.size() == 1)
			{
				logt("PINGMOD", "RSSI: [%d] [%d] [%d] [%d] Sum: %d" , a, b, c, d, sum);
				logt("PINGMOD", "Cmts: %d" , cmtrs);
				return true;
			}
	}

	//Must be called to allow the module to get and set the config
	return Module::TerminalCommandHandler(commandName, commandArgs);
}


void PingModule::ConnectionPacketReceivedEventHandler(connectionPacket* inPacket, Connection* connection, connPacketHeader* packetHeader, u16 dataLength)
{
	//Must call superclass for handling
	Module::ConnectionPacketReceivedEventHandler(inPacket, connection, packetHeader, dataLength);

	if(packetHeader->messageType == MESSAGE_TYPE_MODULE_TRIGGER_ACTION){
		connPacketModule* packet = (connPacketModule*)packetHeader;

		//Check if our module is meant and we should trigger an action
		if(packet->moduleId == moduleId){
			if(packet->actionType == PingModuleTriggerActionMessages::TRIGGER_PING){
                //Inform the user
               logt("PINGMOD", "Ping request received with data: %d", packet->data[0]);

                //TODO: Send PING_RESPONSE
                //Send PING_RESPONSE
               connPacketModule outPacket;
               outPacket.header.messageType = MESSAGE_TYPE_MODULE_ACTION_RESPONSE;
               outPacket.header.sender = node->persistentConfig.nodeId;
               outPacket.header.receiver = packetHeader->sender;

               outPacket.moduleId = moduleId;
               outPacket.actionType = PingModuleActionResponseMessages::PING_RESPONSE;
               outPacket.data[0] = packet->data[0];
               outPacket.data[1] = 111;

               cm->SendMessageToReceiver(NULL, (u8*)&outPacket, SIZEOF_CONN_PACKET_MODULE + 2, true);

			}
		}
	}


	//Parse Module responses
	if(packetHeader->messageType == MESSAGE_TYPE_MODULE_ACTION_RESPONSE){
		connPacketModule* packet = (connPacketModule*)packetHeader;

		//Check if our module is meant and we should trigger an action
		if(packet->moduleId == moduleId)
		{
			if(packet->actionType == PingModuleActionResponseMessages::PING_RESPONSE)
			{
				logt("PINGMOD", "Ping came back from %u with data %d, %d", packet->header.sender, packet->data[0], packet->data[1]);
			}
		}
	}
}



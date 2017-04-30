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

#include <CustomModule.h>
#include <Logger.h>
#include <Utility.h>
#include <Storage.h>
#include <Node.h>
#include <stdlib.h>
#include <LedWrapper.h>

#include <math.h>

//#include <iostream>       // std::cout, std::endl

#define kTimerInterval 1*1000

extern "C"{
	#include <nrf_delay.h>
}

//---------------------------------------------------------------------------------------------------------------------
CustomModule::CustomModule(u8 moduleId, Node* node, ConnectionManager* cm, const char* name, u16 storageSlot)
	: Module(moduleId, node, cm, name, storageSlot) {
	//Register callbacks n' stuff
	//Logger::getInstance().enableTag("CUSTOMMOD");

	//Save configuration to base class variables
	//sizeof configuration must be a multiple of 4 bytes
	configurationPointer = &configuration;
	configurationLength = sizeof(CustomModuleConfiguration);

	//Start module configuration loading
	LoadModuleConfiguration();
}

//---------------------------------------------------------------------------------------------------------------------
void CustomModule::ConfigurationLoadedHandler() {
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

//---------------------------------------------------------------------------------------------------------------------
void CustomModule::TimerEventHandler(u16 passedTime, u32 appTimer){
	//Do stuff on timer...
	/*if(//---------------------------------------------------------------------------------------------------------------------configuration.pingInterval != 0 && node->appTimerMs - configuration.lastPingTimer > configuration.pingInterval)
		{
			configuration.lastPingTimer = node->appTimerMs;
			SendPing(11392);
		}*/
}

//---------------------------------------------------------------------------------------------------------------------
void CustomModule::ResetToDefaultConfiguration(){
	//Set default configuration values
	configuration.moduleId = moduleId;
	configuration.moduleActive = false;	// 999 SET TO ACTIVE TO HANDLE BLE EVENTS!
	configuration.moduleVersion = 1;

	//Set additional config values...
	configuration.beta = 0;
	configuration.dk = 0;
	configuration.dk_1 = 0;

	configuration.pingInterval = kTimerInterval;
	configuration.lastPingTimer = 0;
	//Set additional config values...
	logt("CUSTOMMOD", "Reset");
}

//---------------------------------------------------------------------------------------------------------------------
bool CustomModule::SendPing(nodeID targetNodeId) {
	// logt("PINGMOD", "Trying to ping node %u from %u", targetNodeId, node->persistentConfig.nodeId);

	logt("CUSTOMMOD", "Trying to ping node %u", targetNodeId);
    //Send ping packet to that node
    connPacketModule packet;
    packet.header.messageType = MESSAGE_TYPE_MODULE_TRIGGER_ACTION;
    packet.header.sender = node->persistentConfig.nodeId;
    packet.header.receiver = targetNodeId;

    packet.moduleId = moduleId;
    packet.actionType = CustomModuleTriggerActionMessages::TRIGGER_PING;
    //Get Average RSSI of all connections
	int a = cm->connections[0]->GetAverageRSSI();
	int b = cm->connections[1]->GetAverageRSSI();
	int c = cm->connections[2]->GetAverageRSSI();
	int d = cm->connections[3]->GetAverageRSSI();

	logt("CUSTOMMOD", "RSSI: [%d][%d][%d][%d]", a,b,c,d);
    cm->SendMessageToReceiver(NULL, (u8*)&packet, SIZEOF_CONN_PACKET_MODULE + 1, true);
	return(true);
}

//---------------------------------------------------------------------------------------------------------------------
bool CustomModule::TerminalCommandHandler(string commandName, vector<string> commandArgs) {
	if(commandName == "custommod") {
		if(commandArgs[0] == "ping"){
			nodeID targetNodeId = atoi(commandArgs[1].c_str());
			//Get the id of the target node
			logt("CUSTOMMOD", "Trying to ping node %u", targetNodeId);
			for(int j=1; j<5; j++){
				SendPing(targetNodeId);	
			}
			//TODO: Send ping packet to that node
			connPacketModule packet;
			packet.header.messageType = MESSAGE_TYPE_MODULE_TRIGGER_ACTION;
			packet.header.sender = node->persistentConfig.nodeId;
			packet.header.receiver = targetNodeId;

			packet.moduleId = moduleId;
			packet.actionType = CustomModuleTriggerActionMessages::TRIGGER_PING;
			return true;
		}else if(commandArgs[0] == "ble_record"){
			if(commandArgs.size() == 2){
				mDistanceTag = atof(commandArgs[1].c_str());
			}else{
				mDistanceTag = 0.0;
			}
			mRecording = true;
			configuration.moduleActive = true;
			return true;
		}else if(commandArgs[0] == "stop"){
			configuration.moduleActive = false;
			mRecording = false;
			return true;
		}
	}
	

	//Must be called to allow the module to get and set the config
	return Module::TerminalCommandHandler(commandName, commandArgs);
}

//---------------------------------------------------------------------------------------------------------------------
void CustomModule::BleEventHandler(ble_evt_t* bleEvent){
	if(mRecording){
		if(bleEvent->header.evt_id == BLE_GAP_EVT_RSSI_CHANGED) {
			i8 rssi = bleEvent->evt.gap_evt.params.rssi_changed.rssi;
			logt("CUSTOMMOD", "BLE event handled with (distance,rssi): %f, %d", mDistanceTag, rssi);
		}
	}
};

//---------------------------------------------------------------------------------------------------------------------
void CustomModule::ConnectionPacketReceivedEventHandler(connectionPacket* inPacket, Connection* connection, connPacketHeader* packetHeader, u16 dataLength) {
	//Must call superclass for handling
	Module::ConnectionPacketReceivedEventHandler(inPacket, connection, packetHeader, dataLength);

	if(packetHeader->messageType == MESSAGE_TYPE_MODULE_TRIGGER_ACTION){
		connPacketModule* packet = (connPacketModule*)packetHeader;

		//Check if our module is meant and we should trigger an action
		if(packet->moduleId == moduleId){
			if(packet->actionType == CustomModuleTriggerActionMessages::TRIGGER_PING){
                //Inform the user
               logt("CUSTOMMOD", "Ping request received with data: %d", packet->data[0]);

                //TODO: Send PING_RESPONSE
                //Send PING_RESPONSE
               connPacketModule outPacket;
               outPacket.header.messageType = MESSAGE_TYPE_MODULE_ACTION_RESPONSE;
               outPacket.header.sender = node->persistentConfig.nodeId;
               outPacket.header.receiver = packetHeader->sender;

               outPacket.moduleId = moduleId;
               outPacket.actionType = CustomModuleActionResponseMessages::PING_RESPONSE;
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
			if(packet->actionType == CustomModuleActionResponseMessages::PING_RESPONSE)
			{
				logt("CUSTOMMOD", "Ping came back from %u with data %d, %d", packet->header.sender, packet->data[0], packet->data[1]);
			}
		}
	}
}



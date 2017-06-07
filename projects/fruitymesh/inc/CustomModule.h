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

#pragma once

#include <Module.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

typedef struct
{
	u8 sum;
}algorithm_local;


class CustomModule: public Module
{
	private:

		//Module configuration that is saved persistently (size must be multiple of 4)
		struct CustomModuleConfiguration : ModuleConfiguration{
			//Insert more persistent config values here
			int dk;
			int dk_1;
			float beta;
			int luk;
			int pos_x;
			int pos_y;
			int pingInterval;
			int lastPingTimer;

		};

		CustomModuleConfiguration configuration;

		enum CustomModuleTriggerActionMessages{
			TRIGGER_PING = 0,
			TRIGGER_MESSAGE = 1
		};

		enum CustomModuleActionResponseMessages{
			PING_RESPONSE = 0,
			MESSAGE_RESPONSE = 1
		};

		bool mRunning = false;
		bool mRecording = false;
		int mDistanceTag = 0;
	public:
		CustomModule(u8 moduleId, Node* node, ConnectionManager* cm, const char* name, u16 storageSlot);

		void ConfigurationLoadedHandler();

		void ResetToDefaultConfiguration();

		void TimerEventHandler(u16 passedTime, u32 appTimer);

		void BleEventHandler(ble_evt_t* bleEvent);

		void ConnectionPacketReceivedEventHandler(connectionPacket* inPacket, Connection* connection, connPacketHeader* packetHeader, u16 dataLength);

		//void NodeStateChangedHandler(discoveryState newState);

		bool TerminalCommandHandler(string commandName, vector<string> commandArgs);

		bool SendPing(nodeID targetNodeId);
};

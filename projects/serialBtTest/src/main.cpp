//
//
//
//

#include <iostream>
#include <serial/serial.h>
#include <thread>

int main(int _argc, char **_argv){
    serial::Serial serial("/dev/ttyACM0",38400,serial::Timeout::simpleTimeout(1000));
    serial.setFlowcontrol(serial::flowcontrol_hardware);

    //std::string cmd = "custommod ble_record";
    //serial.write((uint8_t*)cmd.c_str(), cmd.size());
    if(serial.isOpen()){
        std::cout << "Connected to serial port" << std::endl;
        bool run = true;
        while(run){
            if(serial.available()) {
                std::string line;
                serial.readline(line);
                int idFirst = line.find_first_of("///") ;
                int idLast = line.find_last_of("///") ;

                if(idFirst == -1 || idLast == -1)
                    continue;

                line = line.substr(idFirst+3, idLast);
                int idType = line.find_first_of('N');
                if(idType == -1){
                    idType = line.find_first_of('L');
                    if(idType == -1)
                        continue;
                }

                if(line[idType] == 'L'){
                    float latitude = atof(line.substr(1, idLast).c_str());
                    std::cout << "Latitude: " << latitude << std::endl;
                }else if(line[idType] == 'N'){
                    float longitude= atof(line.substr(1, idLast).c_str());
                    std::cout << "Longitude: " << longitude << std::endl;
                }else{
                    continue;
                }

            }
        }
    }
    else
        std::cout << "Error connecting to serial port" << std::endl;


    serial.close();
	return 0;
}

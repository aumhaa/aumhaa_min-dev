//amounra 021022

#include <iostream>
#include <queue>
#include "RtMidi.h"
#include "c74_min.h"

using namespace c74::min;

std::queue <int> midi_messages;

class min_dots_dots_test2 : public object<min_dots_dots_test2> {
    
    
    
    
public:
    MIN_DESCRIPTION    {"Midi Input Object."};
    MIN_TAGS        {"utilities"};
    MIN_AUTHOR        {"aumhaa"};
    MIN_RELATED        {"midiin, midiparse, midiinfo"};
    
    inlet<>  input    { this, "(bang) send available input ports to output, (symbol) choose input port" };
    outlet<>  output { this, "output serialized midi data from the selected input port" };
    outlet<>  output2 { this, "output names of available input ports" };
    
    RtMidiIn *midiin = new RtMidiIn(RtMidi::MACOSX_CORE);
    //    RtMidiIn *midiin = 0;
    //    bool initialized = false;
    
    
    ~min_dots_dots_test2 (){
        if(midiin){
            midiin->cancelCallback();
            midiin->closePort();
        }
        delete midiin;
    };
    
    min_dots_dots_test2(const atoms& args = {}) {
        if (args.size() > 0) {
            portname = args[0];
        };
        //        if (!midiin) {
        //            try {
        //                midiin = new RtMidiIn(RtMidi::MACOSX_CORE);
        //            }
        //            catch (RtMidiError &error) {
        //                error.printMessage();
        //            }
        //        };
        //      display_ports(std::to_string(portname));
        defer_port_udpate.delay(100);
    }
    
    
    
    //this is a hack to deal with a "bug", calling display_ports from instantiation gets ignored.
    timer<> defer_port_udpate {this,
        MIN_FUNCTION {
            display_ports(std::to_string(portname));
            return {};
        }
    };
    
    //this is a hack to deal with crashypants when we try to call output.send directly from a static member.
    timer<> defer {this,
        MIN_FUNCTION {
            while(!midi_messages.empty()){
                output.send(midi_messages.front());
                midi_messages.pop();
            }
            return {};
        }
    };
    
    //although we store the port in this attribute, we need to use a non-max value
    //for updating since setter has no option to set the data
    //before it proceeds with its sideeffects.
    attribute<symbol> portname { this, "port", "None",
        description {
            "Port to connect to."
            "The port will be connected on instantiation."
        },
        setter {
            MIN_FUNCTION {
                string stored_portname = args[0];
                assign_port(stored_portname);
                display_ports(stored_portname);
                return args;
            }
        }
    };
    
    //send available ports out left outlet and select chosen port in umenu
    void display_ports (string stored_port_name) {
        int nPorts = midiin->getPortCount();
        string portName;
        bool found = false;
        output2.send("clear");
        output2.send("append", "None");
        if(stored_port_name == "None"){
            found = true;
        }
        for ( int i=0; i<nPorts; i++ ) {
            try {
                portName = midiin->getPortName(i);
                if (portName == stored_port_name){
                    found = true;
                    
                }
                output2.send("append", portName);
            }
            catch ( RtMidiError &error ) {
                error.printMessage();
            }
        }
        if (!found) {
            output2.send("append", stored_port_name);
        }
        output2.send("setsymbol", stored_port_name);
    }
    
    //assign the port internally and set its callback.
    void assign_port (string stored_portname) {
        midiin->closePort();
        if (stored_portname == "None") {
            midiin->cancelCallback();
            midiin->closePort();
        }
        else {
            bool found = false;
            int nPorts = midiin->getPortCount();
            string portName;
            for ( int i=0; i<nPorts; i++ ) {
                try {
                    portName = midiin->getPortName(i);
                    if(stored_portname==portName){
                        found = true;
                        midiin->openPort(i, portName);
                        midiin->setCallback(&midiinCallback, (void*)this);
                        midiin->ignoreTypes(false, false, false);
                        cout << portName + "opened!" << endl;
                    }
                }
                catch ( RtMidiError &error ) {
                    error.printMessage();
                }
            }
            if(!found){
                try {
                    midiin->openVirtualPort(stored_portname);
                    midiin->setCallback(&midiinCallback, (void*)this);
                    midiin->ignoreTypes(false, false, false);
                    cout << stored_portname + "opened!" << endl;
                }
                catch ( RtMidiError &error ) {
                    error.printMessage();
                }
            }
        }
    };
    
    //this static function uses *userData as a pointer to the instance.  It can't call anything that
    //sends an output to max, so we defer it slightly and queue its messages to be handled by another method.
    static void midiinCallback ( double deltatime, std::vector< unsigned char > *message, void *userData ) {
        auto& owner = *(min_dots_dots_test2*)userData;
        owner.defer.delay(0);
        unsigned int nBytes = message->size();
        for ( unsigned int i=0; i<nBytes; i++ )
            midi_messages.push(message->at(i));
    };
    
    //select the port from max via a message in inport 1
    message<> m_port { this, "port", "Port selection",
        MIN_FUNCTION {
            //            cout << "port received: " + std::to_string(args) + std::to_string(args.size()) << endl;
            if(args.size()){
                portname = args[0];
            }
            return {};
        }
    };
    
    //update the port information through the second outlet
    message<> bang { this, "bang", "List available input ports.",
        MIN_FUNCTION {
            display_ports(std::to_string(portname));
            return {};
        }
    };
    
    //    message<> m_anything { this, "anything", "Port selection",
    //        MIN_FUNCTION {
    //            cout << "anything received: " + std::to_string(args) << endl;
    //            return {};
    //        }
    //    };
    
    //    message<> m_list { this, "list", "Complete Midi message input",
    //        MIN_FUNCTION {
    //
    //            int length = args.size();
    //            cout << "length of list is: " + std::to_string(length) << endl;
    //            if (args[0] == "port") {
    //                cout << "port argument received" << endl;
    //            };
    //            return {};
    //        }
    //    };
    
    //normally we would use this to set up some things, but it doesn't seem to work as advertised at all.
    //dummy is always true, even when its a genuince instance.
    //    message<> maxclass_setup { this, "maxclass_setup",
    //        MIN_FUNCTION{
    //            cout << "attempt maxclass_setup" << endl;
    //            if(!dummy){
    //                cout << "maxclass_setup" << endl;
    //                midiin = new RtMidiIn(RtMidi::MACOSX_CORE);
    //            }
    //            return{};
    //        }
    ////      assign_port();
    //    };
    
    
    
private:
    
    
};


MIN_EXTERNAL(min_dots_dots_test2);




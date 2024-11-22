//amounra 021022

#include <iostream>
#include <queue>
#include "RtMidi.h"
#include "c74_min.h"
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()

using namespace c74::min;

//std::queue <int> midi_messages;

class midi4lin : public object<midi4lin> {
    
    
    
    
public:
    MIN_DESCRIPTION    {"Midi Input Object."};
    MIN_TAGS        {"utilities"};
    MIN_AUTHOR        {"aumhaa"};
    MIN_RELATED        {"midiin, midiparse, midiinfo"};
    
    inlet<>  input    { this, "(bang) send available input ports to output, (symbol) choose input port" };
    outlet<>  output { this, "output serialized midi data from the selected input port" };
    outlet<>  output2 { this, "output names of available input ports, bang on initialization" };
    
//    RtMidiIn *midiin = new RtMidiIn(RtMidi::MACOSX_CORE);
    //    RtMidiIn *midiin = 0;
    std::queue <int> midi_messages;
    bool initialized = false;
    long uid = time(0);
    RtMidiIn *midiin;


    
    ~midi4lin (){
        if(midiin){
            midiin->cancelCallback();
            midiin->closePort();
        }
        delete midiin;
    };
    
    midi4lin(const atoms& args = {}) {
        midiin = new RtMidiIn(RtMidi::MACOSX_CORE);
        defer_init.delay(1);
    }
    
    timer<> defer_init {this,
        MIN_FUNCTION {
            if (args.size() > 0) {
                portname = args[0];
            } else {
                portname = "None";
            };
            initialized = true;
            output2.send(k_sym_bang);
            defer_port_udpate.delay(1);
            cout << "midi4lin iniitialized: " + std::to_string(initialized) + "instance: " + std::to_string(uid) << endl;
            return {};
        }
    };
    
    
    //this is a hack to deal with a "bug", calling display_ports from instantiation gets ignored.
    timer<> defer_port_udpate {this,
        MIN_FUNCTION {
            display_ports(std::to_string(portname));
            return {};
        }
    };
    
    //    this is a hack to deal with a "bug", calling display_ports from setting attribute causes crash.
    timer<> defer_stored_port_udpate {this,
        MIN_FUNCTION {
//            cout << "here" + std::to_string(portname) << endl;
            display_ports(std::to_string(portname));
            return {};
        }
    };
    
    //this is a hack to deal with crashypants when we try to call output.send directly from a static member.
    timer<> defer {this,
        MIN_FUNCTION {
            while(!midi_messages.empty()){
//                int out = midi_messages.front();
//                output.send(out);
//                cout << "midiinCallback: " + std::to_string(out) + ":" + std::to_string(uid) << endl;
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
                if (initialized) {
                    string stored_portname = args[0];
                    assign_port(stored_portname);
                    //                display_ports(stored_portname);
                    defer_stored_port_udpate.delay(1);
                    return args;
                }
                return {};
            }
        }
    };
    
//    attribute<atom> available_ports { this, "ports", "None",
//        description {
//            "Available ports to connect to."
//            "The list of ports that are available."
//        },
//        setter {
//            MIN_FUNCTION {
//                return {};
//            }
//        },
//        getter {
//            MIN_GETTER_FUNCTION {
//                if (initialized) {
////                    string portName;
////                    int nPorts = midiin->getPortCount();
////                    atom portList[nPorts+1];
////                    portList[0]='None';
////                    for ( int i=0; i<nPorts; i++ ) {
////                        try {
////                            portName = midiin->getPortName(i);
////                            portList[i+1] = portName;
////                        }
////                        catch ( RtMidiError &error ) {
////                            error.printMessage();
////                        }
////                    }
////                    return {portList};
////                    cout << "anything received: " + std::to_string(args) << endl;
//                    cout << "testing" << endl;
////                    string portlist[1];
////                    atom portlist = ["None"];
//                    atom portlist[1];
//                    portlist[0] = ["None"];
//                    return {portlist};
//                }
//                return {};
//            }
//        }
//    };
    
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
    
//    //assign the port internally and set its callback.
//    void assign_port (string stored_portname) {
//        midiin->closePort();
//        if (stored_portname == "None") {
//            midiin->cancelCallback();
//            midiin->closePort();
//        }
//        else {
//            bool found = false;
//            int nPorts = midiin->getPortCount();
//            string portName;
//            for ( int i=0; i<nPorts; i++ ) {
//                try {
//                    portName = midiin->getPortName(i);
//                    if(stored_portname==portName){
//                        found = true;
//                        midiin->openPort(i, portName);
////                        midiin->setCallback(&midiinCallback, (void*)this);
//                        midiin->setCallback(&midiinCallback, (void*)midi4lin);
//                        midiin->ignoreTypes(false, false, false);
//                        cout << portName + " opened!" << endl;
//                    }
//                }
//                catch ( RtMidiError &error ) {
//                    error.printMessage();
//                }
//            }
//            if(!found){
//                try {
//                    midiin->openVirtualPort(stored_portname);
//                    midiin->setCallback(&midiinCallback, (void*)this);
//                    midiin->ignoreTypes(false, false, false);
//                    cout << stored_portname + " opened!" << endl;
//                }
//                catch ( RtMidiError &error ) {
//                    error.printMessage();
//                }
//            }
//        }
//    };

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
//                        midiin->setCallback(&midiinCallback, (void*)this);
                        midiin->setCallback(&midiinCallback, (void*)this);
                        midiin->ignoreTypes(false, false, false);
                        cout << portName + " opened!" << endl;
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
                    cout << stored_portname + " opened!" << endl;
                }
                catch ( RtMidiError &error ) {
                    error.printMessage();
                }
            }
        }
    };
    
    //this static function uses *userData as a pointer to the instance.  It can't call anything that
    //sends an output to max, so we defer it slightly and queue its messages to be handled by another method.
//    static void midiinCallback2 ( double deltatime, std::vector< unsigned char > *message, void *userData ) {
//        auto& owner = *(midi4lin*)userData;
////        auto* owner = static_cast<midi4lin*>(userData);
//        owner.defer.delay(0);
//        unsigned int nBytes = message->size();
//        for ( unsigned int i=0; i<nBytes; i++ ) {
//            midi_messages.push(message->at(i));
////            int out = message->at(i);
////            owner.output.send(out);
//        }
//    };
    
    // Static callback
    static void midiinCallback(double deltatime, std::vector<unsigned char>* message, void* userData) {
        auto* instance = static_cast<midi4lin*>(userData);
        if (instance) {
            unsigned long nBytes = message->size();
            for (unsigned long i = 0; i < nBytes; i++) {
                instance->midi_messages.push(message->at(i));
            }
            instance->defer.delay(0);  // Defer processing
        }
    }
    
    //select the port from max via a message in inport 1
    message<threadsafe::yes> m_port { this, "port", "Port selection",
        MIN_FUNCTION {
            //            cout << "port received: " + std::to_string(args) + std::to_string(args.size()) << endl;
            if(args.size()){
                portname = args[0];
            }
            return {};
        }
    };
    
    //update the port information through the second outlet
    message<threadsafe::yes> bang { this, "bang", "List available input ports.",
        MIN_FUNCTION {
            cout << "iniitialized: " + std::to_string(initialized) << endl;
            if(initialized){
                display_ports(std::to_string(portname));
            }
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
    
    
    
//private:
    
    
};


MIN_EXTERNAL(midi4lin);




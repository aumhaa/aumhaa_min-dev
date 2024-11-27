//amounra 021022

#include <iostream>
#include <queue>
#include "RtMidi.h"
#include "c74_min.h"
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()

using namespace c74::min;

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
    std::queue <int> midi_messages;
    std::vector<string>  port_list {};
    atom port_args = "None";
    atom virtual_args = false;
    bool initialized = false;
    long uid = time(0);
    string stored_portname = "None";
    bool is_virtual = false;
    RtMidiIn *midiin;
    int init_tries = 0;

    ~midi4lin (){
        if(midiin){
            midiin->cancelCallback();
            midiin->closePort();
        }
        delete midiin;
    };
    
    midi4lin(const atoms& args = {}) {
        midiin = new RtMidiIn(RtMidi::MACOSX_CORE);
        if (args.size() > 0) {
            port_args = args[0];
        }
        if (args.size() > 1) {
            virtual_args = args[1];
        }
        defer_init.delay(1);
    }
    
    timer<> defer_init {this,
        MIN_FUNCTION {
            initialized = true;
            portname = port_args;
            virtual_enabled = virtual_args;
            defer_port_udpate.stop();
            update_port_list(true);
            assign_port();
            defer_port_udpate.delay(2000);
            cout << "midi4lin iniitialized: " + std::to_string(initialized) + ", instance: " + std::to_string(uid) << endl;
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
    
    //this is a hack to deal with a "bug", calling display_ports from instantiation gets ignored.
    timer<> defer_port_udpate {this,
        MIN_FUNCTION {
            defer_port_udpate.stop();
            update_port_list();
            defer_port_udpate.delay(2000);
            return {};
        }
    };

    //this is a hack to deal with a "bug", calling display_ports from instantiation gets ignored.
    timer<> defer_portname {this,
        MIN_FUNCTION {
            portname = port_args;
            return {};
        }
    };
    
    //although we store the port in this attribute, we need to use a non-max value
    //for updating since setter has no option to set the data
    //before it proceeds with its sideeffects.
    attribute<symbol> portname { this, "portname", "None",
        description {
            "Port to connect to."
            "The port will be connected on instantiation."
        },
        setter {
            MIN_FUNCTION {
                if (initialized) {
//                        cout << port_args + " portname setter, " + std::to_string(init_tries) + " initialized: " + std::to_string(initialized) + ", instance: " + std::to_string(uid) << endl;
                    defer_port_udpate.stop();
                    stored_portname = std::string(args[0]);
                    update_port_list();
                    assign_port();
                    defer_port_udpate.delay(10);
                    return args;
                }
                else {
//                  init_tries += 1;
                    port_args = args[0];
//                  cout << port_args + " portname setter not initialized, instance: " + std::to_string(uid) << endl;
                }
                return {};
            }
        }
    };
    
    attribute<bool> virtual_enabled { this, "virtual_enabled", false,
        description {
            "Virtual port enable/disable."
            "When true a virtual endpoint will be created with given portname."
        },
        setter {
            MIN_FUNCTION {
                if (initialized) {
                    defer_port_udpate.stop();
                    is_virtual = bool(args[0]);
                    update_port_list();
                    assign_port();
                    defer_port_udpate.delay(10);
                    return args;
                }
                else {
                    virtual_args = args[0];
                }
                return {};
            }
        }
    };

    //update the port information through the second outlet
    message<> bang { this, "bang", "List available output ports.",
        MIN_FUNCTION {
            if (initialized) {
                update_port_list(true);
            }
            return {};
        }
    };
    
    std::vector<string> retrieve_current_port_list() {
        std::vector<string>  new_port_list = {};
        int nPorts = midiin->getPortCount();
        string portName;
        for ( int i=0; i<nPorts; i++ ) {
            try {
                portName = midiin->getPortName(i);
                new_port_list.push_back(portName);
            }
            catch ( RtMidiError &error ) {
                error.printMessage();
            }
        }
        if (is_virtual && (stored_portname!="None")) {
            auto it = std::find(new_port_list.begin(), new_port_list.end(), stored_portname);
            if (it == new_port_list.end()) {
                new_port_list.push_back(stored_portname);
            }
        }
        return new_port_list;
    }
    
    bool port_list_is_changed(std::vector<string> new_port_list) {
        return new_port_list != port_list;
    }

    void update_port_list(bool force = false) {
        std::vector<string> new_port_list = retrieve_current_port_list();
        if(force || port_list_is_changed(new_port_list)) {
            port_list = new_port_list;
//            reconnect_stored_port();
            display_ports();
        }
    }
    
    void display_ports () {
        long nPorts = port_list.size();
        string portName;
//        bool found = stored_portname == "None";
        output2.send("clear");
        output2.send("append", "None");

        for ( int i=0; i<nPorts; i++ ) {
            portName = port_list[i];
//            if (portName == stored_portname){
//                found = true;
//            }
            output2.send("append", portName);
        }
//        if (!found) {
//            output2.send("append", stored_portname);
//        }
        output2.send("setsymbol", stored_portname);
        display_assigned_port();
    }
    
    //automatically reconnect a stored port address when its port becomes available
    void reconnect_stored_port() {
//        if(!midiin->isPortOpen()) {
            assign_port();
//        }
    }
    
//    //assign the port internally and set its callback.
//    void assign_port_old () {
//        midiin->cancelCallback();
//        midiin->closePort();
//        if (stored_portname != "None") {
////            midiin->closePort();
//            bool found = false;
//            long nPorts = port_list.size();
//            string portName;
//            for ( int i=0; i<nPorts; i++ ) {
//                try {
//                    portName = port_list[i];
//                    if(stored_portname==portName){
//                        found = true;
//                        midiin->openPort(i, portName);
//                        midiin->setCallback(&midiinCallback, (void*)this);
//                        midiin->ignoreTypes(false, false, false);
//                        cout << portName + " opened!, initialized: " + std::to_string(initialized) + ", instance: " + std::to_string(uid) << endl;
//                    }
//                }
//                catch ( RtMidiError &error ) {
//                    error.printMessage();
//                }
//            }
//            if(!found && is_virtual){
//                try {
//                    midiin->openVirtualPort(stored_portname);
//                    midiin->setCallback(&midiinCallback, (void*)this);
//                    midiin->ignoreTypes(false, false, false);
//                    cout << stored_portname + " opened!, initialized: " + std::to_string(initialized) + ", instance: " + std::to_string(uid) << endl;
//                }
//                catch ( RtMidiError &error ) {
//                    error.printMessage();
//                }
//            }
//        }
//        display_assigned_port();
//    };
    
    //assign the port internally and set its callback.
    void assign_port () {
        midiin->cancelCallback();
        midiin->closePort();
        if (stored_portname != "None") {
//            midiin->closePort();
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
                        cout << portName << + " opened!, initialized: " + std::to_string(initialized) + ", instance: " + std::to_string(uid) << endl;
                    }
                }
                catch ( RtMidiError &error ) {
                    error.printMessage();
                }
            }
            if(!found && is_virtual){
                try {
                    midiin->openVirtualPort(stored_portname);
                    midiin->setCallback(&midiinCallback, (void*)this);
                    midiin->ignoreTypes(false, false, false);
                    cout << stored_portname << + " opened!, initialized: " + std::to_string(initialized) + ", instance: " + std::to_string(uid) << endl;
                }
                catch ( RtMidiError &error ) {
                    error.printMessage();
                }
            }
        }
        display_assigned_port();
    };
    
    
    void display_assigned_port() {
        atoms as{};
        as.push_back("checkitem");
        output2.send("clearchecks");
        auto it = std::find(port_list.begin(), port_list.end(), stored_portname);
        if (it != port_list.end()) {
            auto index = std::distance(port_list.begin(), it);
            as.push_back(index+1);
            output2.send(as);
            output2.send("setsymbol", stored_portname);
        } else {
            as.push_back(0);
            output2.send(as);
            output2.send("setsymbol", "None");
        }

    }
    
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
    message<threadsafe::no> m_port { this, "port", "Port selection",
        MIN_FUNCTION {
            //            cout << "port received: " + std::to_string(args) + std::to_string(args.size()) << endl;
            if(args.size()){
                portname = std::string(args[0]);
//                port_args = args[0];
//                defer_portname.delay(1);
            }
            return {};
        }
    };
    
    message<threadsafe::no> m_virtual { this, "virtual", "Port selection",
        MIN_FUNCTION {
            //            cout << "port received: " + std::to_string(args) + std::to_string(args.size()) << endl;
            if(args.size()){
                virtual_enabled = bool(args[0]);
//                port_args = args[0];
//                defer_portname.delay(1);
            }
            return {};
        }
    };
    
    //update the port information through the second outlet
//    message<threadsafe::yes> bang { this, "bang", "List available input ports.",
//        MIN_FUNCTION {
//            cout << "iniitialized: " + std::to_string(initialized) << endl;
//            if(initialized){
//                display_ports(std::to_string(portname));
//            }
//            return {};
//        }
//    };
    
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




//amounra 022122

#include <iostream>
#include <queue>
#include "RtMidi.h"
#include "c74_min.h"

using midi_byte = uint8_t;
using midi_data = midi_byte[3];


using namespace c74::min;

std::queue <int> midi_messages;

std::vector<unsigned char>  m4m_message {};

bool vport_is_open = false;

class midi4lout : public object<midi4lout> {
    

public:
    MIN_DESCRIPTION    {"Midi Input Object."};
    MIN_TAGS        {"utilities"};
    MIN_AUTHOR        {"aumhaa"};
    MIN_RELATED        {"midiin, midiparse, midiinfo"};
    
    inlet<>  input    { this, "serialized midi data input" };
    inlet<> input2 {this, "set output port, query available ports"};
    outlet<>  output2 { this, "output names of available input ports" };
    
    RtMidiOut *midiout = new RtMidiOut(RtMidi::MACOSX_CORE);

    
    
    ~midi4lout (){
        if(midiout){
            midiout->closePort();
        }
        delete midiout;
    };
    
    midi4lout(const atoms& args = {}) {
        if (args.size() > 0) {
            portname = args[0];
        };
        //        if (!midiout) {
        //            try {
        //                midiout = new RtMidiIn(RtMidi::MACOSX_CORE);
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
        int nPorts = midiout->getPortCount();
        string portName;
        bool found = false;
        output2.send("clear");
        output2.send("append", "None");
        if(stored_port_name == "None"){
            found = true;
        }
        for ( int i=0; i<nPorts; i++ ) {
            try {
                portName = midiout->getPortName(i);
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
        midiout->closePort();
        vport_is_open = false;
        if (stored_portname == "None") {
            midiout->closePort();
        }
        else {
            bool found = false;
            int nPorts = midiout->getPortCount();
            string portName;
            for ( int i=0; i<nPorts; i++ ) {
                try {
                    portName = midiout->getPortName(i);
                    if(stored_portname==portName){
                        found = true;
                        midiout->openPort(i, portName);
                        cout << portName + " opened!" << endl;
                    }
                }
                catch ( RtMidiError &error ) {
                    error.printMessage();
                }
            }
            if(!found){
                try {
                    //midiout->openVirtualPort(stored_portname);
                    midiout->openVirtualPort(stored_portname);
                    vport_is_open = true;
                    cout << stored_portname + " opened!" << endl;
                }
                catch ( RtMidiError &error ) {
                    error.printMessage();
                }
            }
        }
    };

    
//    //select the port from max via a message in inport 1
//    message<> m_port { this, "port", "Port selection",
//        MIN_FUNCTION {
//            //            cout << "port received: " + std::to_string(args) + std::to_string(args.size()) << endl;
//            if(args.size()){
//                portname = args[0];
//            }
//            return {};
//        }
//    };

    message<threadsafe::no> ints {
        this, "int", "MIDI byte or Port selection", MIN_FUNCTION {
            int a;
            unsigned char x;
            int stored_message_size = m4m_message.size();

            switch (inlet) {
                case 0:
                    if(args.size()==1){
                        a = args[0];
                        if(a == 247){
                            if(stored_message_size>1){
                                x = static_cast<unsigned char>(a);
                                m4m_message.push_back(x);
                            }
                        }
                        else if(a > 127){
                            m4m_message = {};
                            x = static_cast<unsigned char>(a);
                            m4m_message.push_back(x);
                        }
                        else if(a < 128){
                            x = static_cast<unsigned char>(a);
                            m4m_message.push_back(x);
                        }
                        if(validate_message()){
                            send_message();
                        }
                    }
                    break;
                case 1:
                    if(args.size()){
                        portname = args[0];
                    }
                    break;
                default:
                    assert(false);
            }
            return {};
        }
    };

    //update the port information through the second outlet
    message<> bang { this, "bang", "List available output ports.",
        MIN_FUNCTION {
            display_ports(std::to_string(portname));
            return {};
        }
    };
    
    void send_message() {
        
//        cout << "Send Message Out: " + std::to_string(num) << endl;

        int x;
        unsigned char a;
        std::vector<unsigned char>  msg {};
        int stored_message_size = m4m_message.size();

        if(stored_message_size>1){
            if ( midiout->isPortOpen() || vport_is_open) {
                for (int i=0; i<stored_message_size; i++) {
                    x = m4m_message[i];
                    a = static_cast<unsigned char>(x);
                    msg.push_back(a);
                }
                try {
                    midiout->sendMessage(&msg);
                }
                catch (RtMidiError &error) {
                    error.printMessage();
                    cout << "Send MIDI Error" << endl;
                }
            }
        }
    };
    
    
    //    message<> m_anything { this, "anything", "Port selection",
    //        MIN_FUNCTION {
    //            cout << "anything received: " + std::to_string(args) << endl;
    //            return {};
    //        }
    //    };
    
    message<> m_list { this, "list", "Complete Midi message input",
        MIN_FUNCTION {
            int x;
            unsigned char a;
            std::vector<unsigned char>  msg {};
            int length = args.size();
//            cout << "length of list is: " + std::to_string(length) << endl;
            
            if(length>1){
                if ( midiout->isPortOpen() || vport_is_open) {
                    for (int i=0; i<length; i++) {
                        x = args[i];
                        a = static_cast<unsigned char>(x);
                        msg.push_back(a);
                    }
                    try {
                        midiout->sendMessage(&msg);
                    }
                    catch (RtMidiError &error) {
                        error.printMessage();
                        cout << "Send MIDI Error" << endl;
                    }
                }
            }
            return {};
        }
    };
    
    //normally we would use this to set up some things, but it doesn't seem to work as advertised at all.
    //dummy is always true, even when its a genuince instance.
    //    message<> maxclass_setup { this, "maxclass_setup",
    //        MIN_FUNCTION{
    //            cout << "attempt maxclass_setup" << endl;
    //            if(!dummy){
    //                cout << "maxclass_setup" << endl;
    //                midiout = new RtMidiIn(RtMidi::MACOSX_CORE);
    //            }
    //            return{};
    //        }
    ////      assign_port();
    //    };
    
    
    
private:
    
    bool validate_message() {
        unsigned long status = m4m_message[0];
        int m_index = m4m_message.size();
        
            if (status < 0x80) {    // bad byte, should never happen
//                    m_index = 0;
                return false;
            }
            if (status < 0xF0) {    // channel message
                status = status & 0xF0;
//                    b1 = m_accumulated_midi_message[1];
//                    b2 = m_accumulated_midi_message[2];
                switch (status) {
                    case 0x80:
                    case 0x90:
                    case 0xA0:
                    case 0xB0:
                    case 0xE0:
                        if (m_index == 3) {
//                                m_accumulated_midi_message[1] = b1 & 0x7F;
//                                m_accumulated_midi_message[2] = b2 & 0x7F;
                            return true;
                        }
                        else
                            return false;
                        break;
                    case 0xD0:
                    case 0xC0:
                        if (m_index == 2) {
//                                m_accumulated_midi_message[1] = b1 & 0x7F;
                            return true;
                        }
                        else
                            return false;
                        break;
                    default:    // should never be here
                        return false;
                }
            }
            else {
                switch (status) {
                    case 0xF0:    // system exclusive
                        if (m4m_message[m_index-1] == 0xF7)
                            return true;
                        return false;
                    case 0xF1:    // undefined
                        return true;
                    case 0xF2:    // song position
                        if (m_index == 3)
                            return true;
                        else
                            return false;
                    case 0xF3:    // song select
                        if (m_index == 2)
                            return true;
                        else
                            return false;
                    case 0xF4:
                    case 0xF5:
                    case 0xF6:
                    case 0xF8:
                    case 0xF9:
                    case 0xFA:
                    case 0xFB:
                    case 0xFC:
                    case 0xFD:
                    case 0xFE:
                    case 0xFF:
                        return true;
                    case 0xF7:    // bad eox
                        m_index = 0;
                        return false;
                    default:
                        return false;
                }
            }
            return false;
        }
    };
    



MIN_EXTERNAL(midi4lout);




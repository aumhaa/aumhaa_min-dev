/// @file
///	@ingroup 	minexamples
///	@copyright	Copyright 2018 The Min-DevKit Authors. All rights reserved.
///	@license	Use of this source code is governed by the MIT License found in the License.md file.

#include <iostream>
#include <queue>
#include "RtMidi.h"
#include "c74_min.h"

//using namespace c74::min;

using midi_byte = uint8_t;
using midi_data = midi_byte[3];


using namespace c74::min;

std::queue <int> midi_messages;

std::vector<unsigned char>  m4m_message {};

bool vport_is_open = false;

class hello_world : public object<hello_world> {
public:
    MIN_DESCRIPTION	{"Post to the Max Console."};
    MIN_TAGS		{"utilities"};
    MIN_AUTHOR		{"Cycling '74"};
    MIN_RELATED		{"print, jit.print, dict.print"};
    
    inlet<>  input	{ this, "(bang) post greeting to the max console" };
    inlet<> input2 {this, "set output port, query available ports"};
    //    outlet<> output	{ this, "(anything) output the message which is posted to the max console" };
    outlet<>  output2 { this, "output names of available input ports" };
    RtMidiOut *midiout = new RtMidiOut(RtMidi::MACOSX_CORE);
    
    ~hello_world (){
        if(midiout){
            midiout->closePort();
        }
        delete midiout;
    };
    
    hello_world(const atoms& args = {}) {
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
    
    timer<> defer_port_udpate {this,
        MIN_FUNCTION {
            cout << "hello world 2" << endl;
            return {};
        }
    };
    
    attribute<symbol> portname { this, "port", "None",
        description {
            "Port to connect to."
            "The port will be connected on instantiation."
        },
        setter {
            MIN_FUNCTION {
                string stored_portname = args[0];
                assign_port(stored_portname);
                //                display_ports(stored_portname);
                return args;
            }
        }
    };
    
    //update the port information through the second outlet
    message<> bang { this, "bang", "List available output ports.",
        MIN_FUNCTION {
            display_ports(std::to_string(portname));
            return {};
        }
    };

    //update the port information through the second outlet
    message<> create_port { this, "create_port", "Create a virtual port.",
        MIN_FUNCTION {
            create_port(std::to_string(portname));
            return {};
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
    }
    
    message<threadsafe::yes> m_ints {
        this, "int", "MIDI byte or Port selection", MIN_FUNCTION {
            int a;
            unsigned char x;
            int stored_message_size = m4m_message.size();
            a = args[0];
            //          cout << "m_ints:" + std::to_string(a) << endl;
            switch (inlet) {
                case 0:
                    //                    if(args.size()==1){
                    if(a == 0x8f){
                        if(stored_message_size>1){
                            x = static_cast<unsigned char>(a);
                            m4m_message.push_back(x);
                        }
                    }
                    else if(a >= 0x80){
                        m4m_message = {};
                        x = static_cast<unsigned char>(a);
                        m4m_message.push_back(x);
                    }
                    else if(a < 0x80){
                        x = static_cast<unsigned char>(a);
                        m4m_message.push_back(x);
                    }
                    //                    }
                    if(validate_message()){
                        send_message();
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
    
    void send_message() {
        
//        cout << "Send Message Out: " + std::to_string(num) << endl;
//        cout << "Send Message:" << endl;
        int x;
        unsigned char a;
        std::vector<unsigned char>  msg {};
        int stored_message_size = m4m_message.size();

        if(stored_message_size>1){
            if ( midiout->isPortOpen() || vport_is_open) {
                for (int i=0; i<stored_message_size; i++) {
                    x = m4m_message[i];
//                    cout << "item:" + std::to_string(x) << endl;
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
        m4m_message.clear();
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
            cout << "length of list is: " + std::to_string(length) << endl;
            
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
    
    
//private:
    
    bool validate_message() {
        unsigned long status = m4m_message[0];
        int m_index = m4m_message.size();
        //        cout << "validate message:" + std::to_string(status) + " " + std::to_string(m_index) << endl;
        if (status < 0x80) {    // bad byte, should never happen
            //                    m_index = 0;
            m4m_message.clear();
            return false;
        }
        else if (status < 0xF0) {    // channel message
            status = status & 0xF0;
            switch (status) {
                case 0x80:
                    //note off
                    //                        cout << "status noteOff" << endl;
                    if (m_index == 3) {
                        return true;
                    }
                    else if(m_index > 3) {
                        m4m_message.clear();
                    }
                    return false;
                    break;
                case 0x90:
                    //note on
                    //                        cout << "status noteOn" << endl;
                    if (m_index == 3) {
                        return true;
                    }
                    else if(m_index > 3) {
                        m4m_message.clear();
                    }
                    return false;
                    break;
                case 0xA0:
                    //poly pressure
                    if (m_index == 3) {
                        return true;
                    }
                    else if(m_index > 3) {
                        m4m_message.clear();
                    }
                    return false;
                    break;
                case 0xB0:
                    //cc
                    //                        cout << "status CC" << endl;
                    if (m_index == 3) {
                        return true;
                    }
                    else if(m_index > 3) {
                        m4m_message.clear();
                    }
                    return false;
                    break;
                case 0xE0:
                    //pb
                    if (m_index == 3) {
                        return true;
                    }
                    if (m_index > 3) {
                        m4m_message.clear();
                    }
                    return false;
                    break;
                case 0xD0:
                    //chan pressure
                    if (m_index == 2) {
                        return true;
                    }
                    else if(m_index > 2) {
                        m4m_message.clear();
                    }
                    return false;
                    break;
                case 0xC0:
                    //program change
                    if (m_index == 2) {
                        //                                m_accumulated_midi_message[1] = b1 & 0x7F;
                        return true;
                    }
                    else if(m_index > 2) {
                        m4m_message.clear();
                    }
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
                    break;
                case 0xF1:    // undefined
                    m4m_message.clear();
                    return false;
                    break;
                case 0xF2:    // song position
                    if (m_index == 3) {
                        return true;
                    }
                    else if (m_index > 3) {
                        m4m_message.clear();
                    }
                    return false;
                    break;
                case 0xF3:    // song select
                    if (m_index == 2)
                        return true;
                    else if (m_index > 2){
                        m4m_message.clear();
                    }
                    return false;
                    break;
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
                    break;
                case 0xF7:    // bad eox
                    m_index = 0;
                    m4m_message.clear();
                    return false;
                    break;
                default:
                    m4m_message.clear();
                    return false;
            }
        }
        m4m_message.clear();
        return false;
    }
    
};

MIN_EXTERNAL(hello_world);

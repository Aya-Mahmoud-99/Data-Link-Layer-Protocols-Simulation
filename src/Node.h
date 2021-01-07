//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __SECTION1_NODE_H_
#define __SECTION1_NODE_H_

#include <omnetpp.h>
#include <bitset>
#include "MyMessage_m.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
using namespace omnetpp;


/**
 * TODO - Generated class
 */
#define MAX_SEQ 7
class Node : public cSimpleModule
{
    void send_data();
    void inc(int &k);
  protected:
    // to calculate
    std::ifstream fileIn;
    int noElementsFile;
    int timeInterval=2;
    bool endSession;
    MyMessage_Base* timer[MAX_SEQ+1];
    static int received_messages;
    static int generated_frames;
    static int retransmitted_frames;
    static int total_transmitted_data;
    static int dropped_frames;
    static int useful_transmitted_data;
    static bool between(int a,int b,int c);
    int next_frame_to_send;
    int ack_expected;
    int frame_expected;
    // node wa7da wala kter?
    std::vector<std::string> buffer;
    int dest;
    std::string fileName;
    //int dest [MAX_SEQ+1];
    int nbuffered;
    virtual void initialize();

    virtual void handleMessage(cMessage *msg);
};

#endif

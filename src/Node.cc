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

#include "Node.h"
Define_Module(Node);
void Node::send_data(){
    MyMessage_Base* mmsg=new MyMessage_Base("Hello");

    mmsg->setM_Payload(buffer[next_frame_to_send].c_str());
    // call function to add haming code check bits
    // call function to randomize error
    mmsg->setSeq_Num(next_frame_to_send);
    // to ask
    mmsg->setAckSeq_Num((frame_expected+MAX_SEQ)%(MAX_SEQ+1));
    // send to whom?
    send(mmsg,"outs",dest[next_frame_to_send]);
    scheduleAt(simTime() + timeInterval, timer);
}
bool Node::between(int a,int b,int c){
    if( ((a<=b)&&(b<c)) || ((c<a)&&(a<=b)) ||((b<c)&&(c<a)) ) return true;
    return false;
}
void Node::inc(int &k){
if(k<MAX_SEQ) k=k+1;
else k=0;
}
void Node::initialize()
{
    // TODO - Generated method body
    next_frame_to_send=0;
    ack_expected=0;
    frame_expected=0;
    nbuffered=0;
   /* if ( strcmp(getName(),"Tic")==0)
    {
        MyMessage_Base * msg = new MyMessage_Base("Hello");
        msg->setM_Payload("hi there!");
        msg->setM_Type(1);
        msg->setSeq_Num(12);
        std::bitset<8> temp('A');
        msg->setMycheckbits(temp);
      send(msg,"out");
    }*/
}

void Node::handleMessage(cMessage *msg)
{
    /*// TODO - Generated method body
    MyMessage_Base *mmsg = check_and_cast<MyMessage_Base *>(msg);
    EV<<"received message with sequence number ...   ";
    EV << mmsg->getSeq_Num();
    EV<<"  and payload of ... ";
    EV<< mmsg->getM_Payload();
    EV<<"   and check bits of ...";
    EV<< mmsg->getMycheckbits().to_string();

    int rand=uniform(0,1)*10;
       EV<<"rand is "<<std::to_string(rand)<<endl;
       if(rand<8) // prob to delay the message
       {
           std::string mypayload= mmsg->getM_Payload();
           mypayload[0]=mypayload[0]+5;
           mmsg->setM_Payload(mypayload.c_str());
       }

    send(mmsg,"out");*/
    MyMessage_Base *mmsg = check_and_cast<MyMessage_Base *>(msg);
    // if the message is data and ack
    if(mmsg->getM_Type()==0){
    if(mmsg->getSeq_Num()==frame_expected){
        received_messages++;
        //correct using haming code
        inc(frame_expected);
    }
    while(between(ack_expected, mmsg->getAckSeq_Num(), next_frame_to_send)){
        nbuffered=nbuffered-1;
        cancelAndDelete(timer);
        inc(ack_expected);
    }
    }
    //if the message is time out
    if(mmsg->getM_Type()==1){

        next_frame_to_send=ack_expected;
        for(int i=0;i<nbuffered;i++){
            send_data();
            inc(next_frame_to_send);
        }
    }
    if(mmsg->getM_Type()==2){
        dest[next_frame_to_send]=atoi(mmsg->getName());
        buffer[next_frame_to_send]=mmsg->getM_Payload();
        nbuffered=nbuffered+1;
        send_data();
        inc(next_frame_to_send);

    }

}
int Node::received_messages=0;
int Node::sent_messages=0;
int Node::retransmitted_frames=0;
int Node::total_transmitted_data=0;




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
    mmsg->setM_Type(0);
    MyMessage_Base* dmsg=mmsg->dup();
    // send to whom?
    int rand1=uniform(0,1)*10;
      // EV<<"rand is "<<std::to_string(rand)<<endl;
       if(rand1>=par("pLoss").intValue()) // prob to delay the message
       {
           send(mmsg,"outs",dest);
       }
       else{
           dropped_frames++;
       }
       int rand2=uniform(0,1)*10;
       if(rand2<par("pDup").intValue()) // prob to delay the message
       {
           //send(dmsg,"outs",dest);
           double interval = exponential(1 / par("lambda").doubleValue());
           //scheduleAt(simTime() + interval, dmsg,"outs",dest);
           sendDelayed(dmsg, interval,"outs",dest);
       }
       timer[next_frame_to_send] =new MyMessage_Base("timer");
       timer[next_frame_to_send]->setM_Type(1);
       scheduleAt(simTime() + timeInterval, timer[next_frame_to_send]);
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
    endSession=false;
}

void Node::handleMessage(cMessage *msg)
{
    MyMessage_Base *mmsg = check_and_cast<MyMessage_Base *>(msg);
    // if the message is data and ack
    if(mmsg->getM_Type()==0){
        EV<<"DATA";
    if(mmsg->getSeq_Num()==frame_expected){
        received_messages++;
        //correct using haming code
        inc(frame_expected);
    }
    while(between(ack_expected, mmsg->getAckSeq_Num(), next_frame_to_send)){
        nbuffered=nbuffered-1;
        cancelAndDelete(timer[ack_expected]);
        inc(ack_expected);
    }
    }
    //if the session has ended
    if(mmsg->getM_Type()==3){
        for(int i=0;i<MAX_SEQ+1;i++){
           if(timer[i]) cancelAndDelete(timer[i]);
        }
    }
   if(mmsg->getM_Type()==1){
       EV<<"TIME OUT";
        next_frame_to_send=ack_expected;
        for(int i=0;i<nbuffered;i++){
            retransmitted_frames++;
            send_data();
            inc(next_frame_to_send);
        }
    }
   // }
    // a file received from parent module
    if(mmsg->getM_Type()==2){
        EV<<"FILE";
        dest=atoi(mmsg->getName());
        fileName=mmsg->getM_Payload();

        fileIn.open(fileName);
       // int interval=0;
        EV<<fileName;
        std::string str;
        while (!getline (fileIn, str))
        {
            if(nbuffered>MAX_SEQ)break;
           // std::string str;
            fileIn >> str;
            EV<<str<<std::endl;
            buffer.push_back(str);
            nbuffered=nbuffered+1;
            send_data();
            inc(next_frame_to_send);
        }
        MyMessage_Base* fmsg=new MyMessage_Base("Hello");
        fmsg->setM_Type(3);
        send(fmsg,"outs",dest);
    }
    }
int Node::received_messages=0;
int Node::generated_frames=0;//toddddddo
int Node::retransmitted_frames=0;
int Node::total_transmitted_data=0;
int Node::dropped_frames=0;
int Node::useful_transmitted_data=0;//todooooooooooooooooooo




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
        cancelAndDelete(timer[ack_expected]);
        inc(ack_expected);
    }
    while (!fileIn.eof())
    {
        if(nbuffered>MAX_SEQ)break;
        std::string str;
        fileIn >> str;
        //MyMessage_Base* smsg=new MyMessage_Base("Hello");

        //smsg->setM_Payload(str.c_str());
        //smsg->setM_Type(3);
        // call function to add haming code check bits
        // call function to randomize error
        // to ask
        //double interval = exponential(1 / par("lambda").doubleValue());
        //scheduleAt(simTime() + interval, smsg);
        //interval++;
       // buffer.push_back(str);
       // nbuffered=nbuffered+1;
        buffer.push_back(str);
        nbuffered=nbuffered+1;
        send_data();
        inc(next_frame_to_send);
    }
    }
    //if the message is time out
    if(mmsg->getM_Type()==1){

        next_frame_to_send=ack_expected;
        for(int i=0;i<nbuffered;i++){
            retransmitted_frames++;
            send_data();
            inc(next_frame_to_send);
        }
    }
    // a file received from parent module
    if(mmsg->getM_Type()==2){
        dest=atoi(mmsg->getName());
        fileName=mmsg->getM_Payload();
        fileIn.open(fileName);
       // int interval=0;
        while (!fileIn.eof())
        {
            if(nbuffered>MAX_SEQ)break;
            std::string str;
            fileIn >> str;
            //MyMessage_Base* smsg=new MyMessage_Base("Hello");

            //smsg->setM_Payload(str.c_str());
            //smsg->setM_Type(3);
            // call function to add haming code check bits
            // call function to randomize error
            // to ask
            //double interval = exponential(1 / par("lambda").doubleValue());
            //scheduleAt(simTime() + interval, smsg);
            //interval++;
           // buffer.push_back(str);
           // nbuffered=nbuffered+1;
            buffer.push_back(str);
            nbuffered=nbuffered+1;
            send_data();
            inc(next_frame_to_send);
        }
    }
      /*  MyMessage_Base* smsg=new MyMessage_Base("Hello");
        smsg->setM_Type(4);
        // call function to add haming code check bits
        // call function to randomize error
        // to ask
        scheduleAt(simTime() + interval, smsg);*/
       // send_data();
     //   inc(next_frame_to_send);
    }
    // the node scheduled this data to it self when it read it from the file
   /* if(mmsg->getM_Type()==3){

            buffer.push_back(mmsg->getM_Payload());
            nbuffered=nbuffered+1;
            send_data();
            inc(next_frame_to_send);
    }*/
    // the node scheduled this data to it self when it readed it from the file
   /* if(mmsg->getM_Type()==4){
        MyMessage_Base* smsg=new MyMessage_Base("Hello");
        smsg->setM_Type(4);
        send(smsg,"outs",dest);

    }

}*/
int Node::received_messages=0;
int Node::generated_frames=0;//toddddddo
int Node::retransmitted_frames=0;
int Node::total_transmitted_data=0;
int Node::dropped_frames=0;
int Node::useful_transmitted_data=0;//todooooooooooooooooooo



